
#include "Font.h"
#include "Vec2.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

// Definición del constructor de Font
Font::Font()
{
    tFontTexture = nullptr;
    fHeight = 0.0f;
    bakedCharData = nullptr;
    vFontSize = Vec2(0, 0);
    fontColor.r = 1;
    fontColor.g = 1;
    fontColor.b = 1;
    fontColor.a = 1;
}

// Definición del destructor
Font::~Font()
{
    if (bakedCharData) {
        free(bakedCharData);
    }
    if (tFontTexture) {
        ltex_free(tFontTexture);
    }
}

Font* Font::load(const char* filename, float height)
{
    FILE* file;
    errno_t err = fopen_s(&file, filename, "rb");
    if (err != 0 || !file) {
        printf("No se pudo abrir el archivo de fuente: %s (Error: %d)\n", filename, err);
        return nullptr;
    }

    // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    // Verificar que el archivo no esté vacío
    if (fileSize <= 0) {
        printf("El archivo de fuente está vacío o tiene un tamaño no válido: %s\n", filename);
        fclose(file);
        return nullptr;
    }

    // Regresar al inicio del archivo para leerlo
    fseek(file, 0, SEEK_SET);
    // Asignar memoria para el buffer de la fuente
    unsigned char* fontBuffer = (unsigned char*)malloc(fileSize);
    if (!fontBuffer) {
        printf("No se pudo asignar memoria para la fuente.\n");
        fclose(file);
        return nullptr;
    }
    // Leer el archivo de fuente en el buffer
    size_t bytesRead = fread(fontBuffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        printf("Error al leer el archivo de fuente. Esperados: %ld, Leídos: %zu\n", fileSize, bytesRead);
        free(fontBuffer);  // Liberar memoria en caso de error
        fclose(file);
        return nullptr;
    }
    // Cerrar el archivo
    fclose(file);

    //1 - RENDERIZQAR ALPHAN BUFFER
    int b_w = 1024;  // Ancho
    int b_h = 512;

    unsigned char* bitmap = (unsigned char*)malloc(b_w * b_h);
    if (!bitmap) {
        printf("No se pudo asignar memoria para el mapa de bits.\n");
        free(fontBuffer);
        return nullptr;
    }

    stbtt_bakedchar* bakedCD = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * 96);  // Para los primeros 96 caracteres
    if (!bakedCD) {
        printf("No se pudo asignar memoria para los datos de los glifos.\n");
        free(bitmap);
        free(fontBuffer);
        return nullptr;
    }

    int bakeResult = stbtt_BakeFontBitmap(fontBuffer, 0, height, bitmap, b_w, b_h, 32, 96, bakedCD);
    if (bakeResult == -1) {
        printf("Error al generar el mapa de bits de la fuente.\n");
        free(bakedCD);
        free(bitmap);
        free(fontBuffer);
        return nullptr;
    }

    //2 - CREAR COLOR BUFFER:
    unsigned char* rgbaBuffer = (unsigned char*)malloc(b_w * b_h * 4);  // 4 componentes por píxel
    if (!rgbaBuffer) {
        printf("No se pudo asignar memoria para el buffer RGBA.\n");
        return nullptr;
    }

    for (int i = 0; i < b_w* b_h; ++i) {
        rgbaBuffer[i * 4 + 0] = 255;  // R
        rgbaBuffer[i * 4 + 1] = 255;  // G
        rgbaBuffer[i * 4 + 2] = 255;  // B

        rgbaBuffer[i * 4 + 3] = bitmap[i];  // A
    }

    //3 - CREAR TEXTURA:
    ltex_t* texture = ltex_alloc(b_w, b_h, 0);
    ltex_setpixels(texture, rgbaBuffer);

    free(fontBuffer);
    free(bitmap);
    free(rgbaBuffer);

    Font* newFont = new Font();
    newFont->tFontTexture = texture;
    newFont->fHeight = height;
    newFont->bakedCharData = bakedCD;
    Vec2 size(b_w, b_h);
    newFont->vFontSize = size;

    return newFont;
}

float Font::getHeight() const
{
    return fHeight;
}

const Vec2& Font::getPosition() const
{
    return vFontPos;
}

void Font::setColor(float r, float g, float b, float a)
{
    fontColor.r = r;
    fontColor.g = g;
    fontColor.b = b;
    fontColor.a = a;
}

void Font::setPosition(const Vec2& _position)
{
    vFontPos = _position;
}

Vec2 Font::getTextSize(const char* text) const
{
    float tWidth = 0.0f;
    float tHeight = 0.0f;

    for (const char* c = text; *c; ++c) {
        int charIndex = *c - 32; // Los caracteres ASCII comienzan desde el índice 32
        if (charIndex < 0 || charIndex >= 96) {
            continue;
        }

        stbtt_bakedchar& glyph = bakedCharData[charIndex];

        tWidth += glyph.xadvance;

        if (glyph.yoff < 0) {
            tHeight = fmax(tHeight, -glyph.yoff);
        }
    }

    // La altura del texto será la altura máxima de los glifos
    // Si la fuente es más alta que los glifos individuales, la ajustamos
    tHeight = fmax(tHeight, fHeight);

    return Vec2(tWidth, tHeight);
}

void Font::draw(const char* text, const Vec2& pos) const
{
    Vec2 textSize = getTextSize(text);
    float angle = 0.0f;
    Vec2 pivot(0.5f, 0.5f);

    float xpos = pos.x;
    float ypos = pos.y;

    // Recorremos cada carácter del texto
    for (int i = 0; text[i] != '\0'; ++i)
    {
        int char_index = text[i] - 32;
        if (char_index < 0 || char_index >= 96)
        {
            continue;
        }

        stbtt_aligned_quad q = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        stbtt_GetBakedQuad(bakedCharData, tFontTexture->width, tFontTexture->height, char_index, &xpos, &ypos, &q, 0);

        float width = q.x1 - q.x0;
        float height = q.y1 - q.y0;
        
        lgfx_setblend(BLEND_ADD);
        lgfx_setcolor(fontColor.r, fontColor.g, fontColor.b, fontColor.a);
        ltex_drawrotsized(tFontTexture, xpos, ypos, angle, pivot.x, pivot.y, width, height, q.s0, q.t0, q.s1, q.t1);

    }
}

