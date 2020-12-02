/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Vector2 declarations
*/

#pragma once

#include <cmath>

// A class to represent 2-D vectors.
class ECE_Vector2
{
public:

    ECE_Vector2() : x(0), y(0) {};
    ECE_Vector2(double x, double y) : x(x), y(y) {};
    ECE_Vector2(int x, int y) : x(x), y(y) {};
    ECE_Vector2(const ECE_Vector2& v) : x(v.x), y(v.y) {};
    ~ECE_Vector2() {};

    // coordinates
    double x, y;

    ECE_Vector2 normal() const;
    double magnitude() const;
    double magnitude_squared() const;
    ECE_Vector2 as_int() const;

    ECE_Vector2 operator-() const;
    ECE_Vector2& operator+=(const ECE_Vector2& rhs);
    ECE_Vector2& operator-=(const ECE_Vector2& rhs);
    ECE_Vector2& operator*=(const double& scalar);
    ECE_Vector2& operator/=(const double& scalar);

    friend ECE_Vector2 operator+(ECE_Vector2 lhs, const ECE_Vector2& rhs) { lhs += rhs; return lhs; };
    friend ECE_Vector2 operator-(ECE_Vector2 lhs, const ECE_Vector2& rhs) { lhs -= rhs; return lhs; };
    friend ECE_Vector2 operator*(ECE_Vector2 lhs, const double& scalar) { lhs *= scalar; return lhs; };
    friend ECE_Vector2 operator/(ECE_Vector2 lhs, const double& scalar) { lhs /= scalar; return lhs; };
    
    inline friend bool operator==(const ECE_Vector2& lhs, const ECE_Vector2& rhs) { return (std::abs(lhs.x - rhs.x) < ECE_Vector2::THRESH && std::abs(lhs.y - rhs.y) < ECE_Vector2::THRESH); };
    inline friend bool operator!=(const ECE_Vector2& lhs, const ECE_Vector2& rhs) { return !(lhs == rhs); };

private:

    // a threshold value for comparing doubles.
    // values differing by less than the threshold value are considered the same
    static constexpr double THRESH = 0.000001;

};
