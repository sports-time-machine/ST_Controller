#ifndef _ST_OPERATOR_INTERFACE_H_
#define _ST_OPERATOR_INTERFACE_H_

#include "ofMain.h"
#include "ofxUDPManager.h"
#include "GameRecord.h"

class testApp;

class STOperatorInterface {
	testApp *app;
	ofxUDPManager sender;

	GameRecord *record;

public:
	std::string address;
	int port;

	STOperatorInterface(testApp *app, std::string address, int port);

	// ���݂̏�Ԃ��ς�����Ƃ��ɃR���g���[������ʒm�����
	void state_changed(const std::string &state);

	// UI����󂯂郁�b�Z�[�W�̓����
	void message(char *argv[], int argc);

	void send_to_interface(const char *message);
};

#endif

