/************************************************************************************************
 File: Ray.h
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#ifndef __Ray_Tracer__C_____Ray__
#define __Ray_Tracer__C_____Ray__

#include <stdio.h>
#include "Assets.h"
#include "Point.h"

class Ray {
public:
    Ray();
    Ray(const Ray& ray);
    Ray(Point endPoint, Point direction);
    Ray(Point _startPoint, std::vector<float> _unitDirectionVector);

    std::vector<float> normalize();   /* Returns this ray's unitDirectionVector */
    Point getStartPoint() const { return startPoint; };
    Point getDirectionPoint() const { return directionPoint; };
    Point getRayPoint(float magnitude); /* Returns point along ray */
    Ray getReversedRay();
    Ray getReflectedRay(Ray normal);
    
private:
    Point startPoint, directionPoint;
    std::vector<float> unitDirectionVector;
};

#endif /* defined(__Ray_Tracer__C_____Ray__) */
