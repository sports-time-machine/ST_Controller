
#include "STOperatorInterface.h"
#include "testApp.h"

using namespace PSL;

STOperatorInterface::STOperatorInterface(testApp *app, std::string address, int port) {
	this->app = app;
	this->address = address;
	this->port = port;
	char addr[1024];
	strcpy(addr, address.c_str());
	printf_s("instructor sender.Create() => %d\n", sender.Create());
	printf_s("instructor sender.Connect(%s, %d) => %d\n", addr, port, sender.Connect(addr, port));

	// 必ずあるようにしておく
	record = new GameRecord();
};

void STOperatorInterface::state_changed(const std::string &state) {
	// Stateの変更をUIに伝える。
	char buffer[1024];
	sprintf(buffer, "STATE %s", state.c_str());
	send_to_interface(buffer);
	printf_s("UI <- %s\n", buffer);

	// と同時に、状態がLoadingになったらGameRecordを新しくする。
	if (state == "loading") {
		if (record) delete record;
		record = new GameRecord;
	}
}

void STOperatorInterface::message(char *argv[], int argc) {

	char buf[1024];

	if (argc < 1) return;

	if (strcmp(argv[0], "IDENT") == 0 && argc >= 3) {
		if (argc >= 3) {
			record->setPlayerId(argv[1]);
			record->setGameId(argv[2]);
		}
		app->loadGameRecord(record);
		record = NULL;
		/*
		sprintf(buf, "IDENT %s %s", argv[1], argv[2]);
		app->change_state("loading");
		app->shout(buf);
		*/
	} else if (strcmp(argv[0], "PARTNER") == 0) {
		if (argc >= 2)
			record->addPartnerID(argv[1]);
	} else if (strcmp(argv[0], "BACKGROUND") == 0) {
		if (argc >= 2)
			record->setBackground(argv[1]);
	} else if (strcmp(argv[0], "COMMENT") == 0) {
		if (argc >= 2)
			record->setComment(argv[1]);
	} else if (strcmp(argv[0], "TAG") == 0) {
		if (argc >= 2)
			record->addTag(argv[1]);
	} else if (strcmp(argv[0], "COLOR") == 0) {
		if (argc >= 2)
			record->setColor(argv[1]);
	} else if (strcmp(argv[0], "STYLE") == 0) {
		if (argc >= 2)
			record->setStyle(argv[1]);

	} else if (strcmp(argv[0], "START") == 0) {
		app->change_state("prerunning");

	} else if (strcmp(argv[0], "STOP") == 0) {
		app->change_state("waiting_run");

	} else if (strcmp(argv[0], "GOAL") == 0) {
		app->change_state("goal");

	} else if (strcmp(argv[0], "INIT") == 0) {
		app->change_state("inited");

	} else if (strcmp(argv[0], "SHOUT") == 0) {
		switch (argc) {
		case 2:	sprintf(buf, "%s", argv[1]);	break;
		case 3:	sprintf(buf, "%s %s", argv[1], argv[2]);	break;
		case 4:	sprintf(buf, "%s %s %s", argv[1], argv[2], argv[3]);	break;
		case 5:	sprintf(buf, "%s %s %s %s", argv[1], argv[2], argv[3], argv[4]);	break;
		case 6:	sprintf(buf, "%s %s %s %s %s", argv[1], argv[2], argv[3], argv[4], argv[5]);	break;
		case 7:	sprintf(buf, "%s %s %s %s %s %s", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);	break;
		default:
			return;
		}

		app->shout(buf);

	} else if (strcmp(argv[0], "STATE") == 0) {

		if (argc >= 2)
			app->change_state(argv[1]);

	} else {

	}

	printf_s("ui message | %s\n", argv[0]);
};

void STOperatorInterface::send_to_interface(const char * message) {

	char message_buf[1024];
	//sprintf_s(message_buf, "%s", message);
	//sender.Send(message_buf, strlen(message_buf));
	sender.Send(message, strlen(message));

}

