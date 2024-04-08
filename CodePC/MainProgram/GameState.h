#pragma once
#include <SFML\Graphics.hpp>
#include "State.h"
#include <string>

class GameState
{
protected: 
	sf::RenderWindow window;
public:
	GameState(std::string title);
	virtual ~GameState();
	State setNoState();
	virtual State update() = 0;
	virtual void render() = 0;
	virtual void handleEvents() = 0;
};
