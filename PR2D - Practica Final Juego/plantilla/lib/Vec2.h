#pragma once 


class Vec2
{
public:
	double x;
	double y;

	Vec2();
	Vec2(double x, double y);

	// Operaciones b�sicas entre vectores
	Vec2 operator+(const Vec2& _v) const;
	Vec2 operator-(const Vec2& _v) const;
	Vec2 operator*(const Vec2& _v) const;
	Vec2 operator/(const Vec2& _v) const;
	void operator+=(const Vec2& _v);
	void operator-=(const Vec2& _v);

	// Operaciones b�sicas con escalares:
	Vec2 operator+(double scalar) const;
	Vec2 operator-(double scalar) const;
	Vec2 operator*(double scalar) const;
	Vec2 operator/(double scalar) const;

	void operator+=(double scalar);
	void operator-=(double scalar);
	void operator*=(double scalar);
	void operator/=(double scalar);

	// Metodos b�sicos
	double dot(const Vec2& _v) const; // Producto escalar
	double length() const;               // Magnitud del vector
	Vec2 normalise() const;              // Vector normalizado
	double distance(const Vec2& _v) const; // Distancia entre vectores
	double angle(const Vec2& _v) const;    // �ngulo entre vectores (en radianes)
};