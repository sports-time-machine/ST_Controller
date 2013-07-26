#ifndef _TEST_APP_H_
#define _TEST_APP_H_

#pragma warning(disable:4996)//unsafe function
#include "PSL/PSL.h"
typedef PSL::variable PSLv;

#include "ofMain.h"
#include "ofxUDPManager.h"
#include "UDPReceiverThread.h"
#include "STOperatorInterface.h"

#include "STClient.h"

class State;
class GameRecord;
class STSoundPlayer;

class testApp : public ofBaseApp{

public:

	PSL::PSLVM *psl;
	void init_psl();

	typedef map<std::string, STClient*> Clients;
	typedef map<std::string, STSoundPlayer*> Sounds;
	typedef std::vector<STSoundPlayer *> FadingSounds;


	Clients       clients;
	Sounds        sounds;
	FadingSounds  fading_sounds;


	struct Msg
	{
		volatile bool request;
		volatile bool done;
		volatile bool _lock;
		std::string address;
		std::vector<std::string> received_message;

		void lock()
		{
			while (_lock)
			{
				Sleep(1);
			}
			_lock = true;
		}

		void unlock()
		{
			_lock = false;
		}

		Msg()
		{
			request = false;
			done = false;
			_lock = false;
		}
	};
	Msg msg;

	// GameInfoはUIが生成し、
	// loadGameInfoを通してtestAppに渡される。
	// saveGameInfoの呼び出しでDBに保存。
	// そのセッションが終わったらtestAppが解放すること。
	GameRecord *current_game;

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

	void exit();

	// スクリプトからも呼べる関数
	static int change_state(const char * state_name);
	static int play_sound_vol(const char * name, float vol);
	static int play_sound(const char * name, int loop=0, float vol=1.0, float pan=0);
	static int fadein_sound(const char *name, float target_volume, int frames, int loop=1);
	static int fadeout_sound(const char *name, int frames);
	static int fade_sound(const char *name, float target_volume, int frames, float initial_volume, int autostop=1, int loop=0);
	static int stop_sound(const char * name);
	static int shout(const char * message);
	static int send_frame_clock(bool onoff, bool autostop=false);
	static void api_blackout(int frames);
	static void enable_frame_signal();
	static void disable_frame_signal();
	// autostopはリプレイ用。記録されたフレーム数で自動的に止まる。

	// すべての機器が接続したかどうか。
	bool allPearConnected() const;
	// すべてのクライアントがREADYになったかどうか。
	bool allClientIs(const string& status) const;


	STClient* getClient(std::string address);
	// IDENTのときにUIから呼ばれる。
	void loadGameRecord(GameRecord *record);
	// saveの時にSavingStateから呼ばれる。return false if failed.
	bool saveGameRecord();
	// saveステートを出るときにSavingStateから呼ばれる。
	void releaseGameRecord();
	/*
		app->loadGameInfo(record);
		sprintf(buf, "IDENT %s %s", argv[1], argv[2]);
		app->change_state("loading");
		app->shout(buf);
		*/

	void dispatchMessage(char *argv[], int argc, std::string from);

	// パブリックメンバ変数
	int               idle_frame;

private:
	struct FrameSig
	{
		bool enabled;
		int  index;
		FrameSig()
		{
			enabled = false;
			index = 0;
		}
	};

	STOperatorInterface *opInterface;
	ofxUDPManager     sender;
	UDPReceiverThread receiver_t;
	char              sendbuf[UDP_PACKET_BUF_SIZE];
	State*            current_state;
	State*            next_state;
	FrameSig          frame_sig;

	void setup_sounds();
	void update_sounds();
};

#endif
