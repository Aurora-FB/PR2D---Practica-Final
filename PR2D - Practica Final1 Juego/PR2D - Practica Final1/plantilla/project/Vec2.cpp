
#include "Vec2.h"
#include <cmath>
#include <stdexcept>

Vec2::Vec2()
{
	x = 0;
	y = 0;
}

Vec2::Vec2(double _x, double _y)
{
	x = _x;
	y = _y;
}

// Operaciones básicas entre vectores
Vec2 Vec2::operator+(const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
	return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(const Vec2& other) const {
	return Vec2(x * other.x, y * other.y);
}

Vec2 Vec2::operator/(const Vec2& other) const {
	return Vec2(x / other.x, y / other.y);
}

void Vec2::operator+=(const Vec2& other) {
	x += other.x;
	y += other.y;
}

void Vec2::operator-=(const Vec2& other) {
	x -= other.x;
	y -= other.y;
}

// Operaciones básicas con escalares:

Vec2 Vec2::operator+(double scalar) const {
	return Vec2(x + scalar, y + scalar);
}

Vec2 Vec2::operator-(double scalar) const {
	return Vec2(x - scalar, y - scalar);
}

Vec2 Vec2::operator*(double scalar) const {
	return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(double scalar) const {
	if (scalar == 0.0) {
		throw std::runtime_error("Division by zero in Vec2::operator/");
	}
	return Vec2(x / scalar, y / scalar);
}

void Vec2::operator+=(double scalar) {
	x += scalar;
	y += scalar;
}

void Vec2::operator-=(double scalar) {
	x -= scalar;
	y -= scalar;
}

void Vec2::operator*=(double scalar) {
	x *= scalar;
	y *= scalar;
}

void Vec2::operator/=(double scalar)
{
	if (scalar == 0.0) {
		throw std::runtime_error("Division by zero in Vec2::operator/=");
	}
	x /= scalar;
	y /= scalar;
}

// Metodos básicos
double Vec2::dot(const Vec2& _v) const
{
	double fDot_result = (x * _v.x) + (y * _v.y);
	return fDot_result;
}

double Vec2::length() const
{
	float length = std::sqrt(x * x + y * y);
	return length;
}

Vec2 Vec2::normalise() const
{
	Vec2 vNew;
	double dVLength = length();

	if (0.0f == dVLength) {
		vNew.x = 0;
		vNew.y = 0;
		return vNew;
	}

	vNew = operator/(dVLength);
	return vNew;
}

double Vec2::distance(const Vec2& _v) const
{
	double distance = std::sqrt((_v.x - x) * (_v.x - x) + (_v.y - y) * (_v.y - y));
	return distance;
}

double Vec2::angle(const Vec2& _v) const
{
	double dotProduct = dot(_v);
	double lengthsProduct = length() * _v.length();

	if (lengthsProduct == 0) {
		return 0;
	}

	double angle = std::acos(dotProduct / lengthsProduct);

	return angle;
}




