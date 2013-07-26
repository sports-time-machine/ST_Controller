
#include "STSoundPlayer.h"

STSoundPlayer::STSoundPlayer()
{
	max_volume = 1.0;
	fade_step = 0;
}

void STSoundPlayer::play()
{
	if (getIsPlaying()) {
		stop();
		printf("play! but PLAYING now\n");
	}
	ofSoundPlayer::play();	
}

void STSoundPlayer::start_fade(float target_vol, int frame, float initial_vol, bool autostop)
{
	target_volume = target_vol;
	fade_step = frame;
	if (initial_vol >= 0) {
		current_volume = initial_vol;
		setVolume(current_volume * max_volume);
		printf("set volume %f\n", current_volume * max_volume);
	}
	autostop_on_volume = autostop;
}

bool STSoundPlayer::update_fade()
{
	if (fade_step > 0) {

		float diff = target_volume - current_volume;
		current_volume += diff / fade_step;
		setVolume(current_volume * max_volume);
		if (autostop_on_volume && current_volume == 0)
		{
	//		printf("fade_step is %d\n", fade_step);
	//		puts("FADE AUTO STOP!!!!!!!!!!!!!!!!!!");
			stop();
		}
		//printf("====> fade %d:%5.3f\n", fade_step, current_volume * max_volume);
		fade_step--;
	}

	return (fade_step <= 0);
}

void STSoundPlayer::play_loop()
{
	if (getIsPlaying()) stop();
	setLoop(true);
	setMultiPlay(false);
	play();
}
