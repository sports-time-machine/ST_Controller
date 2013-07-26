
#include "ErrorState.h"
#include "testApp.h"

ErrorState::ErrorState(testApp *app):
	State("error", app)
	{
	}

void ErrorState::enterState()
{
	State::enterState();
}

State* ErrorState::updateState()
{
	State::updateState();
	return this;
}

void ErrorState::leaveState()
{
	State::leaveState();
}

