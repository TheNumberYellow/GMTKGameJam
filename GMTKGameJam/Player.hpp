#pragma once
#include "SFML/Graphics.hpp"

class Bullet
{
public:

	float velocity;
	sf::Sprite sprite;
};

class TileMap;

inline float dotProduct(sf::Vector2f a, sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}

inline float vecLength(sf::Vector2f vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

struct Edge {
	sf::Vector2f a, b;
	Edge();

	Edge(sf::Vector2f a, sf::Vector2f b);

	sf::Vector2f normal;
};

bool lineIntersect(Edge a, Edge b, sf::Vector2f& intersectPoint, float* aT, float* bT);


class Player : public sf::Drawable
{
private:

	sf::Texture playerTexture;
	sf::Texture barTexture;
	sf::Texture jumpTexture;
	sf::Texture shootTexture;
	sf::Texture bulletTexture;

	sf::Sprite playerSprite;
	sf::Sprite barSprite;
	std::vector<sf::Sprite> jumpSprites;
	std::vector<sf::Sprite> shootSprites;
	std::vector<Bullet> bullets;

	sf::Vector2f velocity;
	bool onGround = false;
	bool holdingSpace = false;

	void Move(sf::Vector2f displacementVector, const TileMap &tileMap);

	float movementSpeed;
	float jumpVelocity;
	float gravityAcceleration;

	sf::Vector2i GetTilePos(const TileMap& tileMap);

	bool touchedLevelEnd = false;
	bool died = false;
	bool wantsReset = false;


	float jumpPeriod = 1.0f;
	float jumpTimer = jumpPeriod;

	float shootPeriod = 2.0;
	float shootTimer = shootPeriod + 0.5;

	bool facingLeft = false;
protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:

	Player(std::string textureFilePath);

	void Update(float deltaT, const TileMap &tileMap);

	void SetPosition(sf::Vector2f newPosition);
	void SetSize(sf::Vector2f newSize);
	void ScaleParameters();
	void ResetTimers();

	sf::Vector2f GetScreenPos() const;

	bool TouchedLevelEnd();
	bool Died();
	bool WantsReset();

};

