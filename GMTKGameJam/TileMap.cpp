#include "TileMap.hpp"

#include "Enemy.hpp"
#include "Player.hpp"

TileMap::TileMap(std::string tilesetFilePath, sf::Vector2u tilesetTileSize, sf::Vector2f windowSize) :
	_tilesetTileSize(tilesetTileSize), _windowSize(windowSize)
{
	loadTileset(tilesetFilePath);
	_flagTexture.loadFromFile("images/finishFlag.png");
	_flagSprite.setTexture(_flagTexture);

}

void TileMap::loadTileset(std::string filePath)
{
	_tileset.loadFromFile(filePath);

}

void TileMap::setLevel(Level newLevel)
{
	_tiles = newLevel.tiles;
	_numRows = newLevel.numRows;
	_numCols = newLevel.numCols;

	_tileSize.x = _windowSize.x / (_numCols);
	_tileSize.y = (_windowSize.y - (_windowSize.y / 10.0f)) / (_numRows);
	
	// Adjust non-square levels
	if (_tileSize.x > _tileSize.y) {
		_mapTopLeft.x = (_windowSize.x - ((_tileSize.y) * (float)_numCols)) / 2.0f;

		_mapTopLeft.y = 0;

		_tileSize.x = _tileSize.y;
	}
	else if (_tileSize.y > _tileSize.x) {
		_mapTopLeft.y = ((_windowSize.y - (_windowSize.y / 10.0f)) - ((_tileSize.x) * (float)_numRows)) / 2.0f;

		_mapTopLeft.x = 0;
		_tileSize.y = _tileSize.x;
	}
	else {
		_mapTopLeft = sf::Vector2f(0, 0);
	}

	_spawnPos = newLevel.spawnPosition;
	_endPos = newLevel.endPosition;

	sf::Vector2f flagTextureSize = sf::Vector2f(_flagTexture.getSize());
	_flagSprite.setScale(_tileSize.x / flagTextureSize.x, _tileSize.y / flagTextureSize.y);
	_flagSprite.setPosition(getScreenPosFromTilePos(_endPos));

	initVertexBuffer();
	generateVertexBuffer();
}

sf::Vector2f TileMap::getTileSize() const
{
	return _tileSize;
}

sf::Vector2f TileMap::getScreenPosFromTilePos(sf::Vector2i tilePos) const
{
	return sf::Vector2f(tilePos.x * _tileSize.x, tilePos.y * _tileSize.y) + _mapTopLeft;
}

sf::Vector2i TileMap::getTilePosFromScreenPos(sf::Vector2f screenPos) const
{
	screenPos -= _mapTopLeft;

	return sf::Vector2i(screenPos.x / _tileSize.x, screenPos.y / _tileSize.y);
}

sf::Vector2f TileMap::getSpawnPosition() const
{
	return getScreenPosFromTilePos(_spawnPos);
}

sf::Vector2i TileMap::getEndTilePos() const
{
	return _endPos;
}

char TileMap::getTile(sf::Vector2i tilePos) const
{
	return _tiles[tilePos.x + (_numCols * tilePos.y)];
}

char TileMap::getTile(sf::Vector2f screenPos) const
{
	return getTile(getTilePosFromScreenPos(screenPos));
}

char TileMap::getTileValue(sf::Vector2i tilePos) const
{
	return _tiles[tilePos.x + (tilePos.y * _numCols)];
}

char TileMap::getTileValue(sf::Vector2f screenPos) const
{
	return getTileValue(getTilePosFromScreenPos(screenPos));
}

unsigned int TileMap::getNumCols() const
{
	return _numCols;
}

unsigned int TileMap::getNumRows() const
{
	return _numRows;
}

sf::Vector2f TileMap::getLevelSize() const
{
	return sf::Vector2f(_numCols * _tileSize.x, _numRows * _tileSize.y);
}

sf::FloatRect TileMap::getLevelBounds() const
{
	return sf::FloatRect(_mapTopLeft.x, _mapTopLeft.y, _numCols * _tileSize.x, _numRows * _tileSize.y);
}

void TileMap::Update(float deltaT, Player& player)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].Update(deltaT, player);
	}
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &_tileset;

	target.draw(_vertices, states);

	target.draw(_flagSprite);
	for (int i = 0; i < enemies.size(); i++)
	{
		target.draw(enemies[i]);
	}
}

void TileMap::initVertexBuffer()
{
	_vertices.setPrimitiveType(sf::Quads);
	_vertices.create((size_t)_numRows * (size_t)_numCols * 4);
	_vertices.setUsage(sf::VertexBuffer::Usage::Static);
}

void TileMap::generateVertexBuffer()
{
	sf::Vertex* vertArray = new sf::Vertex[(size_t)_numRows * (size_t)_numCols * 4];

	for (unsigned int i = 0; i < _tiles.size(); i++) {
		unsigned int x = i % _numCols;
		unsigned int y = i / _numCols;

		char tile;
		if (_tiles[i] == 5)
		{
			tile = 0;
			Enemy newEnemy;
			newEnemy.SetPosition(sf::Vector2f(x * _tileSize.x + (_tileSize.x / 2.0f), y * _tileSize.y + (_tileSize.y / 2.0f)));
			enemies.push_back(newEnemy);
		}
		else {
			tile = _tiles[i];
		}

		int tu = (tile) % (_tileset.getSize().x / _tilesetTileSize.x);
		int tv = (tile) / (_tileset.getSize().x / _tilesetTileSize.x);



		sf::Vertex* quad = &vertArray[(size_t)i * 4];

		quad[0].position = sf::Vector2f(x * _tileSize.x, y * _tileSize.y) + _mapTopLeft;
		quad[1].position = sf::Vector2f((x + 1) * _tileSize.x, y * _tileSize.y) + _mapTopLeft;
		quad[2].position = sf::Vector2f((x + 1) * _tileSize.x, (y + 1) * _tileSize.y) + _mapTopLeft;
		quad[3].position = sf::Vector2f(x * _tileSize.x, (y + 1) * _tileSize.y) + _mapTopLeft;

		quad[0].texCoords = sf::Vector2f(tu * _tilesetTileSize.x, tv * _tilesetTileSize.y);
		quad[1].texCoords = sf::Vector2f((tu + 1) * _tilesetTileSize.x, tv * _tilesetTileSize.y);
		quad[2].texCoords = sf::Vector2f((tu + 1) * _tilesetTileSize.x, (tv + 1) * _tilesetTileSize.y);
		quad[3].texCoords = sf::Vector2f(tu * _tilesetTileSize.x, (tv + 1) * _tilesetTileSize.y);

	}

	_vertices.update(vertArray);
	delete[] vertArray;

}