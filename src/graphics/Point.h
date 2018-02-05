#ifndef POINT_H
#define POINT_H

#include <cmath>

template<class T>
class Point {
public:
	Point(T x, T y) : x(x), y(y) {}
	Point() : x(0), y(0) {}

	T x;
	T y;

	T distanceTo(const Point &other) {
		return hypot(other.x - x, other.y - y);
	}

	Point<double> toDouble() const {
		return Point<double>(x, y);
	}

	Point<long> toLong() const {
		return Point<long>(x, y);
	}

	friend Point<T> operator+(const Point<T> &lhs, const Point<T> &rhs) {
		return Point<T>(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	friend Point<T> operator-(const Point<T> &lhs, const Point<T> &rhs) {
		return Point<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	friend Point<T> operator*(T scalar, const Point<T> &p) {
		return Point<T>(scalar * p.x, scalar * p.y);
	}

	friend Point<T> operator*(const Point<T> &p, T scalar) {
		return scalar * p;
	}
};

#endif
