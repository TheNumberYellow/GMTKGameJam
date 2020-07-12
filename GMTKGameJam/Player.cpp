#include "Player.hpp"
#include "TileMap.hpp"

#include "MathUtils.hpp"

#include <iostream>

bool lineIntersect(Edge a, Edge b, sf::Vector2f& intersectPoint, float* aT, float* bT) {
	float tAA = (b.a.y - b.b.y) * (a.a.x - b.a.x) + (b.b.x - b.a.x) * (a.a.y - b.a.y);
	float tAB = (b.b.x - b.a.x) * (a.a.y - a.b.y) - (a.a.x - a.b.x) * (b.b.y - b.a.y);

	float tBA = (a.a.y - a.b.y) * (a.a.x - b.a.x) + (a.b.x - a.a.x) * (a.a.y - b.a.y);
	float tBB = (b.b.x - b.a.x) * (a.a.y - a.b.y) - (a.a.x - a.b.x) * (b.b.y - b.a.y);

	if (tAB == 0.0f || tBB == 0.0f) {
		return false;
	}
	float tA = tAA / tAB;
	float tB = tBA / tBB;

	if (tA >= 0.0f && tA <= 1.0f && tB >= 0.0f && tB <= 1.0f) {

		intersectPoint = a.a + tA * (a.b - a.a);

		if (aT != nullptr) {
			*aT = tA;
		}
		if (bT != nullptr) {
			*bT = tB;
		}

		return true;
	}
	else {
		return false;
	}
}

inline Edge::Edge() {

}

inline Edge::Edge(sf::Vector2f a, sf::Vector2f b) : a(a), b(b)
{
	normal = sf::Vector2f((b - a).y, -(b - a).x);

	float len = vecLength(normal);

	normal /= len;
}


