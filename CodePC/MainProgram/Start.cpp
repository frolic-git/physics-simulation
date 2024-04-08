#include "Game.h"
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#else
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-graphics.lib")
#endif
int main()
{
	GameState *current = new Game();
	State currentState = State::NO_CHANGE;

	//current = new Game();
	//currentState = State::NO_CHANGE;

	while (currentState != State::EXIT)
	{
		current->handleEvents();
		currentState = current->update();
		
		if (currentState != State::NO_CHANGE)
		{
			
			switch (currentState)
			{
			case State::PLAY:
				delete current;
				current = new Game();
				break;
			}
		}
		current->render();
	}

	delete current;

	return 0;
}