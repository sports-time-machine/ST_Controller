#pragma once

#include "State.h"

class PreRunningState : public State {

public:
	PreRunningState(testApp *app);

	virtual void enterState();
	virtual State* updateState();
	virtual void leaveState();

};
