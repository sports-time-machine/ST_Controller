
#include "WaitingRunState.h"
#include "testApp.h"

WaitingRunState::WaitingRunState(testApp *app):
	State("waiting_run", app)
	{
	}

void WaitingRunState::enterState()
{
	State::enterState();
}

State* WaitingRunState::updateState()
{
	State::updateState();
	return this;
}

void WaitingRunState::leaveState()
{
	State::leaveState();
}

