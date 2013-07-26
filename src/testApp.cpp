#include <string>
#include "testApp.h"
#include "states.h"
#include "EmptyStates.h"
#include "GameRecord.h"
#include "STSoundPlayer.h"

using namespace PSL;

//#define INSPECT_INTERVAL

#ifdef INSPECT_INTERVAL
#define INTERVAL_HIST_SIZE	1024
long interval_hist[INTERVAL_HIST_SIZE];
long long last_micros;
long *ptr;
#endif


struct Blackout
{
	bool enabled;
	int  time;
	int  frames;
	Blackout()
	{
		enabled = false;
		time    = 0;
		frames  = 0;
	}
};

static Blackout blackout;


static PSL::variable get_date_ymd()
{
	time_t now;
	time(&now);
	tm* date = localtime(&now);
	
	PSL::variable ret;
	ret[0] = date->tm_year;
	ret[1] = date->tm_mon;
	ret[2] = date->tm_mday;
	return ret;
}

void testApp::init_psl()
{
	// スクリプト環境の準備
	//if (psl) delete psl;

	//psl = new PSLVM();
	if (psl->loadScript("script.psl")) exit();

	psl->addFunction("get_date_ymd", &get_date_ymd);
	psl->addFunction("change_state", &testApp::change_state);
	psl->addFunction("play_sound", &testApp::play_sound);
	psl->addFunction("play_sound_vol", &testApp::play_sound_vol);
	psl->addFunction("fadein_sound", &testApp::fadein_sound);
	psl->addFunction("fadeout_sound", &testApp::fadeout_sound);
	psl->addFunction("fade_sound", &testApp::fade_sound);
	psl->addFunction("stop_sound", &testApp::stop_sound);
	psl->addFunction("shout", &testApp::shout);
	psl->addFunction("blackout", &testApp::api_blackout);
	psl->addFunction("enable_frame_signal", &enable_frame_signal);
	psl->addFunction("disable_frame_signal", &disable_frame_signal);
	
	psl->run();
	printf_s("PSL script loaded.\n");
}

//--------------------------------------------------------------
void testApp::setup(){
	idle_frame = 0;

	// スクリプト環境の準備
	//psl = NULL;
	psl = new PSLVM();
	init_psl();

	// 状態クラスの準備
	current_state = NULL;
	next_state = new InitedState(this);
	new CalibratingState(this);
	new ReadyState(this);
	new LoadingState(this);
	new WaitingRunState(this);
	new PreRunningState(this);
	new RunningState(this);
	new GoalState(this);
	new WaitingPlaybackState(this);
	new PlayingBackState(this);
	new PlaybackGoalState(this);
	new SavingState(this);
	new SaveEndState(this);
	new ErrorState(this);

	// サウンド素材の準備
	setup_sounds();

	// 通信関係のクラスの準備
	variable str;
	str = psl->get("client_port");			int port = str.toInt();
	str = psl->get("broadcast_address");	std::string address = str;
	char addr[1024];
	strcpy(addr, address.c_str());
	printf_s("sender.Create() => %d\n", sender.Create());
	sender.SetEnableBroadcast(true);
	printf_s("sender.Connect(%s, %d) => %d\n", addr, port, sender.Connect(addr, port));
	// UDP受信スレッド
	str = psl->get("server_port");	port = str.toInt();
	receiver_t.setup(this, port);
	receiver_t.startThread(true, false);

	opInterface = NULL;
#ifdef INSPECT_INTERVAL
	last_micros = ofGetElapsedTimeMicros();
	memset(interval_hist, 0, sizeof(long) * INTERVAL_HIST_SIZE);
	ptr = interval_hist;
#endif

	// 画面やOF関係の準備
	ofSetFrameRate(30);
	current_game = NULL;
}

