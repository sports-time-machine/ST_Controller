
#ifndef ST_C_SOUND_H_
#define ST_C_SOUND_H_

#include "ofSoundPlayer.h"

class STSoundPlayer : public ofSoundPlayer {

public:

	float max_volume;		// �W���̍ő剹��(0.0 - 1.0)
	float current_volume;	// ���݂̉���(0.0 - 1.0 as max_volume)
	float target_volume;	// �t�F�[�h�̍s����(0.0 - 1.0 as max_volume)
	int fade_step;			// ���Ɖ��t���[����target_volume�ɓ��B��������
	bool autostop_on_volume;	// �t�F�[�h����ŉ��ʂ��O�ɂȂ����Ƃ��Ɏ����I�ɍĐ���~���邩�B

	float pan;

	STSoundPlayer();

	void play();

	// �t�F�[�h����B
	void start_fade(float target_vol, int frame, float initial_volume=-1, bool autostop=true);
	bool update_fade();	// ccurrent_volume��target_volume�ɂȂ�����true;

	void play_loop();

};

#endif

