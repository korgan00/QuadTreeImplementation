#include "Circle.h"


Circle::Circle()
{
}
Circle::Circle(Point c, string n, int col[], int z) : _center(c), _name(n), _z(z) {
    _color[0] = col[0];
    _color[1] = col[1];
    _color[2] = col[2];
}


Circle::~Circle()
{
}

Point Circle::center() const {
	return _center;
}
int* Circle::color() {
    return _color;
}
string Circle::name() const {
    return _name;
}
int Circle::z() const {
    return _z;
}
bool Circle::IsColliding(const Point& c) const {
	return _center.sqrDist(c) < SQUARED_RADIUS;
}