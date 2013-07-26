
#include "testApp.h"
#include "CalibratingState.h"

using namespace PSL;

CalibratingState::CalibratingState(testApp *app):
	State("calibrating", app)
	{
	}

void CalibratingState::enterState()
{
	State::enterState();
}

State* CalibratingState::updateState()
{
	State::updateState();
	return this;
}

void CalibratingState::leaveState()
{
	State::leaveState();
}