void Player::Move(sf::Vector2f displacementVector, const TileMap& tileMap)
{

	onGround = false;
	static const float MIN_SEPERATION = 0.001f;
	static const unsigned int MAX_PHYSICS_ITERATIONS = 5;

	sf::Vector2f playerExtents = sf::Vector2f(playerSprite.getGlobalBounds().width / 2.0f, playerSprite.getGlobalBounds().height / 2.0f);

	float remainingT = 1.0f;

	for (int k = 0; k < MAX_PHYSICS_ITERATIONS && remainingT > 0.0f; k++) {
		sf::Vector2f startPos = playerSprite.getPosition();
		sf::Vector2f endPos = playerSprite.getPosition() + displacementVector;

		sf::Vector2f minMovement = displacementVector;
		float minT = remainingT;

		// Check tilemap boundaries
		Edge topBoundary(sf::Vector2f(tileMap.getLevelBounds().left, tileMap.getLevelBounds().top), sf::Vector2f(tileMap.getLevelBounds().left + tileMap.getLevelBounds().width, tileMap.getLevelBounds().top));
		Edge botBoundary(sf::Vector2f(tileMap.getLevelBounds().left + tileMap.getLevelBounds().width, tileMap.getLevelBounds().top + tileMap.getLevelBounds().height), sf::Vector2f(tileMap.getLevelBounds().left, tileMap.getLevelBounds().top + tileMap.getLevelBounds().height));
		Edge leftBoundary(sf::Vector2f(tileMap.getLevelBounds().left, tileMap.getLevelBounds().top + tileMap.getLevelBounds().height), sf::Vector2f(tileMap.getLevelBounds().left, tileMap.getLevelBounds().top));
		Edge rightBoundary(sf::Vector2f(tileMap.getLevelBounds().left + tileMap.getLevelBounds().width, tileMap.getLevelBounds().top), sf::Vector2f(tileMap.getLevelBounds().left + tileMap.getLevelBounds().width, tileMap.getLevelBounds().top + tileMap.getLevelBounds().height));

		sf::Vector2f boundaryIntersection;

		float playerT;
		float boundaryT;

		bool looped = false;

		bool hitBottom = false;

		if (lineIntersect(Edge{ startPos, endPos }, topBoundary, boundaryIntersection, &playerT, &boundaryT)) {
			if (playerT < minT) {
				minMovement = sf::Vector2f(boundaryIntersection.x, tileMap.getLevelBounds().top + tileMap.getLevelBounds().height) + (topBoundary.normal * MIN_SEPERATION) - startPos;
				looped = true;
			}
		}
		if (lineIntersect(Edge{ startPos, endPos }, botBoundary, boundaryIntersection, &playerT, &boundaryT)) {
			if (playerT < minT) {
				minMovement = sf::Vector2f(boundaryIntersection.x, tileMap.getLevelBounds().top) + (botBoundary.normal * MIN_SEPERATION) - startPos;
				looped = true;
			}
		}
		if (lineIntersect(Edge{ startPos, endPos }, leftBoundary, boundaryIntersection, &playerT, &boundaryT)) {
			if (playerT < minT) {
				minMovement = sf::Vector2f(tileMap.getLevelBounds().left + tileMap.getLevelBounds().width, boundaryIntersection.y) + (leftBoundary.normal * MIN_SEPERATION) - startPos;
				looped = true;
			}
		}
		if (lineIntersect(Edge{ startPos, endPos }, rightBoundary, boundaryIntersection, &playerT, &boundaryT)) {
			if (playerT < minT) {
				minMovement = sf::Vector2f(tileMap.getLevelBounds().left, boundaryIntersection.y) + (rightBoundary.normal * MIN_SEPERATION) - startPos;
				looped = true;
			}
		}


		sf::Vector2i startTilePos = tileMap.getTilePosFromScreenPos(playerSprite.getPosition());
		sf::Vector2i endTilePos = tileMap.getTilePosFromScreenPos(playerSprite.getPosition() + displacementVector);

		sf::Vector2i topLeftTilePos = sf::Vector2i(startTilePos.x < endTilePos.x ? startTilePos.x : endTilePos.x, startTilePos.y < endTilePos.y ? startTilePos.y : endTilePos.y) - sf::Vector2i(1, 1);
		sf::Vector2i botRightTilePos = sf::Vector2i(startTilePos.x > endTilePos.x ? startTilePos.x : endTilePos.x, startTilePos.y > endTilePos.y ? startTilePos.y : endTilePos.y) + sf::Vector2i(1, 1);

		bool collided = false;
		sf::Vector2f collisionNormal;

		for (int i = topLeftTilePos.x; i != botRightTilePos.x + 1; i++) {
			for (int j = topLeftTilePos.y; j != botRightTilePos.y + 1; j++) {
				if (i < 0 || j < 0 || i >= tileMap.getNumCols() || j >= tileMap.getNumRows()) continue; // Out of tilemap bounds (will want to loop later)

				char tile = tileMap.getTile(sf::Vector2i(i, j));

				if (tile == 0) continue;

				// Check each tile side against movement vector
				Edge topEdge(tileMap.getScreenPosFromTilePos(sf::Vector2i(i, j)) - playerExtents, tileMap.getScreenPosFromTilePos(sf::Vector2i(i + 1, j)) + sf::Vector2f(playerExtents.x, -playerExtents.y));
				Edge botEdge(tileMap.getScreenPosFromTilePos(sf::Vector2i(i + 1, j + 1)) + playerExtents, tileMap.getScreenPosFromTilePos(sf::Vector2i(i, j + 1)) + sf::Vector2f(-playerExtents.x, playerExtents.y));
				Edge leftEdge(tileMap.getScreenPosFromTilePos(sf::Vector2i(i, j + 1)) + sf::Vector2f(-playerExtents.x, playerExtents.y), tileMap.getScreenPosFromTilePos(sf::Vector2i(i, j)) - playerExtents);
				Edge rightEdge(tileMap.getScreenPosFromTilePos(sf::Vector2i(i + 1, j)) + sf::Vector2f(playerExtents.x, -playerExtents.y), tileMap.getScreenPosFromTilePos(sf::Vector2i(i + 1, j + 1)) + playerExtents);


				sf::Vector2f intersectionPoint;
				float nearestT;
				bool hitThisTile = false;

				if (lineIntersect(Edge{ startPos, endPos }, topEdge, intersectionPoint, &nearestT, nullptr)) {
					onGround = true;
					if (nearestT < minT) {
						hitBottom = true;

						looped = false;
						collided = true;
						collisionNormal = topEdge.normal;
						minT = nearestT;
						minMovement = intersectionPoint + (topEdge.normal * MIN_SEPERATION) - startPos;

						hitThisTile = true;
					}
				}
				if (lineIntersect(Edge{ startPos, endPos }, botEdge, intersectionPoint, &nearestT, nullptr)) {
					if (nearestT < minT) {
						hitBottom = false;

						looped = false;
						collided = true;
						collisionNormal = botEdge.normal;
						minT = nearestT;
						minMovement = intersectionPoint + (botEdge.normal * MIN_SEPERATION) - startPos;

						hitThisTile = true;
					}
				}
				if (lineIntersect(Edge{ startPos, endPos }, leftEdge, intersectionPoint, &nearestT, nullptr)) {
					if (nearestT < minT) {
						hitBottom = false;

						looped = false;
						collided = true;
						collisionNormal = leftEdge.normal;
						minT = nearestT;
						minMovement = intersectionPoint + (leftEdge.normal * MIN_SEPERATION) - startPos;

						hitThisTile = true;
					}
				}
				if (lineIntersect(Edge{ startPos, endPos }, rightEdge, intersectionPoint, &nearestT, nullptr)) {
					if (nearestT < minT) {
						hitBottom = false;

						looped = false;
						collided = true;
						collisionNormal = rightEdge.normal;
						minT = nearestT;
						minMovement = intersectionPoint + (rightEdge.normal * MIN_SEPERATION) - startPos;

						hitThisTile = true;
					}
				}

				if (hitThisTile)

				{
					if (tile == 4)
					{
						died = true;
					}
				}

			}
		}

		//if (hitBottom) {
		//	if (_velocity.y > game.getWindowHeight() * 0.15f) {
		//		_sound.setBuffer(_impactSound);
		//		_sound.setVolume(25.f);
		//		_sound.play();

		//	}

		//}

		playerSprite.move(minMovement);
		if (!looped) {
			displacementVector = displacementVector - (collisionNormal * (dotProduct(displacementVector, collisionNormal)));
			velocity = velocity - (collisionNormal * (dotProduct(velocity, collisionNormal)));
		}
		displacementVector = displacementVector - (collisionNormal * (dotProduct(displacementVector, collisionNormal)));
		velocity = velocity - (collisionNormal * (dotProduct(velocity, collisionNormal)));
		remainingT -= minT;
	}
}

