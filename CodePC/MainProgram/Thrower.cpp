#include "Thrower.h"

Thrower::Thrower(float x, float y, float width, float height)
{
	sprite.setPosition(x, y);
	sprite.setSize(sf::Vector2f(width, height));
	sprite.setFillColor(sf::Color::Blue);
}

Thrower::~Thrower()
{
}

sf::Vector2f Thrower::getPos()
{
	return sprite.getPosition();;
}

void Thrower::setPos(float x, float y)
{
	sprite.setPosition(x, y);
}

float Thrower::decideAngle()
{
	float returnVal = -1.0f;
	float angle = 0.0f;
	bool keyPressed = false;
	bool increment = true;

	if (increment)
		angle++;
	else if (!increment)
		angle--;

	if (angle <= 90.0f)
		increment = true;
	else if (angle > 90.0f)
		increment = false;

	if (!keyPressed) {

	}
	return angle;
}

float Thrower::decidePower()
{
	float power = 0.0f;
	return power;
}

void Thrower::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
}
