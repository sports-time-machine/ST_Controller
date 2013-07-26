
#include "LoadingState.h"
#include "testApp.h"

LoadingState::LoadingState(testApp *app):
	State("loading", app)
	{
	}

void LoadingState::enterState()
{
	State::enterState();
}

State* LoadingState::updateState()
{
	State::updateState();

	//if (frameNum > 30 && frameNum % 30 == 0) {
	if (frameNum > 2 && frameNum % 10 == 0) {
		if (app->allClientIs("READY")) {
			return State::byName("waiting_run");
		}
	}

	return this;
}

void LoadingState::leaveState()
{
	State::leaveState();
}

