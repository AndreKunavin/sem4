#pragma once

#include <math.h>
#include <random>



struct Vector2
{
    static Vector2 rand() {
        std::random_device rd;
        std::mt19937 gen((rd()));
        std::uniform_real_distribution<> distr(0, 1);
        float x = distr(gen);
        float y = distr(gen);
        return Vector2(x, y);
    }

    Vector2(float m_x, float m_y) {
        x = m_x;
        y = m_y; 
    }
    Vector2(){}
    float len();    
    float x, y;
};
