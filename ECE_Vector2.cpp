/*
Author: jmcmath3
Class: ECE4122
Last Date Modified: 12/01/20
Description:
    ECE_Vector2 definitions
*/

#include <cmath>

#include "ECE_Vector2.h"


ECE_Vector2 ECE_Vector2::normal() const { return operator/(ECE_Vector2(x, y), magnitude()); }
double ECE_Vector2::magnitude() const { return sqrt(magnitude_squared()); }
double ECE_Vector2::magnitude_squared() const { return (pow(x, 2) + pow(y, 2)); }
ECE_Vector2 ECE_Vector2::as_int() const { return ECE_Vector2((int) x, (int) y); }


ECE_Vector2 ECE_Vector2::operator-() const { return ECE_Vector2(-x, -y); }
ECE_Vector2& ECE_Vector2::operator+=(const ECE_Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
ECE_Vector2& ECE_Vector2::operator-=(const ECE_Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
ECE_Vector2& ECE_Vector2::operator*=(const double& scalar) { x *= scalar; y *= scalar; return *this; }
ECE_Vector2& ECE_Vector2::operator/=(const double& scalar) { x /= scalar; y /= scalar; return *this; }
