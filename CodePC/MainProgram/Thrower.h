#pragma once
#include <SFML\Graphics.hpp>

class Thrower : public sf::Drawable
{
private:
	sf::RectangleShape sprite;
public:
	Thrower(float x, float y, float width, float height);
	~Thrower();

	sf::Vector2f getPos();
	void setPos(float x, float y);
	float decideAngle();
	float decidePower();


	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};