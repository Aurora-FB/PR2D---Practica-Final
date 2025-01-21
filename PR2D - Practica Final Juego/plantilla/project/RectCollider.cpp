

#include "RectCollider.h"
#include "CircleCollider.h"
#include "PixelsCollider.h"

#include "CollisionFunctions.h"

RectCollider::RectCollider(const Vec2& _position, const Vec2& _size)
    : vPosition(_position), vSize(_size) {
}

bool RectCollider::collides(const Collider& other) const
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

bool RectCollider::collides(const Vec2& circlePos, float circleRadius) const
{
    return checkCircleRect(circlePos, circleRadius, vPosition, vSize);
}

bool RectCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const
{
    return checkRectRect(rectPos, rectSize, vPosition, vSize);
}

bool RectCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
    return checkPixelsRect(pixelsPos, pixelsSize, pixels, vPosition, vSize);
}

const Vec2& RectCollider::getPosition() const
{
    return vPosition;
}

void RectCollider::setPosition(const Vec2& _position)
{
    vPosition = _position;
}

const Vec2& RectCollider::getSize() const
{
    return vSize;
}

void RectCollider::setSize(const Vec2& _size)
{
    vSize = _size;
}