sf::Vector2i Player::GetTilePos(const TileMap& tileMap)
{
	return tileMap.getTilePosFromScreenPos(GetScreenPos());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(playerSprite);
	target.draw(barSprite);
	
	for (int i = 0; i < jumpSprites.size(); i++)
	{
		target.draw(jumpSprites[i]);
	}
	for (int i = 0; i < shootSprites.size(); i++)
	{
		target.draw(shootSprites[i]);
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		target.draw(bullets[i].sprite);
	}
}

Player::Player(std::string textureFilePath)
{
	playerTexture.loadFromFile(textureFilePath);
	playerSprite.setTexture(playerTexture);


	barTexture.loadFromFile("images/timerBackground.png");
	barSprite.setTexture(barTexture);

	jumpTexture.loadFromFile("images/jumpTexture.png");
	shootTexture.loadFromFile("images/shootTexture.png");
	bulletTexture.loadFromFile("images/bulletTexture.png");

	for (int i = 0; i < 5; i++)
	{
		jumpSprites.push_back(sf::Sprite());
		jumpSprites[i].setTexture(jumpTexture);
	}

	for (int i = 0; i < 5; i++)
	{
		shootSprites.push_back(sf::Sprite());
		shootSprites[i].setTexture(shootTexture);
	}

	sf::Vector2f textureSize = sf::Vector2f(barTexture.getSize());
	sf::Vector2f newSize = sf::Vector2f(Scale::Value(100.0f), Scale::Value(5.0f * (9.0f / 16.0f)));

	barSprite.setScale(sf::Vector2f(newSize.x / textureSize.x, newSize.y / textureSize.y));
	barSprite.setPosition(sf::Vector2f(0.0f, Scale::Value(100.0f * (9.0f / 16.0f)) - barSprite.getGlobalBounds().height));

	textureSize = sf::Vector2f(jumpTexture.getSize());
	newSize = sf::Vector2f(Scale::Value(1.0f), Scale::Value(5.0f * (9.0f / 16.0f)));

	for (int i = 0; i < 5; i++)
	{
		jumpSprites[i].setScale(sf::Vector2f(newSize.x / textureSize.x, newSize.y / textureSize.y));
	}

	for (int i = 0; i < 5; i++)
	{
		shootSprites[i].setScale(sf::Vector2f(newSize.x / textureSize.x, newSize.y / textureSize.y));
	}

	sf::FloatRect spriteBounds = playerSprite.getLocalBounds();

	playerSprite.setOrigin(sf::Vector2f(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f));
}

