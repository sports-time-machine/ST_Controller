
#include "testApp.h"
#include "InitedState.h"

using namespace PSL;

InitedState::InitedState(testApp *app):
	State("inited", app)
	{
	}

void InitedState::enterState()
{
	char message[1024];
	State::enterState();

	variable str1 = app->psl->get("server_address");
	variable str2 = app->psl->get("server_port");
	int port = str2;
	sprintf_s(message, "PING %s", str1.c_str());
	app->shout(message);
	sprintf_s(message, "UI_PING %s %d", str1.c_str(), port);
	app->shout(message);
	frame_counter = 0;
}

State* InitedState::updateState()
{
	State::updateState();

	if (app->allPearConnected())
		return State::byName("ready");

	frame_counter++;
	const int FRAMES_PER_SEC = 30;
	if (frame_counter > 3 * FRAMES_PER_SEC) {
		// N•b–ˆ‚ÉPING‚ğ‘—‚éB
		enterState();
		frame_counter = 0;
	}
	
	return this;
}

void InitedState::leaveState()
{
	State::leaveState();
}

