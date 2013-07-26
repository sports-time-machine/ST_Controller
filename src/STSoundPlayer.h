
#ifndef ST_C_SOUND_H_
#define ST_C_SOUND_H_

#include "ofSoundPlayer.h"

class STSoundPlayer : public ofSoundPlayer {

public:

	float max_volume;		// 標準の最大音量(0.0 - 1.0)
	float current_volume;	// 現在の音量(0.0 - 1.0 as max_volume)
	float target_volume;	// フェードの行き先(0.0 - 1.0 as max_volume)
	int fade_step;			// あと何フレームでtarget_volumeに到達したいか
	bool autostop_on_volume;	// フェード操作で音量が０になったときに自動的に再生停止するか。

	float pan;

	STSoundPlayer();

	void play();

	// フェードする。
	void start_fade(float target_vol, int frame, float initial_volume=-1, bool autostop=true);
	bool update_fade();	// ccurrent_volumeがtarget_volumeになったらtrue;

	void play_loop();

};

#endif

