#pragma once
#include "State.h"

class GoalState : public State {

public:
	GoalState(testApp *app);
	virtual void enterState();
	virtual State* updateState();
	virtual void leaveState();
};
