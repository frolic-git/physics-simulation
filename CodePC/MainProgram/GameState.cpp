#include "GameState.h"

GameState::GameState(std::string title)
	:window(sf::VideoMode(300, 150), title)
{
}

GameState::~GameState()
{
}

State GameState::setNoState()
{
	return State::NO_CHANGE;
}
