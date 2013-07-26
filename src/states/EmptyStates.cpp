
#include "EmptyStates.h"
#include "testApp.h"

#define EMPTY_STATE_CLASS(CL,internal_name) \
	CL::CL(testApp *app):State(internal_name, app)  { }\
	void   CL::enterState()    { State::enterState(); }\
	State* CL::updateState()   { State::updateState(); return this; }\
	void   CL::leaveState()    { State::leaveState(); }

EMPTY_STATE_CLASS(PlayingBackState,  "playing_back")
EMPTY_STATE_CLASS(PlaybackGoalState, "playback_goal")
EMPTY_STATE_CLASS(SaveEndState,      "save_end")
