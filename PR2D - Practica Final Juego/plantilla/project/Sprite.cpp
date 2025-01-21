
#include "Sprite.h"  // Archivo de cabecera local
#include "Collider.h"
#include "CircleCollider.h"
#include "RectCollider.h"
#include "PixelsCollider.h"

#include <litegfx.h>
#include <iostream>

using namespace std;

Sprite::Sprite(int _id, const ltex_t* _tex, int _hframes, int _vframes)
{
    spriteId = _id;
    tex = _tex;
    hframes = _hframes;
    vframes = _vframes;

    collider = nullptr;
    collisionType = COLLISION_NONE;
    unsigned char* pi =  new unsigned char[tex->width * tex->height * 4];
    ltex_getpixels(_tex, pi);
    pixels = pi;
    size.x = tex->width / hframes;
    size.y = tex->height / vframes;
    scale.x = 1;
    scale.y = 1;
    pivot.x = 0.5;
    pivot.y = 0.5;
}

Sprite::~Sprite()
{
        // Liberar el collider si fue creado dinámicamente
        if (collider) {
            delete collider;
            collider = nullptr;
        }

        // Liberar los píxeles si fueron asignados dinámicamente
        if (pixels) {
            delete[] pixels; // Liberamos la memoria de los píxeles
            pixels = nullptr;
        }

}

int Sprite::getId()
{
    return spriteId;
}
void Sprite::setCallback(CallbackFunc func) {
    callback = func;
}

void* Sprite::getUserData() {
    return userData;
}

void Sprite::setUserData(void* data) {
    userData = data;
}

const ltex_t* Sprite::getTexture() const
{
    return tex;
};

void Sprite::setTexture(const ltex_t* _tex, int _hframes, int _vframes)
{
    tex = _tex;
    hframes = _hframes;
    vframes = _vframes;

    unsigned char* pi = new unsigned char[tex->width * tex->height * 4];
    ltex_getpixels(_tex, pi);
    pixels = pi;
    size.x = tex->width / hframes;
    size.y = tex->height / vframes;
}

lblend_t Sprite::getBlend() const
{
    return blend;
}

void Sprite::setBlend(lblend_t mode)
{
    blend = mode;
}

float Sprite::getRed() const
{
    return color.r;
}

float Sprite::getGreen() const
{
    return color.g;
}

float Sprite::getBlue() const
{
    return color.b;
}

float Sprite::getAlpha() const
{
    return color.a;
}

void Sprite::setColor(float r, float g, float b, float a)
{
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
}

const Vec2& Sprite::getPosition() const
{
    return position;
}

void Sprite::setPosition(const Vec2& _position)
{
    position = _position;

    //Actualizamos la posicion del collider:
    if (collider)
    {
        if (collisionType == COLLISION_CIRCLE)
        {
            static_cast<CircleCollider*>(collider)->setPosition(position);
        }
        else if (collisionType == COLLISION_RECT)
        {
            Vec2 rectPos = position - (pivot * size);
            static_cast<RectCollider*>(collider)->setPosition(position);
        }
        else if (collisionType == COLLISION_PIXELS)
        {
            Vec2 pixelPos = position - (pivot * size);
            static_cast<PixelsCollider*>(collider)->setPosition(position);
        }
    }
}

float Sprite::getAngle() const
{
    return angle;
}

void Sprite::setAngle(float _angle)
{
    angle = _angle;
}

const Vec2& Sprite::getScale() const
{
    return scale;
}

void Sprite::setScale(const Vec2& _scale)
{
    scale = _scale;

    if (collider)
    {
        if (collisionType == COLLISION_RECT)
        {
            Vec2 rectSize = size.operator*(scale);
            static_cast<RectCollider*>(collider)->setSize(rectSize);
        }

    }
}

Vec2 Sprite::getSize() const
{
    return size;
}

const Vec2& Sprite::getPivot() const
{
    return pivot;
}
void Sprite::setPivot(const Vec2& _pivot)
{
    pivot = _pivot;
}

int Sprite::getHframes() const
{
    return hframes;
}

int Sprite::getVframes() const
{
    return vframes;
}

int Sprite::getFps() const
{
    return fps;
}
void Sprite::setFps(int _fps)
{
    fps = _fps;
}

float Sprite::getCurrentFrame() const
{
    return currentFrame;
}

void Sprite::setCurrentFrame(int frame)
{
    currentFrame = frame;
}

CollisionType Sprite::getCollisionType() const
{
    return collisionType;
}

void Sprite::setCollisionType(CollisionType type)
{
    collisionType = type;

    if (collisionType == COLLISION_CIRCLE)
    {
        collider = new CircleCollider(position, fmax(tex->height, tex->width)/2);
        
    }
    else if (collisionType == COLLISION_RECT)
    {
        Vec2 rectSize = size.operator*(scale);
        collider = new RectCollider(position, rectSize);
    }
    else if (collisionType == COLLISION_PIXELS)
    {
        Vec2 pixelSize(tex->width, tex->height);
        collider = new PixelsCollider(position, size, pixels);
    }
    else if (collisionType == COLLISION_NONE)
    {
        collider = nullptr;
    }
    setPosition(position);
}

Collider* Sprite::getCollider() const
{
    return collider;
}

bool Sprite::collides(const Sprite& other) const
{
    return collider->collides(*other.getCollider());
}



void Sprite::update(float deltaTime)
{

    if (callback) {
        callback(*this, deltaTime);
    }

    float frameDuration = 1.0f / fps;   

    timeAccumulator += deltaTime;

    if (timeAccumulator >= frameDuration) {
        currentFrame += 1.0f;
        timeAccumulator = 0.0f;

        if (currentFrame >= hframes) {
            currentFrame = 0.0f;
        }
    }
}

void Sprite::draw()
{

    float u0 = (1.0f / fps) * currentFrame;
    float u1 = (1.0f / fps) * currentFrame + (1.0f / fps);
    float v0 = 0;
    float v1 = 1;

    lgfx_setblend(blend);
    lgfx_setcolor(color.r, color.g, color.b, color.a);
    ltex_drawrotsized(tex, position.x, position.y, angle /** (3.14 / 180.0f)*/, pivot.x, pivot.y, size.x * scale.x, size.y * scale.y, u0, v0, u1, v1);
}