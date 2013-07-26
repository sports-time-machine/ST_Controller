
#include "RunningState.h"
#include "testApp.h"

RunningState::RunningState(testApp *app):
	State("running", app)
{
}

void RunningState::enterState()
{
	State::enterState();
}

State* RunningState::updateState()
{
	State::updateState();
	return this;
}

void RunningState::leaveState()
{
	State::leaveState();
}

