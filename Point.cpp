/************************************************************************************************
 File: Point.cpp
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#include "Point.h"

Point::Point()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Point::Point(const Point& point)
{
    x = point.getX();
    y = point.getY();
    z = point.getZ();
}

Point::Point(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

Point Point::adjust(float adjustment, std::vector<float> normal)
{
    Point adjustedPoint;
    
    if (normal[0] < 0) {
        adjustedPoint.setX(x - adjustment);
    } else {
        adjustedPoint.setX(x + adjustment);
    }
    
    if (normal[1] < 0) {
        adjustedPoint.setY(y - adjustment);
    } else {
        adjustedPoint.setY(y + adjustment);
    }
    
    if (normal[2] < 0) {
        adjustedPoint.setZ(z - adjustment);
    } else {
        adjustedPoint.setZ(z + adjustment);
    }
    
    return adjustedPoint;
}

Point Point::normalize()
{
    float norm = sqrtf( (x * x) + (y * y) + (z * z) );
    
    return Point( x/norm, y/norm, z/norm );
}

Point Point::operator+(float additive)
{
    return Point(getX() + additive, getY() + additive, getZ() + additive);
}

std::vector<float> Point::operator-(const Point& point)
{
    std::vector<float> resultVector;
    
    resultVector.push_back(this->getX() - point.getX());
    resultVector.push_back(this->getY() - point.getY());
    resultVector.push_back(this->getZ() - point.getZ());
    
    return resultVector;
}