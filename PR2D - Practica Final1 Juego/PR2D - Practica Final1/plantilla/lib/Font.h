#pragma once  // Previene la inclusión múltiple

#include "Vec2.h"
#include "Color.h"
#include <stb_truetype.h>     
#include <litegfx.h> 


class Font
{
public:
    // Métodos públicos

    ~Font();

    static Font* load(const char* filename, float height);
    float getHeight() const;
    Vec2 getTextSize(const char* text) const;
    void draw(const char* text, const Vec2& pos) const;

    const Vec2& getPosition() const;
    void setPosition(const Vec2& _position);

    void setColor(float r, float g, float b, float a);

private:
    // Variables miembro privadas
    ltex_t* tFontTexture;  // Textura de la fuente
    float fHeight;  // Altura de la fuente
    stbtt_bakedchar* bakedCharData;  // Datos de los glifos horneados
    Vec2 vFontSize;  // Tamaño de la fuente
    Vec2 vFontPos;
    Color fontColor;

    // Constructor y destructor privados
    Font();
};