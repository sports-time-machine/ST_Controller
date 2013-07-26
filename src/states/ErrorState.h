#ifndef _ST_ERROR_STATE_H_
#define _ST_ERROR_STATE_H_

#include "State.h"

/*
	STC_0_INITED = 0,			// 起動直後
	STC_1_READY,				// 全クライアントと接続
	STC_2_LOADING,				// クライアントがデータロード中
	STC_3_WAITING_RUN,			// 走行準備完了
	STC_4_RUNNING,				// 走行中（記録中）
	STC_5_WAITING_PLAYBACK,		// 記録終了（再生待ち）
	STC_6_PLAYING_BACK,			// 再生中
	STC_7_SAVING,				// データセーブ中
	STC_ERROR
*/

class ErrorState : public State {

public:
	ErrorState(testApp *app);

	// 状態に入るときに呼ばれる。
	virtual void enterState();
	// 状態にいる間、繰り返し呼ばれる。状態を移る場合は移り先を返す。維持は自身またはNULLｓ
	virtual State* updateState();
	// 状態を出るときに呼ばれる。
	virtual void leaveState();

};

#endif

