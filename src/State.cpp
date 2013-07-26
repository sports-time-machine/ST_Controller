
#include "State.h"
#include "testApp.h"

using namespace PSL;

map<std::string, State*> State::state_table = map<std::string, State*>();

State::State(const std::string _state_name, testApp *_app) {
	state_name = _state_name;
	app = _app;
	// ó‘Ôˆê——‚É©g‚ğ“o˜^
	state_table[state_name] = this;
}

State* State::byName(const std::string state_name)
{
	map<std::string, State*>::iterator it = state_table.find(state_name);
	if (it == state_table.end())
	{
		printf("byName %s‚ª¸”s‚µ‚Ü‚µ‚½BABORT‚Å‚·B", state_name.c_str());
		return NULL;
	}
	else
	{
		return it->second;
	}
}

void State::enterState()
{
	printf_s("STATE: enter %s\n", state_name.c_str());
	variable on_enter_state = app->psl->get("on_enter_state");
	on_enter_state(state_name.c_str());
	frameNum = 0;
}

State* State::updateState()
{
	variable on_update_state = app->psl->get("on_update_state");
	on_update_state(state_name.c_str(), frameNum++);
	return this;
}

void State::leaveState()
{
	variable on_leave_state = app->psl->get("on_leave_state");
	on_leave_state(state_name.c_str());
	printf_s("STATE: leave %s\n", state_name.c_str());
}

std::string &State::name() {
	return state_name;
}
