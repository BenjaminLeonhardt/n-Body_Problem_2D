#include "stdafx.h"
#include "Vector2D.h"


Vector2D::Vector2D(){
}

Vector2D::Vector2D(double x, double y) {
	this->x = x;
	this->y = y;
}

Vector2D::~Vector2D(){
}

Vector2D& Vector2D::operator+=(const Vector2D &v2) {
	this->x += v2.x;
	this->y += v2.y;
	return *this;
};

Vector2D& Vector2D::operator-=(const Vector2D &v2) {
	this->x -= v2.x;
	this->y -= v2.y;
	return *this;
};

Vector2D& Vector2D::operator+(const Vector2D &v2) {
	return Vector2D(this->x + v2.x, this->y + v2.y);
}

Vector2D& Vector2D::operator-(const Vector2D &v2) {
	return Vector2D(this->x - v2.x, this->y - v2.y);
}

Vector2D& Vector2D::operator*(const Vector2D &v2) {
	return Vector2D(this->x * v2.x, this->y * v2.y);
}

Vector2D& Vector2D::operator/(const Vector2D &v2) {
	return Vector2D(this->x / v2.x, this->y / v2.y);
}