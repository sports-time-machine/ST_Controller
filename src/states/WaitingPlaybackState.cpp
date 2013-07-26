
#include "WaitingPlaybackState.h"
#include "testApp.h"

WaitingPlaybackState::WaitingPlaybackState(testApp *app):
	State("waiting_playback", app)
	{
	}

void WaitingPlaybackState::enterState()
{
	State::enterState();
}

State* WaitingPlaybackState::updateState()
{
	State::updateState();

	return this;
}

void WaitingPlaybackState::leaveState()
{
	State::leaveState();
}

