#pragma once
#include <string>
#include "Point.h"
using namespace std;

#ifndef CIRCLES_RADIUS
#define CIRCLES_RADIUS 100
#endif 
#ifndef SQUARED_RADIUS
#define SQUARED_RADIUS CIRCLES_RADIUS*CIRCLES_RADIUS
#endif 


class Circle
{
private:
	Point _center;
	string _name;
    int _color[3];

public:
    Circle();
    Circle(Point, string, int[]);
	~Circle();

    Point center() const;
    int* color();
    string name() const;

	//bool IsColliding(Circle);
	bool IsColliding(const Point&) const;
};

