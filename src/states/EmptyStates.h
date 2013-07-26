#pragma once
#include "State.h"

#define EMPTY_STATE_CLASS_H(CL) \
	class CL : public State {\
	public:\
		CL(testApp *app);\
		virtual void enterState();\
		virtual State* updateState();\
		virtual void leaveState();\
	};

EMPTY_STATE_CLASS_H(PlayingBackState)
EMPTY_STATE_CLASS_H(PlaybackGoalState)
EMPTY_STATE_CLASS_H(SaveEndState)
