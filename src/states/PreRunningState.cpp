
#include "PreRunningState.h"
#include "testApp.h"

PreRunningState::PreRunningState(testApp *app):
	State("prerunning", app)
{
}

void PreRunningState::enterState()
{
	State::enterState();
}

State* PreRunningState::updateState()
{
	State::updateState();

	return this;
}

void PreRunningState::leaveState()
{
	State::leaveState();
}
