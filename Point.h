/************************************************************************************************
 File: Point.h
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#ifndef __Ray_Tracer__C_____Point__
#define __Ray_Tracer__C_____Point__

#include <stdio.h>
#include <vector>
#include <cmath>

class Point {
public:
    Point();
    Point(const Point& point);
    Point(float _x, float _y, float _z);
    
    Point adjust(float adjustment, std::vector<float> normal);
    void setX(float _x) { x = _x; }
    void setY(float _y) { y = _y; }
    void setZ(float _z) { z = _z; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    Point adjust(float adjustment);
    Point normalize();
    
    Point operator+(float additive);
    std::vector<float> operator-(const Point& point);
    
private:
    float x, y, z;
};

#endif /* defined(__Ray_Tracer__C_____Point__) */
