#pragma once

#include "Collider.h"

class PixelsCollider : public Collider
{
public:

	PixelsCollider(const Vec2& _position, const Vec2& _size, const uint8_t* _pixels);

	bool collides(const Collider& other) const override;

	const Vec2& getPosition() const;
	void setPosition(const Vec2& _position);

	const Vec2& getSize() const;
	void setSize(const Vec2& _size);

	const uint8_t* getPixels() const;
	void setPixels(const uint8_t* _pixels);

private:
	Vec2 vPosition;
	Vec2 vSize;
	const uint8_t* pixels;

	bool collides(const Vec2& circlePos, float circleRadius) const override;
	bool collides(const Vec2& rectPos, const Vec2& rectSize) const override;
	bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;
};
