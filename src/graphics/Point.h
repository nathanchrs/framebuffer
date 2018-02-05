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
		return sqrt(hypot(other.x - x, other.y - y));
	}

	friend Point<T> operator+(const Point<T> &lhs, const Point<T> &rhs);
	friend Point<T> operator-(const Point<T> &lhs, const Point<T> &rhs);
	friend Point<T> operator*(T scalar, const Point<T> &p);
	friend Point<T> operator*(const Point<T> &p, T scalar);
};

template <class T>
Point<T> operator+(const Point<T> &lhs, const Point<T> &rhs) {
	return Point<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <class T>
Point<T> operator-(const Point<T> &lhs, const Point<T> &rhs) {
	return Point<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <class T>
Point<T> operator*(T scalar, const Point<T> &p) {
	return Point<T>(T * p.x, T * p.y);
}

template <class T>
Point<T> operator*(const Point<T> &p, T scalar) {
	return T * p;
}

#endif
