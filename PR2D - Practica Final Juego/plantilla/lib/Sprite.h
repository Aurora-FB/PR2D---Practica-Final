#pragma once

#include "Collider.h"
#include <litegfx.h> 
#include "Color.h"

enum CollisionType {

	COLLISION_NONE,
	COLLISION_CIRCLE,
	COLLISION_RECT,
	COLLISION_PIXELS

};

class Sprite
{
public:

	typedef void (*CallbackFunc)(Sprite&, float);

	Sprite(int id, const ltex_t* _tex, int _hframes = 1, int _vframes = 1);
	~Sprite();
	int getId();
	void setCallback(CallbackFunc func);

	void* getUserData();
	void setUserData(void* data);

	const ltex_t* getTexture() const;
	void setTexture(const ltex_t* tex, int hframes = 1, int vframes = 1);

	lblend_t getBlend() const;
	void setBlend(lblend_t mode);

	float getRed() const;
	float getGreen() const;
	float getBlue() const;
	float getAlpha() const;
	void setColor(float r, float g, float b, float a);

	const Vec2& getPosition() const;
	void setPosition(const Vec2& _position);

	float getAngle() const;
	void setAngle(float _angle);

	const Vec2& getScale() const;
	void setScale(const Vec2& scale);

	Vec2 getSize() const;

	const Vec2& getPivot() const;
	void setPivot(const Vec2& pivot);

	int getHframes() const;
	int getVframes() const;

	int getFps() const;
	void setFps(int fps);

	float getCurrentFrame() const;
	void setCurrentFrame(int frame);


	CollisionType getCollisionType() const;
	void setCollisionType(CollisionType type);

	Collider* getCollider() const;
	bool collides(const Sprite& other) const;

	void update(float deltaTime);
	void draw();

private:

	int spriteId;
	CallbackFunc callback = nullptr;
	const ltex_t* tex;
	int hframes, vframes;

	void* userData = nullptr;
	lblend_t blend = BLEND_ADD;
	Color color;
	Vec2 position;
	float angle = 0.0f;
	Vec2 scale;
	Vec2 size;
	Vec2 pivot;
	int fps = 0;
	float currentFrame = 0.0f;
	float timeAccumulator = 0.0f;

	CollisionType collisionType;
	Collider* collider;
	const uint8_t* pixels = nullptr;
};
