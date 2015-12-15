/************************************************************************************************
 File: Ray.cpp
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#include "Ray.h"

Ray::Ray ()
{
    startPoint.setX(0.0);
    startPoint.setY(0.0);
    startPoint.setZ(0.0);
    
    directionPoint.setX(0.0);
    directionPoint.setY(0.0);
    directionPoint.setZ(0.0);
    
    unitDirectionVector.push_back(0.0);
    unitDirectionVector.push_back(0.0);
    unitDirectionVector.push_back(0.0);
}

Ray::Ray (const Ray& ray)
{
    startPoint = ray.getStartPoint();
    directionPoint = ray.getDirectionPoint();
    unitDirectionVector = directionPoint - startPoint;
    
    float temp = 0.0;
    
    for (int i = 0; i < unitDirectionVector.size(); i++) {
        temp += pow(unitDirectionVector[i], 2);
    }
    
    temp = sqrtf(temp);
    
    for (int i = 0; i < unitDirectionVector.size(); i++) {
        unitDirectionVector[i] /= temp;
    }
}

Ray::Ray(Point _startPoint, Point _directionPoint)
{
    startPoint = _startPoint;
    directionPoint = _directionPoint;
    unitDirectionVector = directionPoint - startPoint;
    
    float temp = 0.0;
    
    for (int i = 0; i < unitDirectionVector.size(); i++) {
        temp += pow(unitDirectionVector[i], 2);
    }
    
    temp = sqrtf(temp);
    
    for (int i = 0; i < unitDirectionVector.size(); i++) {
        unitDirectionVector[i] /= temp;
    }
}

Ray::Ray(Point _startPoint, std::vector<float> _unitDirectionVector)
{
    startPoint = _startPoint;
    unitDirectionVector = _unitDirectionVector;
    directionPoint = getRayPoint(1.0);
}

std::vector<float> Ray::normalize()
{
    return unitDirectionVector;
}

Point Ray::getRayPoint(float magnitude)
{
    return Point(startPoint.getX() + (unitDirectionVector[0] * magnitude),
                 startPoint.getY() + (unitDirectionVector[1] * magnitude),
                 startPoint.getZ() + (unitDirectionVector[2] * magnitude));
}

Ray Ray::getReversedRay()
{
    return Ray(directionPoint, startPoint);
}

/* Assumes the ray points in the direction of the intersection with the normal and therefore must
    be reversed to be substituted for 'V' in the equation used to find the reflected ray: 
    (R = 2(V.N)N - V). 'V' and 'normal' must be normalized, this function takeds care of that. */
Ray Ray::getReflectedRay(Ray normal)
{
    std::vector<float> incident = this->unitDirectionVector;
    
    for (int i = 0; i < incident.size(); i++) {
        incident[i] *= -1;
    }
    
    std::vector<float> normalVector = normal.normalize();
    float mult = 2.0 * dotProduct(normalVector, incident);
    
    std::vector<float> reflected;
    
    for (int i = 0; i < normalVector.size(); i++) {
        reflected.push_back( (mult * normalVector[i]) - incident[i] );
    }

    return Ray(normal.getStartPoint(), reflected);
}

