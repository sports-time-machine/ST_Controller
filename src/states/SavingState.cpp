
#include "SavingState.h"
#include "testApp.h"

SavingState::SavingState(testApp *app):
	State("saving", app)
	{
	}

void SavingState::enterState()
{
	State::enterState();
	app->shout("SAVE");
	app->saveGameRecord();
}

State* SavingState::updateState()
{
	State::updateState();

	if (frameNum > 30 && frameNum % 30 == 0) {
		if (app->allClientIs("IDLE")) {
			return State::byName("save_end");
		}
	}
	return this;
}

void SavingState::leaveState()
{
	// セーブが完了したらINITを送る。
	app->shout("INIT");
	app->releaseGameRecord();
	State::leaveState();
}

