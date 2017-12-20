#pragma once

enum Color { Red, Green, Blue };

// 11
class Point {
private:
	int _x;
	int _y;

public:
	Point(int x = 0, int y = 0) : _x(x), _y(y) {}

	int x() {
		return _x;
	}
	int y() {
		return _y;
	}
	void x(int i) {
		_x = i;
	}
	void y(int j) {
		_y = j;
	}

	int sqrDist(const Point& p) const {
		int xDist = p._x - _x;
		int yDist = p._y - _y;

		return xDist * xDist + yDist * yDist;
	}

	Point operator+(const Point& p) const {
		return Point(_x + p._x, _y + p._y);
	}
	Point& operator+=(Point& p) {
		_x += p._x;
		_y += p._y;
		return *this;
	}
	Point operator-(const Point& p) const {
		return Point(_x - p._x, _y - p._y);
	}
	Point& operator-=(Point& p) {
		_x -= p._x;
		_y -= p._y;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream & Str, Point const & v) {
		// print something from v to str, e.g: Str << v.getX();
		Str << "[" << v._x << "," << v._y << "]";
		return Str;
	}

	static bool Point::IsAtLeft(const Point& a, const Point& b, const Point& c) {
		return Scalar(a, b, c) > 0;
	}

	static float Scalar(const Point& a, const Point& b, const Point& c) {
		Point ac = a - c;
		Point ab = a - b;
		float acMod = (float)sqrt(ac.x()*ac.x() + ac.y()*ac.y());
		float abMod = (float)sqrt(ab.x()*ab.x() + ab.y()*ab.y());

		float acx = ac._x / acMod;
		float acy = ac._y / acMod;
		float abx = ab._x / abMod;
		float aby = ab._y / abMod;

		return abx * acy - acx * aby;
	}

	
};