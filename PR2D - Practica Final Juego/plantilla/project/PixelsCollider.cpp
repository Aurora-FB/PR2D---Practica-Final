

#include "PixelsCollider.h"
#include "RectCollider.h"
#include "CircleCollider.h"
#include "CollisionFunctions.h"

PixelsCollider::PixelsCollider(const Vec2& _position, const Vec2& _size, const uint8_t* _pixels)
    : vPosition(_position), vSize(_size), pixels(_pixels) {
}

bool PixelsCollider::collides(const Collider& other) const
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

bool PixelsCollider::collides(const Vec2& circlePos, float circleRadius) const
{
    return checkCirclePixels(circlePos, circleRadius, vPosition, vSize, pixels);
}

bool PixelsCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const
{
    return checkPixelsRect(vPosition, vSize, pixels, rectPos, rectSize);
}

bool PixelsCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* _pixels) const
{
    return checkPixelsPixels(vPosition, vSize, pixels, pixelsPos, pixelsSize, _pixels);
}

const Vec2& PixelsCollider::getPosition() const
{
    return vPosition;
}

void PixelsCollider::setPosition(const Vec2& _position)
{
    vPosition = _position;
}

const Vec2& PixelsCollider::getSize() const
{
    return vSize;
}

void PixelsCollider::setSize(const Vec2& _size)
{
    vSize = _size;
}

const uint8_t* PixelsCollider::getPixels() const
{
    return pixels;
};

void PixelsCollider::setPixels(const uint8_t* _pixels)
{
    pixels = _pixels;
};