#ifndef _ST_STATE_H_
#define _ST_STATE_H_

#include "ofMain.h"

class testApp;

/*
	STC_0_INITED = 0,			// �N������
	STC_1_READY,				// �S�N���C�A���g�Ɛڑ�
	STC_2_LOADING,				// �N���C�A���g���f�[�^���[�h��
	STC_3_WAITING_RUN,			// ���s��������
	STC_4_RUNNING,				// ���s���i�L�^���j
	STC_5_WAITING_PLAYBACK,		// �L�^�I���i�Đ��҂��j
	STC_6_PLAYING_BACK,			// �Đ���
	STC_7_SAVING,				// �f�[�^�Z�[�u��
	STC_ERROR
*/

class State {

	static map<std::string, State*> state_table;

protected:
	std::string state_name;
	testApp *app;
	long frameNum;		// enterState()�łO�ɂȂ�AupdateState() �̒��ŃC���N�������g�����B

public:
	State(const std::string state_name, testApp *app);

	long getFrameNum() const { return frameNum; }
	static State* byName(const std::string state_name);

	// ��Ԃɓ���Ƃ��ɌĂ΂��B
	virtual void enterState();
	// ��Ԃ̊ԁA�J��Ԃ��Ă΂��B���̏�Ԃ�Ԃ��B�ێ��͎��g�BNULL�͏�ԂȂ��ɑJ�ځB
	virtual State* updateState();
	// ��Ԃ��o��Ƃ��ɌĂ΂��B
	virtual void leaveState();

	std::string &name();

};

#endif