void Player::Update(float deltaT, const TileMap& tileMap)
{

	bool isUnderControl = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		velocity.x = -movementSpeed;
		isUnderControl = true;
		facingLeft = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x = movementSpeed;
		isUnderControl = true;
		facingLeft = false;
	}
	if (!isUnderControl)
	{
		velocity.x = 0.0f;
	}

	jumpTimer -= deltaT;
	shootTimer -= deltaT;


	if (jumpTimer < 0.0f)
	{
		jumpTimer += jumpPeriod;
		if (onGround)
		{
			velocity.y = jumpVelocity;
		}
	}
	
	if (shootTimer < 0.0f)
	{
		shootTimer += shootPeriod;
		// Shoot a bullet
		bullets.push_back(Bullet());
		bullets[bullets.size() - 1].sprite.setTexture(bulletTexture);
		
		sf::Vector2f textureSize = sf::Vector2f(bulletTexture.getSize());
		sf::Vector2f newSize = sf::Vector2f(Scale::Value(0.15f), Scale::Value(0.15f));

		bullets[bullets.size() - 1].sprite.setScale(sf::Vector2f(newSize.x / textureSize.x, newSize.y / textureSize.y));

		sf::FloatRect spriteBounds = bullets[bullets.size() - 1].sprite.getLocalBounds();

		bullets[bullets.size() - 1].sprite.setOrigin(sf::Vector2f(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f));

		bullets[bullets.size() - 1].sprite.setPosition(playerSprite.getPosition());

		if (facingLeft)
		{
			bullets[bullets.size() - 1].velocity = -10.0f;
		}
		else
		{
			bullets[bullets.size() - 1].velocity = 10.0f;
		}
	}

	float jumpPositionRatio = jumpTimer / jumpPeriod;
	float shootPositionRatio = shootTimer / shootPeriod;
	float screenWidth;
	float screenHeight;

	if (tileMap.getNumCols() > tileMap.getNumRows())
	{
		screenWidth = tileMap.getNumCols() * tileMap.getTileSize().x;
		screenHeight = screenWidth * (9.0f / 16.0f);
	}
	else
	{
		screenHeight = tileMap.getNumRows() * tileMap.getTileSize().y + ((tileMap.getTileSize().y * tileMap.getNumRows()) / 9.0f);
		screenWidth = screenHeight * (16.0f / 9.0f);
	}
	
	for (int i = 0; i < jumpSprites.size(); i++)
	{
		jumpSprites[i].setPosition(jumpPositionRatio * (screenWidth / 5.0f) * jumpPeriod + (screenWidth / 5.0f * i) * jumpPeriod, screenHeight - screenHeight * 0.05f);
	}

	for (int i = 0; i < shootSprites.size(); i++)
	{
		shootSprites[i].setPosition(shootPositionRatio * (screenWidth / 5.0f) * shootPeriod + (screenWidth / 5.0f * i) * shootPeriod, screenHeight - screenHeight * 0.05f);
	}

	velocity.y += gravityAcceleration * deltaT;

	Move(velocity * deltaT, tileMap);

	for (int i = bullets.size() - 1; i >= 0; i--)
	{
		bullets[i].sprite.move(sf::Vector2f(bullets[i].velocity, 0.0f));

		if (bullets[i].sprite.getPosition().x > screenWidth || bullets[i].sprite.getPosition().x < 0.0f || bullets[i].sprite.getPosition().y > screenHeight || bullets[i].sprite.getPosition().y < 0.0f)
		{
			bullets.erase(bullets.begin() + i);
		}
	}


	if (tileMap.getEndTilePos() == GetTilePos(tileMap))
	{
		touchedLevelEnd = true;

	}
}
 
void Player::SetPosition(sf::Vector2f newPosition)
{
	playerSprite.setPosition(newPosition);
}

void Player::SetSize(sf::Vector2f newSize)
{
	sf::Vector2f textureSize = sf::Vector2f(playerTexture.getSize());

	playerSprite.setScale(sf::Vector2f(newSize.x / textureSize.x, newSize.y / textureSize.y));

	//sf::FloatRect spriteBounds = playerSprite.getLocalBounds();

	//playerSprite.setOrigin(sf::Vector2f(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f));
}

void Player::ScaleParameters()
{
	movementSpeed = Scale::Value(5.0f);
	jumpVelocity = Scale::Value(-12.0f);
	gravityAcceleration = Scale::Value(60.0f);
}

void Player::ResetTimers()
{
	jumpTimer = jumpPeriod;
	shootTimer = shootPeriod + 0.5f;
}

sf::Vector2f Player::GetScreenPos() const
{
	return playerSprite.getPosition();
}

bool Player::TouchedLevelEnd()
{
	if (touchedLevelEnd)
	{
		touchedLevelEnd = false;
		return true;
	}
	return false;
}

bool Player::Died()
{
	if (died)
	{
		died = false;
		return true;
	}
	return false;
}

bool Player::WantsReset()
{
	if (wantsReset)
	{
		wantsReset = false;
		return true;
	}
	return false;
}
