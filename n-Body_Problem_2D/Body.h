#pragma once
#include "Vector2D.h"
#include "stdafx.h"
class Body
{
public:
	Body();
	Body(Vector2D position, Vector2D bewegungsRichtung, double anziehung);
	~Body();
	Vector2D position;
	Vector2D positionAlt;
	Vector2D positionAlt2;
	Vector2D bewegungsRichtung;
	double anziehung;
};

