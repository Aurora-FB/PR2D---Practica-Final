#pragma once

struct Color {
	float r;
	float g;
	float b;
	float a;

	// Constructor para inicializar el color
	Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
		: r(red), g(green), b(blue), a(alpha) {
	}
};
