/************************************************************************************************
 File: Assets.h
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#ifndef Ray_Tracer__C____Assets_h
#define Ray_Tracer__C____Assets_h

#include <vector>
#include <cmath>
#include "Point.h"
#include "Ray.h"

/* Clears framebuffer to black */
void clearFramebuffer();

/* Sets pixel x,y to the color RGB */
void setFramebuffer(int x, int y, float R, float G, float B);

inline float dotProduct(Point p1, Point p2) {
    return ( (p1.getX() * p2.getX()) + (p1.getY() * p2.getY()) + (p1.getZ() * p2.getZ()) );
}

inline float dotProduct(Point p, std::vector<float> v) {
    return ( (p.getX() * v[0]) + (p.getY() * v[1]) + (p.getZ() * v[2]) );
}

inline float dotProduct(std::vector<float> v1, std::vector<float> v2) {
    return ( (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]) );
}

/* Order of the given Point arguments is important */
inline Point crossProduct(Point p1, Point p2) {
    return Point(p1.getY() * p2.getZ() - p1.getZ() * p2.getY(),
                 p1.getX() * p2.getZ() - p1.getZ() * p2.getX(),
                 p1.getX() * p2.getY() - p1.getY() * p2.getX());
}

inline Point projection(Point p1, Point p2, Point mod) {
    float scalar = dotProduct(p1, p2) / dotProduct(p2, p2);
    
    return Point(scalar * mod.getX(), scalar * mod.getY(), scalar * mod.getZ());
}

#endif
