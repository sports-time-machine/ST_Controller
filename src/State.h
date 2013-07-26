#ifndef _ST_STATE_H_
#define _ST_STATE_H_

#include "ofMain.h"

class testApp;

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

class State {

	static map<std::string, State*> state_table;

protected:
	std::string state_name;
	testApp *app;
	long frameNum;		// enterState()で０になり、updateState() の中でインクリメントされる。

public:
	State(const std::string state_name, testApp *app);

	long getFrameNum() const { return frameNum; }
	static State* byName(const std::string state_name);

	// 状態に入るときに呼ばれる。
	virtual void enterState();
	// 状態の間、繰り返し呼ばれる。次の状態を返す。維持は自身。NULLは状態なしに遷移。
	virtual State* updateState();
	// 状態を出るときに呼ばれる。
	virtual void leaveState();

	std::string &name();

};

#endif

