#ifndef _ST_ERROR_STATE_H_
#define _ST_ERROR_STATE_H_

#include "State.h"

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

class ErrorState : public State {

public:
	ErrorState(testApp *app);

	// ��Ԃɓ���Ƃ��ɌĂ΂��B
	virtual void enterState();
	// ��Ԃɂ���ԁA�J��Ԃ��Ă΂��B��Ԃ��ڂ�ꍇ�͈ڂ���Ԃ��B�ێ��͎��g�܂���NULL��
	virtual State* updateState();
	// ��Ԃ��o��Ƃ��ɌĂ΂��B
	virtual void leaveState();

};

#endif

