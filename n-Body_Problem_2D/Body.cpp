#include "stdafx.h"
#include "Body.h"


Body::Body(){
}

Body::Body(Vector2D pos, Vector2D bew, double anz) {
	this->position = pos;
	this->bewegungsRichtung = bew;
	this->anziehung = anz;
}

Body::~Body(){
}
