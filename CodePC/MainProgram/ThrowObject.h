#pragma once
#include <SFML/Graphics.hpp>

class ThrowableObject : public sf::Drawable
{
private:
	sf::CircleShape objectHead;
	float weightOfHead;
	float airResistanceCoefficent;
	float radius;
public:
	ThrowableObject(float x, float y, float radius, float weightOfHead);
	~ThrowableObject();

	float getCoefficent();
	float getRadius();
	float getWeight();
	float getCrossSection();
	sf::Vector2f getPos();
	void setPos(float x, float y);
	void setPos(sf::Vector2f velocity);

	void move(float x, float y);
	void move(sf::Vector2f movement);
	void spin(float radian);

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};