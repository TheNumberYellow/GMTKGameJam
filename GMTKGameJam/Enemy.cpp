#include "Enemy.hpp"

#include "Player.hpp"
#include "MathUtils.hpp"

Enemy::Enemy()
{
	enemyTexture.loadFromFile("images/enemy.png");
	enemyBulletTexture.loadFromFile("images/enemyBulletTexture.png");

	enemySprite.setTexture(enemyTexture);

	sf::Vector2f textureSize = sf::Vector2f(enemyTexture.getSize());
	float newSize = Scale::Value(1.0f);

	enemySprite.setScale(sf::Vector2f(newSize / textureSize.x, newSize / textureSize.y));

	sf::FloatRect spriteBounds = enemySprite.getLocalBounds();

	enemySprite.setOrigin(sf::Vector2f(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f));
}

void Enemy::Update(float deltaT, Player& player)
{

	shootTimer -= deltaT;
	if (shootTimer < 0.0f)
	{
		shootTimer += shootPeriod;
		// Create bullet
		bullets.push_back(Bullet());
		bullets[bullets.size() - 1].sprite.setTexture(enemyBulletTexture);
		
		sf::Vector2f textureSize = sf::Vector2f(enemyBulletTexture.getSize());
		sf::Vector2f newSize = sf::Vector2f(Scale::Value(0.15f), Scale::Value(0.15f));

		bullets[bullets.size() - 1].sprite.setScale(sf::Vector2f(newSize.x / textureSize.x, newSize.y / textureSize.y));

		sf::FloatRect spriteBounds = bullets[bullets.size() - 1].sprite.getLocalBounds();

		bullets[bullets.size() - 1].sprite.setOrigin(sf::Vector2f(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f));

		bullets[bullets.size() - 1].sprite.setPosition(enemySprite.getPosition());

		if (player.GetScreenPos().x < enemySprite.getPosition().x)
		{
			bullets[bullets.size() - 1].velocity = Scale::Value(-10.0f);
		}
		else
		{
			bullets[bullets.size() - 1].velocity = Scale::Value(10.0f);
		}
	}



}

void Enemy::SetPosition(sf::Vector2f newPosition)
{
	enemySprite.setPosition(newPosition);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(enemySprite);
	for (int i = 0; i < bullets.size(); i++) 
	{
		target.draw(bullets[i].sprite);
	}

}
