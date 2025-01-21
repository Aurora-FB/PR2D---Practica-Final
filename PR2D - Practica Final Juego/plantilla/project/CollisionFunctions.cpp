
#include "CollisionFunctions.h"
#include "Vec2.h"

#include <cstdint>
#include <cmath>


bool checkCircleCircle(const Vec2& pos1, float radius1, const Vec2& pos2, float radius2) 
{
	//Distancia de sus centros menor que la suma de sus radios:
	double distance = pos1.distance(pos2);
	float sumRadius = radius1 + radius2;

	if (distance < sumRadius)
	{
		return true;
	}

	return false;
}

bool checkCircleRect(const Vec2& circlePos, float circleRadius, const Vec2& rectPos, const Vec2& rectSize)
{
	
	int closestX = fmax(rectPos.x - rectSize.x / 2, fmin(circlePos.x, rectPos.x + rectSize.x / 2));
	int closestY = fmax(rectPos.y - rectSize.y / 2, fmin(circlePos.y, rectPos.y + rectSize.y / 2));

	Vec2 closest(closestX, closestY);
	double distance = closest.distance(circlePos);

	return distance < circleRadius;
}

bool checkRectRect(const Vec2& rectPos1, const Vec2& rectSize1, const Vec2& rectPos2, const Vec2& rectSize2) 
{
	bool overlapX = (rectPos1.x < rectPos2.x + rectSize2.x / 2) && (rectPos1.x + rectSize1.x/2 > rectPos2.x);
	bool overlapY = (rectPos1.y < rectPos2.y + rectSize2.y / 2) && (rectPos1.y + rectSize1.y / 2 > rectPos2.y);

	return overlapX && overlapY;
}

bool checkCirclePixels(const Vec2& circlePos, float circleRadius, const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) 
{
	// Definir los límites del área de solapamiento.
	float minX = fmax(circlePos.x - circleRadius, pixelsPos.x);
	float maxX = fmin(circlePos.x + circleRadius, pixelsPos.x + pixelsSize.x);
	float minY = fmax(circlePos.y - circleRadius, pixelsPos.y);
	float maxY = fmin(circlePos.y + circleRadius, pixelsPos.y + pixelsSize.y);

	// Recorrer los píxeles en el área de solapamiento.
	for (float y = minY; y < maxY; ++y) {
		for (float x = minX; x < maxX; ++x) {
			int pixelIndex = ((int)y - (int)pixelsPos.y) * (int)pixelsSize.x + ((int)x - (int)pixelsPos.x);

			uint8_t alpha = pixels[pixelIndex * 4 + 3];

			
			if (alpha > 0) {
				Vec2 d(x, y);
				double distance = d.distance(circlePos);
				if (distance < circleRadius) {

					return true;
				}
			}
		}
	}
	return false;
}

bool checkPixelsPixels(const Vec2& pixelsPos1, const Vec2& pixelsSize1, const uint8_t* pixels1, const Vec2& pixelsPos2, const Vec2& pixelsSize2, const uint8_t* pixels2) 
{
	/*Este modo de colisión comprueba la intersección entre dos imágenes, determinando que ha habido colisión únicamente si píxeles no transparentes
	se han solapado en ambas imágenes.
		• En primer lugar, debemos encontrar qué área de las imágenes se solapa.
		• Comparamos píxel a píxel dichas áreas en ambas imágenes.Si coinciden dos píxeles no transparentes, hay colisión.*/

	float minX = fmax(pixelsPos1.x, pixelsPos2.x);
	float maxX = fmin(pixelsPos1.x + pixelsSize1.x, pixelsPos2.x + pixelsSize2.x);
	float minY = fmax(pixelsPos1.y, pixelsPos2.y);
	float maxY = fmin(pixelsPos1.y + pixelsSize1.y, pixelsPos2.y + pixelsSize2.y);

	// Recorrer los píxeles en el área de solapamiento.
	for (float y = minY; y < maxY; ++y) {
		for (float x = minX; x < maxX; ++x) {
			int pixelIndex1 = ((int)y - (int)pixelsPos1.y) * (int)pixelsSize1.x + ((int)x - (int)pixelsPos1.x);
			int pixelIndex2 = ((int)y - (int)pixelsPos2.y) * (int)pixelsSize2.x + ((int)x - (int)pixelsPos2.x);

			uint8_t alpha1 = pixels1[pixelIndex1 * 4 + 3];
			uint8_t alpha2 = pixels1[pixelIndex2 * 4 + 3];

			if (alpha1 > 0 && alpha2 > 0) {
				return true;
			}
		}
	}
	return false;
}

bool checkPixelsRect(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels, const Vec2& rectPos, const Vec2& rectSize) 
{
	/*Este modo se calcula igual que en el caso de colisión píxel - píxel, pero considerando que el rectángulo completo es colisionable.*/

	float minX = fmax(rectPos.x, pixelsPos.x);
	float maxX = fmin(rectPos.x + rectSize.x, pixelsPos.x + pixelsSize.x);
	float minY = fmax(rectPos.y, pixelsPos.y);
	float maxY = fmin(rectPos.y + rectSize.y, pixelsPos.y + pixelsSize.y);

	// Recorrer los píxeles en el área de solapamiento.
	for (float y = minY; y < maxY; ++y) {
		for (float x = minX; x < maxX; ++x) {
			int pixelIndex = ((int)y - (int)pixelsPos.y) * (int)pixelsSize.x + ((int)x - (int)pixelsPos.x);

			uint8_t alpha = pixels[pixelIndex * 4 + 3];

			if (alpha > 0) {
				return true;
			}
		}
	}

	return false;
}