//--------------------------------------------------------------
void testApp::update(){

	if (blackout.enabled)
	{
		++blackout.time;
		if (blackout.frames==blackout.time)
		{
			blackout.enabled = false;
		}
		const int F = blackout.frames;
		const int T = blackout.time;
		static char buf[1024];
		sprintf(buf, "COLOR-OVERLAY 0 0 0 %d", 255*(T)/F);
		shout(buf);
	}
	

	// フレーム同期信号
	if (frame_sig.enabled)
	{
		static char buf[1024];
		sprintf(buf, "FRAME %d", frame_sig.index++);
		shout(buf);
	}

	update_sounds();

#ifdef INSPECT_INTERVAL
	long long now_micros = ofGetElapsedTimeMicros();
	*ptr = (long)(now_micros - last_micros);
	ptr--;
	if (ptr < interval_hist) {
		ptr = interval_hist + INTERVAL_HIST_SIZE - 1;
	}
	last_micros = now_micros;
#endif

	if (this->msg.request)
	{
		msg.lock();
		for (int i=0; i<msg.received_message.size(); ++i)
		{
			static char buffer[10000];
			strcpy(buffer, msg.received_message[i].c_str());
			char *argv[100];
			int argc;
			char *ptr = buffer;
			do {
				ptr = tokenize(ptr, argv, argc);
				//for (int i=0; i<argc; i++) printf_s("argv[%d]: %s\n", i, argv[i]);
				this->dispatchMessage(argv, argc, this->msg.address);
			} while (ptr != NULL);
		}
		this->msg.received_message.clear();
		this->msg.done = true;
		this->msg.unlock();
	}

	// 状態を遷移する場合は前の後始末と次の準備をする。
	if (next_state != current_state) {
		if (current_state != NULL) current_state->leaveState();
		if (next_state != NULL) next_state->enterState();
		current_state = next_state;
		if (opInterface != NULL && current_state != NULL)
			opInterface->state_changed(current_state->name());
	}

	// 現在の状態でやるべきことをやる。（次の状態が返る）
	if (current_state != NULL) {
		State *next = current_state->updateState();
		// スクリプトの中での状態遷移を優先させるために以下の条件をつける。
		if (next_state == current_state) next_state = next;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	char buf[1024];
	int y = 0;
	ofBackground(0);

	y = 30;
	// インストラクターUIとクライアント機の状態
	ofSetColor(255, 255, 0);
	ofDrawBitmapString("* Instractor UI *", 220, y);
	y += 15;
	if (opInterface != NULL) {
		ofSetColor(255, 255, 255);
		sprintf(buf, "Instructor UI @ %s", opInterface->address.c_str());
		ofDrawBitmapString(buf, 220, y);
	}
	y += 15;

	ofSetColor(255, 255, 0);
	ofDrawBitmapString("* Client Machines *", 220, y);
	y += 15;
	map<std::string, STClient*>::iterator it = clients.begin();
	while (it != clients.end()) {
		STClient *client = it->second;
		sprintf_s(buf, "* %s *", client->status.c_str());
		ofSetColor(0, 255, 0);
		ofDrawBitmapString(buf, 220, y);
		sprintf_s(buf, "client[%s]: %s @ %s", client->number.c_str(), client->name.c_str(), client->address.c_str());
		ofSetColor(255, 255, 255);
		ofDrawBitmapString(buf, 300, y);
		y += 15;
		it++;
	}
	y += 10;

	// 現在のコントローラーのステート
	ofSetColor(255, 255, 0);
	ofDrawBitmapString("* CONTROLLER STATE *", 20, y);
	y += 15;
	ofSetColor(255, 255, 255);
	if (current_state != NULL)
		ofDrawBitmapString(current_state->name(), 20, y);
	else
		ofDrawBitmapString("none", 20, y);
	y += 15;

	// フレーム同期信号の状態
	ofSetColor(255);
	sprintf(buf, "frame-index:  %4d (%s)", frame_sig.index, frame_sig.enabled?"true":"false");
	ofDrawBitmapString(buf, 20, y+=15);
	sprintf(buf, "state->frame: %4d", this->current_state->getFrameNum());
	ofDrawBitmapString(buf, 20, y+=15);
	sprintf(buf, "idle-frame:   %4d", idle_frame);
	ofDrawBitmapString(buf, 20, y+=15);


	// フレームレートの傾向
#ifdef INSPECT_INTERVAL
	y = 300;
	ofColor(128, 128, 128);
	long *p = ptr;
	for (int i=0; i< INTERVAL_HIST_SIZE; i++) {
		ofLine(20+i, y - 0.001*(*p), 20+i, y);
		p++;
		if (p == interval_hist + INTERVAL_HIST_SIZE) p = interval_hist;
	}
	ofColor(255, 0, 0);
	ofLine(20, y - 33, 20+INTERVAL_HIST_SIZE, y - 33);
#endif
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	variable on_key = psl->get("on_key");

	switch (key) {
	case 'r':
		init_psl();
		break;
	case 'i':
		init_psl();
		//shout("RELOAD-CONFIG");
		shout("INIT");
		change_state("inited");
		break;
		/*
	case '1':		play_sound("se1");	break;
	case '2':		play_sound("se2");	break;
	case '3':		play_sound("se3");	break;
	case '4':		play_sound("se4");	break;
	case '5':		play_sound("se5");	break;
	case '6':		play_sound("se6");	break;
	*/
	default:
		if (current_state != NULL) {
			sprintf(sendbuf, "%c", key);
			on_key(current_state->name().c_str(), sendbuf);
		}
	}
}

//--------------------------------------------------------------
void testApp::exit(){
	receiver_t.stopThread();
}

/***************************************************************
	スクリプトからも呼び出せるメソッド
***************************************************************/
//--------------------------------------------------------------
// 状態を遷移させる。
int testApp::change_state(const char * state_name){
	printf_s("change state: %s\n", state_name);
	testApp *me = (testApp*)ofGetAppPtr();
	State *s = State::byName(state_name);
	me->next_state = s;
	return 0;
}

//--------------------------------------------------------------
// サウンドを鳴らす・ループ再生する
int testApp::play_sound_vol(const char * name, float vol){
	return play_sound(name, false, vol, 0.0f);
}
int testApp::play_sound(const char * name, int loop, float vol, float pan){
	testApp *me = (testApp*)ofGetAppPtr();
	map<std::string, STSoundPlayer*>::iterator it = me->sounds.find(name);
	if (it != me->sounds.end()) {
		if (it->second->isLoaded()) {
			STSoundPlayer *player = it->second;
			player->setVolume(vol);
			player->current_volume = vol;
			player->setPan(pan);
			if  (loop)
				it->second->play_loop();
			else
				it->second->play();
		} else {
			printf_s("sound %s is not loaded\n", name);
		}
	} else {
		printf_s("sound %s is not found.\n", name);
	}

	return 0;
}
// サウンドをフェードさせる。
int testApp::fadein_sound(const char *name, float target_volume, int frames, int loop)
{
//#	printf_s("fade in ///// %s\n", name);
	fade_sound(name, target_volume, frames, 0.0);
	play_sound(name, loop);
	return 0;
}
int testApp::fadeout_sound(const char *name, int frames)
{
//#	printf_s("fade out \\\\\\\\\\ %s\n", name);
	return fade_sound(name, 0.0, frames, -1.0);
}
int testApp::fade_sound(const char *name, float target_volume, int frames, float initial_volume, int autoonoff, int loop)
{
	testApp *me = (testApp*)ofGetAppPtr();
//#	printf("fade_sound: %s  target:%5.3f, frame:%d, init(%5.3f), auto=%d loop=%d\n", name ,target_volume, frames, initial_volume, autoonoff, loop);
	map<std::string, STSoundPlayer*>::iterator it = me->sounds.find(name);
	if (it != me->sounds.end()) {
		STSoundPlayer *player = it->second;
		player->start_fade(target_volume, frames, initial_volume, autoonoff);
		if (player->current_volume == 0 && ! player->getIsPlaying()) {
			if (loop) player->play_loop();
			else player->play();
		}
	}
	return 0;
}

// サウンドを停止する
int testApp::stop_sound(const char * name){
	testApp *me = (testApp*)ofGetAppPtr();
	map<std::string, STSoundPlayer*>::iterator it = me->sounds.find(name);
	if (it != me->sounds.end()) {
		if (it->second->isLoaded()) {
			it->second->stop();
		} else {
			printf_s("sound %s is not loaded\n", name);
		}
	}
	return 0;
}


//--------------------------------------------------------------
// 画面のブラックアウト
void testApp::api_blackout(int frames)
{
	blackout.enabled = true;
	blackout.time    = 0;
	blackout.frames  = frames;
}

void testApp::enable_frame_signal()
{
	testApp *me = (testApp*)ofGetAppPtr();
	me->frame_sig.enabled = true;
	me->frame_sig.index   = 0;
}

void testApp::disable_frame_signal()
{
	testApp *me = (testApp*)ofGetAppPtr();
	me->frame_sig.enabled = false;
	me->frame_sig.index   = 0;
}




//--------------------------------------------------------------
// 全クライアント機にメッセージをブロードキャストする。
int testApp::shout(const char * message){
	char message_buf[1024];
	sprintf_s(message_buf, "%s\n", message);
	testApp *me = (testApp*)ofGetAppPtr();
	variable str = me->psl->get("without_shout");
	int without_shout = str;
	if (! without_shout)
		me->sender.Send(message_buf, strlen(message_buf));

	if (strncmp(message_buf, "FRAME ", 6) == 0) {
		// nop
	} else if (strncmp(message_buf, "IDLE-FRAME ", 11) == 0) {
		// nop
	} else {
		printf_s("SHOUT: ");
		printf_s(message);
		printf_s("\n");
	}
	return 0;
}


/***************************************************************
	Stateの中から主に呼ばれるメソッド
***************************************************************/
//--------------------------------------------------------------
bool testApp::allPearConnected() const
{
	const int expected_number_of_clients = PSLv(psl->get("expected_number_of_clients"));
	const bool need_instructor_interface = PSLv(psl->get("need_instructor_interface"));

	// クライアントが期待する台数接続するまで、接続要求をする
	if (clients.size() < expected_number_of_clients)
		return false;
	// インストラクターUIが必要で、接続されていなければ、接続要求をする
	if (need_instructor_interface && opInterface==NULL)
		return false;

	// これ以上の接続要求なし
	return true;
}

bool testApp::allClientIs(const std::string& status) const
{
	auto it = clients.begin();
	while (it != clients.end()) {
		if (it->second->status != status) {
			//shout("STATUS");
			return false;
		}
		it++;
	}
	return true;
}

/***************************************************************
	内部的に使われるメソッド
***************************************************************/
//--------------------------------------------------------------
STClient* testApp::getClient(std::string address){
	map<std::string, STClient*>::iterator it = clients.find(address);
	return it == clients.end() ? NULL : it->second;
}

void testApp::loadGameRecord(GameRecord *record)
{
	if (record == NULL) {
		printf_s("NULL RECORD\n");
		return;
	}

	// 呼び出し側ではオブジェクトの処理はしないので、
	// testApp側で解放すること。
	current_game = record;

	std::vector<std::string> messages;
	record->get_messages(messages);
	std::vector<std::string>::iterator it = messages.begin();
	while (it != messages.end()) {
		printf_s("GameRecord: %s\n", it->c_str());
		shout(it->c_str());
		it++;
	}
	change_state("loading");
}

bool testApp::saveGameRecord()
{
	if (current_game != NULL) {
		if (! current_game->insert_to_database()) {
			// DBに格納できなかった場合。
			while (current_game->post_t.done==false)
			{
				Sleep(1);
			}
			printf("*** SAVE TO DB FAILED ***\n%s\n", current_game->error_content.c_str());
			return false;
		}
	}
	return true;
}

void testApp::releaseGameRecord()
{
	if (current_game != NULL) {
		delete current_game;
		current_game = NULL;
	}
}



//--------------------------------------------------------------
void testApp::dispatchMessage(char *argv[], int argc, std::string from){

	STClient *client;
	variable on_message_in_psl = psl->get("on_message");
	char message_buf[1024];

	if (argc < 0) return;

	if (strcmp(argv[0], "PONG") == 0) {

		// PINGへの応答の場合
		if (argc >= 3 && strcmp(argv[1], "INSTRUCTOR_UI") == 0) {
			// オペレータインタフェースからのPONG INSTRUCTOR_UI <port>
			if (opInterface) delete opInterface;
			opInterface = new STOperatorInterface(this, from, atoi(argv[2]));
		} else {
			// その他からのPONG
			client = getClient(from);
			if (argc < 4) {
				printf_s("ERROR: Invalid PONG response: argc=%d from %s\n", argc, from.c_str());
			} else {
				if (client == NULL) {
					clients[from] = new STClient(std::string(argv[1]), std::string(argv[2]), std::string(argv[3]));
					printf_s("Receive PONG response: name=%s number=%s from %s\n", argv[1], argv[3], from.c_str());
				}
			}
		}

	} else if (strcmp(argv[0], "GAME-TIMEOUT") == 0) {
		printf_s("GameTimeout!!\n");
		variable(psl->get("on_game_timeout"))();

	} else if (opInterface != NULL && opInterface->address == from) {

		//	インストラクターUIからのメッセージの場合、UIに処理を振る。
		opInterface->message(argv, argc);

	} else if (client = getClient(from)) {

		if (strcmp(argv[0], "STATUS") == 0) {
			if (argc < 3) {
				printf_s("ERROR: Invalid STATUS response: argc=%d from %s\n", argc, from);
			} else if (client->name != argv[1]) {
				printf_s("WARNING: STATUS response: unmatch name %s != %s\n", argv[1], client->name);
			} else {
				client->status = std::string(argv[2]);
			}

		} else if (strcmp(argv[0], "HIT") == 0) {

			variable on_hit = psl->get("on_hit");
			if (argc >= 3) {
				if (current_state != NULL) {
					on_hit(current_state->name().c_str(), argv[1], argv[2]);
				}
				sprintf_s(message_buf, "HIT %s", argv[2]);
				shout(message_buf);
			}

		}

	} else {

		// スクリプトに振る
		//on_message_in_psl(current_state->name().c_str(), from.c_str(), argv[0]);

	}

}	

/***************************************************************
	サウンド関連
***************************************************************/
void testApp::setup_sounds()
{
	// もしすでに読み込まれているものがあればすべて破棄する。
	map<std::string, STSoundPlayer*>::iterator it = sounds.begin();
	while (it != sounds.end()) {
		delete it->second;
		sounds.erase(it++);
	}


	// スクリプトに書かれたサウンドファイルを読み込む。
	variable v_sound_dir = psl->get("sound_dir");
	std::string sound_dir = v_sound_dir;
	variable sound_table = psl->get("sounds");
	variable keys = sound_table.keys();
	int len = keys.length();
	for (int i=0; i<len; i++) {
		variable v_name = keys[i];
		variable v_filename = sound_table[v_name];
		std::string name = v_name;
		std::string filename = v_filename;
		filename = sound_dir + "/" + filename;
		printf_s("Load sound file %s as '%s'\n", filename.c_str(), name.c_str());
		STSoundPlayer *s = new STSoundPlayer();
		if (s->loadSound(filename)) {
			s->setMultiPlay(true);
			sounds[name] = s;
		} else {
			delete s;
		}
	}
}

void testApp::update_sounds()
{
	map<std::string, STSoundPlayer*>::iterator it = sounds.begin();
	while (it != sounds.end()) {
		if (it->second->fade_step > 0) it->second->update_fade();
		it++;
	}
}
