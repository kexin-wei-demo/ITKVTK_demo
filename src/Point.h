#pragma once

#include <array>
#include <string>

template <typename T>
struct Point
{
    T x;
    T y;
    T z;

    Point(T x, T y, T z) :
        x(x),
        y(y),
        z(z)
    {
    }
    Point() :
        x(0),
        y(0),
        z(0)
    {
    }

    template <typename U>
    bool operator==(const Point<U>& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    Point<T> operator+(const Point<T>& other) const
    {
        return Point<T>(x + other.x, y + other.y, z + other.z);
    }

    Point<T> operator-(const Point<T>& other) const
    {
        return Point<T>(x - other.x, y - other.y, z - other.z);
    }
    std::string toStdString() const { return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z); }

    std::array<double, 3> toArray() const { return { x, y, z }; }
};