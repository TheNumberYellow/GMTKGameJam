#pragma once
#include "SFML/Graphics.hpp"

#include "Enemy.hpp"
class Player;


struct Level {
	std::vector<char> tiles;
	unsigned int numCols, numRows;
	sf::Vector2i spawnPosition;
	sf::Vector2i endPosition;

	Level() :
		numRows(0), numCols(0) 
	{
	}

	Level(std::vector<char> tiles, 
		unsigned int numCols, unsigned int numRows,
		sf::Vector2i spawnPosition, sf::Vector2i endPosition) :

		tiles(tiles), numCols(numCols), numRows(numRows),
		spawnPosition(spawnPosition), endPosition(endPosition)
	{
	}

};


class TileMap : 
	public sf::Drawable
{
public:
	TileMap(std::string tilesetFilePath, sf::Vector2u tilesetTileSize, sf::Vector2f windowSize);
	
	void loadTileset(std::string filePath);

	void setLevel(Level newLevel);

	sf::Vector2f getTileSize() const;

	sf::Vector2f getScreenPosFromTilePos(sf::Vector2i tilePos) const;
	sf::Vector2i getTilePosFromScreenPos(sf::Vector2f screenPos) const;
	
	sf::Vector2f getSpawnPosition() const;
	sf::Vector2i getEndTilePos() const;

	char getTile(sf::Vector2i tilePos) const;
	char getTile(sf::Vector2f screenPos) const;

	char getTileValue(sf::Vector2i tilePos) const;
	char getTileValue(sf::Vector2f screenPos) const;

	unsigned int getNumCols() const;
	unsigned int getNumRows() const;

	sf::Vector2f getLevelSize() const;
	sf::FloatRect getLevelBounds() const;

	void Update(float deltaT, Player& player);

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

	void initVertexBuffer();
	void generateVertexBuffer();

	unsigned int _numRows, _numCols;
	sf::Vector2f _tileSize;

	sf::Vector2f _mapTopLeft;

	sf::Vector2u _tilesetTileSize;
	sf::Vector2f _windowSize;

	sf::Vector2i _spawnPos;
	sf::Vector2i _endPos;

	sf::Texture _tileset;
	sf::Texture _flagTexture;

	sf::Sprite _flagSprite;

	std::vector<char> _tiles;
	sf::VertexBuffer _vertices;


	std::vector<Enemy> enemies;
};

