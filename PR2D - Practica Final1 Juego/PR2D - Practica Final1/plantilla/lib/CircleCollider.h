#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
public:

	CircleCollider(const Vec2& _position, float _radius);

	bool collides(const Collider& other) const override;

	const Vec2& getPosition() const;
	void setPosition(const Vec2& _position);

	const float getRadius() const;
	void setRadius(const float _radius);

private:
	Vec2 vPosition;
	float fRadius;

	bool collides(const Vec2& circlePos, float circleRadius) const override;
	bool collides(const Vec2& rectPos, const Vec2& rectSize) const override;
	bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;
};