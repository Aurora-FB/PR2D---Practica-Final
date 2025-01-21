
#include "CircleCollider.h"
#include "RectCollider.h"
#include "PixelsCollider.h"
#include "CollisionFunctions.h"

CircleCollider::CircleCollider(const Vec2& _position, float _radius)
    : vPosition(_position), fRadius(_radius) {
}

bool CircleCollider::collides(const Collider& other) const 
{
    if (const CircleCollider* circle = dynamic_cast<const CircleCollider*>(&other))
    {
        return collides(circle->getPosition(), circle->getRadius());
    }
    else if (const RectCollider* rect = dynamic_cast<const RectCollider*>(&other))
    {
        return collides(rect->getPosition(), rect->getSize());
    }
    else if (const PixelsCollider* pixels = dynamic_cast<const PixelsCollider*>(&other))
    {
        return collides(pixels->getPosition(), pixels->getSize(), pixels->getPixels());
    }
    else
    {
        return false;
    }
}

bool CircleCollider::collides(const Vec2& circlePos, float circleRadius) const 
{
    return checkCircleCircle(vPosition, fRadius, circlePos, circleRadius);
}

bool CircleCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const 
{
    return checkCircleRect(vPosition, fRadius, rectPos, rectSize);
}

bool CircleCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const 
{
    return checkCirclePixels(vPosition, fRadius, pixelsPos, pixelsSize, pixels);
}

const Vec2& CircleCollider::getPosition() const
{
    return vPosition;
}

void CircleCollider::setPosition(const Vec2& _position) 
{
    vPosition = _position;
}

const float CircleCollider::getRadius() const
{
    return fRadius;
}

void CircleCollider::setRadius(const float _radius) 
{
    fRadius = _radius;
}