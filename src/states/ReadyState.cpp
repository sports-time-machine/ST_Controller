
#include "ReadyState.h"
#include "testApp.h"

ReadyState::ReadyState(testApp *app):
	State("ready", app)
	{
	}

void ReadyState::enterState()
{
	app->shout("INIT");
	app->releaseGameRecord();
	State::enterState();
}

State* ReadyState::updateState()
{
	State::updateState();

	const int FRAMES_PER_SEC = 30;
	++app->idle_frame;
	if (app->idle_frame%(30*FRAMES_PER_SEC)==0)
	{
		app->idle_frame = 0;
		static int idle_select = 0;
		++idle_select;
		static char buffer[1024];
		sprintf_s(buffer, "IDLE-SELECT %d", idle_select);
		app->shout(buffer);
	}

	return this;
}

void ReadyState::leaveState()
{
	State::leaveState();
}

