#pragma once

#include "Vec2.h"
#include <cstdint>

class Collider
{
public:

	Collider() {};
	virtual bool collides(const Collider& other) const = 0; //si se colisiona con otro collider


private:
	virtual bool collides(const Vec2& circlePos, float circleRadius) const = 0; //si el collider colisiona con un circulo
	virtual bool collides(const Vec2& rectPos, const Vec2& rectSize) const = 0; //si el collider collisiona con un rectangulo
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const = 0; //si el collider colisiona con un pixel
};