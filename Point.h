#pragma once
#include <cmath>
#include <istream>

class Point {
  public:
    Point() = default;
    inline Point(double x, double y) : x{x}, y{y} {};
    double x{};
    double y{};

    //Без friend внешняя функция не может обращаться к private/protected членам, и попытка это сделать приведёт к ошибке компиляции.
    friend std::istream& operator>>(std::istream& stream,Point& point) {
    stream >> point.x >> point.y;
    return stream;
}
};

inline Point operator-(const Point& lhs, const Point& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Point operator+(const Point& lhs, const Point& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Point operator*(const Point& p, double scalar) {
    return {p.x * scalar, p.y * scalar};
}

inline Point operator/(const Point& p, double scalar) {
    return {p.x / scalar, p.y / scalar};
}

inline double distance2(const Point& lhs, const Point& rhs) {
    const Point delta = lhs - rhs;
    return delta.x * delta.x + delta.y * delta.y;
}
