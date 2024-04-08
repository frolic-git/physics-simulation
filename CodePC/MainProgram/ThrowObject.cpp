#include "ThrowObject.h"
ThrowableObject::ThrowableObject(float x, float y, float radius, float weightOfHead)
{
	objectHead.setRadius(radius*50.f);
	objectHead.setFillColor(sf::Color::Red);
	setPos(x, y);

	// The air resistance coefficent of a sphere is 0.47
	this->airResistanceCoefficent = 0.47f;
	this->weightOfHead = weightOfHead;
	this->radius = radius;
}
ThrowableObject::~ThrowableObject()
{
}

float ThrowableObject::getCoefficent()
{
	return airResistanceCoefficent;
}

float ThrowableObject::getRadius()
{
	return objectHead.getRadius();
}

float ThrowableObject::getWeight()
{
	return weightOfHead;
}

float ThrowableObject::getCrossSection()
{
	// Magic number is Pi
	// Equation is Pi times the radius to the power of 2
	return 3.14159265358979323846 * pow(radius, 2);
}

sf::Vector2f ThrowableObject::getPos()
{
	return objectHead.getPosition();
}

void ThrowableObject::setPos(float x, float y)
{
	objectHead.setPosition(x, y);
}

void ThrowableObject::setPos(sf::Vector2f velocity)
{
	objectHead.setPosition(velocity);
}

void ThrowableObject::move(float x, float y)
{
	sf::Vector2f movementVector = sf::Vector2f(x, y);

	objectHead.move(movementVector);
}

void ThrowableObject::move(sf::Vector2f movement)
{
	objectHead.move(movement);
}

void ThrowableObject::spin(float radian)
{
	objectHead.rotate(radian);
}

void ThrowableObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(objectHead);
}
