#pragma once
#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>   
#include <stdlib.h>    
#include <time.h>  
#include <iostream>
#include <fstream>
#include "GameState.h"
#include "Thrower.h"
#include "ThrowObject.h"


//#define M_PI 3.14159265358979323846

class Game : public GameState
{
private:
	float velocityX;
	float velocityY;
	float accelerationX;
	float accelerationY;
	float coefficent;
	float crossSection;

	Thrower thrower;
	ThrowableObject throwObject;

	float gravity;
	float airDensity;
	bool angleSet;
	bool powerSet;
	bool incrementAngle;
	bool incrementPower;
	float angle;
	float power;
	float angleDecided;
	float powerDecided;

	bool firstRun;
	bool secondThrow;

	sf::RectangleShape anglePowerIndicator;
	sf::RectangleShape ground;
	std::vector<sf::CircleShape*> path;
	sf::Text text;
	sf::Font font;

	std::vector<std::string*> coordinates;
	bool ballTouchedGround;
	bool dataWrittenDown;
	std::ofstream writer;

	sf::Clock timeSinceThrow;
	sf::Clock clock;
	sf::Time timePerFrame;
	sf::Time elapsedTimeSinceLastUpdate;
public:
	Game();
	virtual ~Game();
	// Inherited via GameState
	virtual State update() override;

	virtual void render() override;

	virtual void handleEvents() override;

};
