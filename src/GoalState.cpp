#include "GoalState.h"
#include "testApp.h"

GoalState::GoalState(testApp *app):
	State("goal", app)
	{
	}

void GoalState::enterState()
{
	State::enterState();
}

State* GoalState::updateState()
{
	State::updateState();
	return this;
}

void GoalState::leaveState()
{
	State::leaveState();
}

