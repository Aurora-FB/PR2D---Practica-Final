#pragma once

#include "Vec2.h"
#include "Sprite.h"
#include "RectCollider.h"
#include <litegfx.h>
#include <vector>

struct Tile
{
	Tile(int id, const Vec2& pos, const ltex_t* tTexture, Vec2 size, int tColumn, int tRow)
	{
		
		tileId = id;
		tilePos = pos;
		tilesetTexture = tTexture;
		tileSize = size;
		tileColumn = tColumn;
		tileRow = tRow;

		collisionType = COLLISION_RECT;
		collider = new RectCollider(tilePos.operator+(0.5*32), tileSize);
	}

	~Tile() {
		delete collider;
		collider = nullptr;
	};

	Collider* getCollider() const
	{
		return collider;
	}

	Vec2 getSize() const
	{
		return tileSize;
	}

	int tileId;
	Vec2 tilePos;
	const ltex_t* tilesetTexture;
	Vec2 tileSize;
	int tileColumn; // Número de columnas en el tileset
	int tileRow;    // Número de filas en el tileset

	CollisionType collisionType;
	Collider* collider;
};


class World
{
public:
	World(
		float clearRed = 0.15f, 
		float clearGreen = 0.15f, 
		float clearBlue = 0.15f,
		const ltex_t* back0 = nullptr, 
		const ltex_t* back1 = nullptr, 
		const ltex_t* back2 = nullptr, 
		const ltex_t* back3 = nullptr);
	
	float getClearRed() const;
	float getClearGreen() const;
	float getClearBlue() const;
	const ltex_t* getBackground(size_t layer) const;

	float getScrollRatio(size_t layer) const;
	void setScrollRatio(size_t layer, float ratio);

	const Vec2& getScrollSpeed(size_t layer) const;
	void setScrollSpeed(size_t layer, const Vec2& speed);

	const Vec2& getCameraPosition() const;
	void setCameraPosition(const Vec2& pos);

	void addSprite(Sprite& sprite);
	void removeSprite(Sprite& sprite);

	Vec2 getMapSize() const;
	bool loadMap(const char* filename, uint16_t firstColId);

	int checkCollision(Sprite& sprite, std::vector<std::vector<Tile*>> _tiles);
	bool moveSprite(Sprite& sprite, const Vec2& amount);
	void update(float deltaTime);
	void draw(const Vec2& screenSize);

private:
	float clearRed;
	float clearGreen;
	float clearBlue;
	const ltex_t* backgrounds[4];
	float scrollRatios[4];
	Vec2 scrollSpeeds[4];
	Vec2 cameraPos;
	std::vector<Sprite*> sprites;
	Vec2 scrollOffsets[4];

	//Tiles:
	Vec2 mapSize;
	std::vector<std::vector<Tile*>> tiles;
};