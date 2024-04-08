#include "Game.h"
#include <iostream>
#include "State.h"

Game::Game()
	:GameState("Game"), thrower(20, 120, 10, 30), throwObject(3.f, window.getSize().y-7.f, 0.07f, 0.2f)
{
	elapsedTimeSinceLastUpdate = sf::Time::Zero;
	timePerFrame = sf::seconds(1 / 60.f);

	font.loadFromFile("C:/Windows/Fonts/arial.ttf");
	text.setFont(font);
	text.setCharacterSize(20);
	text.setPosition(720, 360);
	text.setFillColor(sf::Color::White);

	anglePowerIndicator.setSize(sf::Vector2f(25.0f, 2.0f));
	anglePowerIndicator.setFillColor(sf::Color::Cyan);
	anglePowerIndicator.setPosition(throwObject.getPos().x+6.f, 150 - anglePowerIndicator.getSize().y);
	
	ground.setSize(sf::Vector2f(window.getSize().y*2,2.f));
	ground.setFillColor(sf::Color::White);
	ground.setPosition(1.f, window.getSize().y - 1);
	
	coefficent = throwObject.getCoefficent();
	crossSection = throwObject.getCrossSection();
	gravity = 9.82f;
	angle = 0.0f;
	power = 10.0f;
	angleDecided = 0.0f;
	powerDecided = 0.0f;

	angleSet = false;
	powerSet = false;
	incrementAngle = true;
	incrementPower = true;
	firstRun = true;
	secondThrow = false;
	ballTouchedGround = false;
	dataWrittenDown = false;

	srand(time(NULL));
	float toModulo = 1.4224f - 1.1455f;
	airDensity = fmod(rand(), toModulo) + 1.1455f;
	text.setString(std::to_string(airDensity));


	window.setFramerateLimit(30);
}

Game::~Game()
{
	for (int i = 0; i < path.size(); i++)
	{
		delete path.at(i);
		path.at(i) = 0;
	}
	for (int i = 0; i < coordinates.size(); i++)
	{
		delete coordinates.at(i);
		coordinates.at(i) = 0;
	}
}

