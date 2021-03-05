#pragma once

#include <math.h>

struct Vector2
{
    Vector2(float m_x, float m_y) {
        x = m_x;
        y = m_y;  
    }
    Vector2(){}
    Vector2 operator*(const float a)const {
        return Vector2(x * a, y * a);
    }
    Vector2 operator+(const Vector2& other)const {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 operator-(const Vector2& other)const {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 operator/(const float a)const {
        return Vector2(x / a, y / a);
    }
    float len();    
    float x, y;
};