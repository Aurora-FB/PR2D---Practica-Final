#pragma once

#include "Collider.h"

class RectCollider : public Collider
{
public:

	RectCollider(const Vec2& _position, const Vec2& _size);

	bool collides(const Collider& other) const override;

	const Vec2& getPosition() const;
	void setPosition(const Vec2& _position);

	const Vec2& getSize() const;
	void setSize(const Vec2& _size);

private:
	Vec2 vPosition;
	Vec2 vSize;

	bool collides(const Vec2& circlePos, float circleRadius) const override;
	bool collides(const Vec2& rectPos, const Vec2& rectSize) const override;
	bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;
};