State Game::update()
{
	
	elapsedTimeSinceLastUpdate += clock.restart();
	
	while (elapsedTimeSinceLastUpdate > timePerFrame)
	{
		elapsedTimeSinceLastUpdate -= timePerFrame;

		/* Angle Incrementer */
		if (angleSet == false)
		{
			// If our bool is true we increment it otherwise decrement
			if (incrementAngle)
				angle++;
			else if (!incrementAngle)
				angle--;

			// Change rotation of angle indicator
			anglePowerIndicator.setRotation(angle + 270);

			// Switch bool if value of angle goes out of range
			if (angle <= 0.0f)
				incrementAngle = true;
			else if (angle >= 90.0f)
				incrementAngle = false;
		}
		/* Power Incrementer */
		else if (powerSet == false)
		{
			// If our bool is true we increment it otherwise decrement
			if (incrementPower)
				power += 0.1f;
			else if (!incrementPower)
				power -= 0.1f;

			// Change size of power indicator
			anglePowerIndicator.setSize(sf::Vector2f((power - 10.f) * 2.5f, anglePowerIndicator.getSize().y));

			// Switch bool if value of power goes out of range
			if (power <= 10.0f)
				incrementPower = true;
			else if (power >= 30.0f)
				incrementPower = false;
		}
		else
		{
			if (firstRun)
			{
				// The X and Y velocity are calculated with Velocity times the angle, using
				// cos for X and
				// sin for Y, multiply Y with -1 to accomodate for SFML's Y axis (its reveresed)
				velocityX = power * cos(angle * (M_PI / 180));
				velocityY = (power * sin(angle * (M_PI / 180))) * (-1.0f);

				accelerationX = 0.0f;
				// Again, Y is positive as SFML has reversed Y
				accelerationY = 9.82f;

				float timeSinceLastFrame = elapsedTimeSinceLastUpdate.asSeconds();
				velocityX = velocityX + (accelerationX * timeSinceLastFrame);
				velocityY = velocityY + (accelerationY * timeSinceLastFrame);

				firstRun = false;
			}
			else
			{
				float velocitySquared = (pow(velocityX, 2) + pow(velocityY, 2));
				float dragSize = (coefficent * (-1.0f)) * ((airDensity * velocitySquared * crossSection) * 0.5f);

				angle = atan(velocityY / velocityX);

				accelerationX = (dragSize * cos(angle)) / throwObject.getWeight();
				// We add 9.82 instead of subtracting to accomodate for SFML's Y axis
				accelerationY = (dragSize * sin(angle)) / throwObject.getWeight() + gravity;

				float timeSinceLastFrame = elapsedTimeSinceLastUpdate.asSeconds();
				velocityX = velocityX + (accelerationX * timeSinceLastFrame);
				velocityY = velocityY + (accelerationY * timeSinceLastFrame);

				float posX = throwObject.getPos().x + (velocityX * timeSinceLastFrame);
				float posY = throwObject.getPos().y + (velocityY * timeSinceLastFrame);

				throwObject.setPos(sf::Vector2f(posX, posY));

				if ((int)throwObject.getPos().x % 1 == 0)
				{
					sf::CircleShape* circle = new sf::CircleShape;
					circle->setRadius(1.f);
					circle->setFillColor(sf::Color::Green);
					circle->setPosition(sf::Vector2f(posX + 2.f, posY + 2.f));
					path.push_back(circle);

					if (circle->getGlobalBounds().intersects(ground.getGlobalBounds()))
						ballTouchedGround = true;

					if (!ballTouchedGround)
					{
						std::string *coordHolder = new std::string[2];
						coordHolder[0] = std::to_string(throwObject.getPos().x);
						coordHolder[1] = std::to_string(throwObject.getPos().y);
						coordinates.push_back(coordHolder);
					}
					else
					{
						if (!dataWrittenDown)
						{
							writer.open("xCoord.txt");
							for (std::string* string : coordinates)
								writer << string[0] << std::endl;
							writer.close();

							writer.open("yCoord.txt");
							for (std::string* string : coordinates)
								writer << string[1] << std::endl;
							writer.close();

							dataWrittenDown = true;
						}
					}
				}
			}

			// Runs two times one time with wind resistane and one time without
			/*if (!secondThrow)
			{
				if (firstRun)
				{
					// The X and Y velocity are calculated with Velocity times the angle, using
					// cos for X and
					// sin for Y, multiply Y with -1 to accomodate for SFML's Y axis (its reveresed)
					velocityX = power * cos(angle * (M_PI / 180));
					velocityY = (power * sin(angle * (M_PI / 180))) * (-1.0f);

					accelerationX = 0.0f;
					// Again, Y is positive as SFML has reversed Y
					accelerationY = 9.82f;

					float timeSinceLastFrame = elapsedTimeSinceLastUpdate.asSeconds();
					velocityX = velocityX + (accelerationX * timeSinceLastFrame);
					velocityY = velocityY + (accelerationY * timeSinceLastFrame);

					firstRun = false;
				}
				else
				{
					float velocitySquared = (pow(velocityX, 2) + pow(velocityY, 2));
					float dragSize = (coefficent * (-1.0f)) * ((airDensity * velocitySquared * crossSection) * 0.5f);

					angle = atan(velocityY / velocityX);

					accelerationX = (dragSize * cos(angle)) / throwObject.getWeight();
					// We add 9.82 instead of subtracting to accomodate for SFML's Y axis
					accelerationY = (dragSize * sin(angle)) / throwObject.getWeight() + gravity;

					float timeSinceLastFrame = elapsedTimeSinceLastUpdate.asSeconds();
					velocityX = velocityX + (accelerationX * timeSinceLastFrame);
					velocityY = velocityY + (accelerationY * timeSinceLastFrame);

					float posX = throwObject.getPos().x + (velocityX * timeSinceLastFrame);
					float posY = throwObject.getPos().y + (velocityY * timeSinceLastFrame);

					throwObject.setPos(sf::Vector2f(posX, posY));

					if ((int)throwObject.getPos().x % 1 == 0)
					{
						sf::CircleShape* circle = new sf::CircleShape;
						circle->setRadius(1.f);
						circle->setFillColor(sf::Color::Green);
						circle->setPosition(sf::Vector2f(posX + 2.f, posY + 2.f));
						path.push_back(circle);


						if (circle->getGlobalBounds().intersects(ground.getGlobalBounds()))
						{
							ballTouchedGround = true;
							firstRun = true;
							secondThrow = true;
						}

						if (!ballTouchedGround)
						{
							std::string* coordHolder = new std::string[2];
							coordHolder[0] = std::to_string(throwObject.getPos().x);
							coordHolder[1] = std::to_string(throwObject.getPos().y);
							coordinates.push_back(coordHolder);
						}
						else
						{
							if (!dataWrittenDown)
							{
								writer.open("xCoord.txt");
								for (std::string* string : coordinates)
									writer << string[0] << std::endl;
								writer.close();

								writer.open("yCoord.txt");
								for (std::string* string : coordinates)
									writer << string[1] << std::endl;
								writer.close();

								ballTouchedGround = false;
								coordinates.clear();
							}
						}
					}
				}
			}
			else
			{
				if (firstRun)
				{
					power = 19.f;
					angle = 45.f;
					airDensity = 1.252706f;

					throwObject.setPos(3.f, window.getSize().y - 7.f);

					// The X and Y velocity are calculated with Velocity times the angle, using
					// cos for X and
					// sin for Y, multiply Y with -1 to accomodate for SFML's Y axis (its reveresed)
					velocityX = power * cos(angle * (M_PI / 180));
					velocityY = (power * sin(angle * (M_PI / 180))) * (-1.0f);

					firstRun = false;
				}

				accelerationX = velocityX / throwObject.getWeight();
				accelerationY = velocityY / throwObject.getWeight() + 9.82f;

				float timeSinceLastFrame = elapsedTimeSinceLastUpdate.asSeconds();
				velocityX = velocityX;// (accelerationX * timeSinceLastFrame);
				velocityY = velocityY + 9.82f * timeSinceLastFrame;//(accelerationY * timeSinceLastFrame);

				float posX = throwObject.getPos().x + (velocityX * timeSinceLastFrame);
				float posY = throwObject.getPos().y + (velocityY * timeSinceLastFrame);

				throwObject.setPos(sf::Vector2f(posX, posY));

				if ((int)throwObject.getPos().x % 1 == 0)
				{
					sf::CircleShape* circle = new sf::CircleShape;
					circle->setRadius(1.f);
					circle->setFillColor(sf::Color::Blue);
					circle->setPosition(sf::Vector2f(posX + 2.f, posY + 2.f));
					path.push_back(circle);


					if (circle->getGlobalBounds().intersects(ground.getGlobalBounds()))
						ballTouchedGround = true;

					if (!ballTouchedGround)
					{
						std::string* coordHolder = new std::string[2];
						coordHolder[0] = std::to_string(throwObject.getPos().x);
						coordHolder[1] = std::to_string(throwObject.getPos().y);
						coordinates.push_back(coordHolder);
					}
					else
					{
						if (!dataWrittenDown)
						{
							writer.open("xCoordSecond.txt");
							for (std::string* string : coordinates)
								writer << string[0] << std::endl;
							writer.close();

							writer.open("yCoordSecond.txt");
							for (std::string* string : coordinates)
								writer << string[1] << std::endl;
							writer.close();

							dataWrittenDown = true;
						}
					}
				}*/

				// -------------------------- //
			//}
		}
	}
	
	return setNoState();
}

void Game::render()
{
	window.clear();
	if (firstRun == true)
		window.draw(anglePowerIndicator);
	for (sf::CircleShape* i : path)
		window.draw(*i);
	window.draw(ground);
	//window.draw(thrower);
	window.draw(text);
	window.draw(throwObject);
	window.display();
}

void Game::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Space) 
			{
				if (angleSet == false)
					angleSet = true;
				else if (powerSet == false)
					powerSet = true;

			}
		}
	}
}






