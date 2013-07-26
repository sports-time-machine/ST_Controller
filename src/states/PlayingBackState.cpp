
#include "PlayingBackState.h"
#include "testApp.h"

PlayingBackState::PlayingBackState(testApp *app):
	State("playing_back", app)
	{
	}

void PlayingBackState::enterState()
{
	State::enterState();
	app->send_frame_clock(true, true);
	app->shout("REPLAY");
}

State* PlayingBackState::updateState()
{
	State::updateState();

	if (frameNum > 30 && frameNum % 30 == 0) {
		if (app->allClientsReady()) {
			return State::byName("save");
		}
	}

	return this;
}

void PlayingBackState::leaveState()
{
	app->send_frame_clock(false);
	State::leaveState();
}

