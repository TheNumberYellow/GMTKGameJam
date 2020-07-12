#pragma once
#include "SFML/Graphics.hpp"

class Player;
class Bullet;

class Enemy : public sf::Drawable
{
private:
	

	sf::Texture enemyTexture;
	sf::Texture enemyBulletTexture;

	sf::Sprite enemySprite;

	std::vector<Bullet> bullets;

	float shootPeriod = 1.5f;
	float shootTimer  = shootPeriod;

public:
	Enemy();
	void Update(float deltaT, Player& player);
	void SetPosition(sf::Vector2f newPosition);

protected:

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

