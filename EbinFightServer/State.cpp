#include "State.h"

State::State(Server& server, std::stack<State*>& currentState) : p_server(server), p_currentState(currentState)
{
	isExitState = false;
}
