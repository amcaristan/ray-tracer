/************************************************************************************************
 File: Surface.cpp
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#include "Surface.h"

/* Sphere */
Sphere::Sphere()
: Surface(SurfaceType::SPHERE, Color(0.1, 0.0, 0.0), Color(0.7, 0.0, 0.0), Color(0.5, 0.5, 0.5), 0.0)
{
    center = Point(0.0, 0.0, 0.0);
    radius = 1.0;
}

Sphere::Sphere(Point sphereCenter, float sphereRadius, Color amb, Color diff, Color spec, float reflection)
: Surface(SurfaceType::SPHERE, amb, diff, spec, reflection)
{
    center = sphereCenter;
    radius = sphereRadius;
}

float Sphere::intersect(Ray ray, Ray& normal)
{
    /* x(1,2) = [(-b (+,-) sqrt(determinant)) / 2a] */
    float a, b, c, determinant, intersection, x1, x2;
    
    std::vector<float> rayUnitDirectionVector = ray.normalize();
    Point rayStartPoint = ray.getStartPoint();
    
    a = powf(rayUnitDirectionVector[0], 2.00) + powf(rayUnitDirectionVector[1], 2.00) + powf(rayUnitDirectionVector[2], 2.00);
    
    b = 2.00 * ((rayUnitDirectionVector[0] * (rayStartPoint.getX() - this->center.getX())) +
                (rayUnitDirectionVector[1] * (rayStartPoint.getY() - this->center.getY())) +
                (rayUnitDirectionVector[2] * (rayStartPoint.getZ() - this->center.getZ())));
    
    c = powf(this->center.getX(), 2.00) + powf(this->center.getY(), 2.00) + powf(this->center.getZ(), 2.00) +
        powf(rayStartPoint.getX(), 2.00) + powf(rayStartPoint.getY(), 2.00) + powf(rayStartPoint.getZ(), 2.00) -
        (2 * (this->center.getX() * rayStartPoint.getX() + this->center.getY() * rayStartPoint.getY() + this->center.getZ() * rayStartPoint.getZ())) -
        powf(this->radius, 2.00);
    
    determinant = powf(b, 2.00) - (4 * a * c);
    float absDeterminant = fabs(determinant);
    
    if (determinant < 0 && absDeterminant > 0.0001) {    /* No intersection */
        return -1;
    } else {    /* Ray passes through sphere */
        x1 = ( -b + sqrtf(absDeterminant) ) / (2 * a);
        x2 = ( -b - sqrtf(absDeterminant) ) / (2 * a);
    }
    
    /* Set intersection to closest intersection point along ray */
    intersection = (x2 > 0) ? x2 : x1;
    
    /* Prepare a ray representing the normal of the surface at the intersection point */
    Point newStartPoint(rayStartPoint.getX() + rayUnitDirectionVector[0] * intersection,
                         rayStartPoint.getY() + rayUnitDirectionVector[1] * intersection,
                         rayStartPoint.getZ() + rayUnitDirectionVector[2] * intersection);
    
    Ray tempRay = Ray(this->center, newStartPoint);
    std::vector<float> tempUnitDirectionVector = tempRay.normalize();
    
    /* Compensate for floating-point error by adding 0.0001 to each 3D coordinate */
    newStartPoint = newStartPoint.adjust(0.0001, tempUnitDirectionVector);
    
    Point newEndPoint(newStartPoint.getX() + tempUnitDirectionVector[0],
                       newStartPoint.getY() + tempUnitDirectionVector[1],
                       newStartPoint.getZ() + tempUnitDirectionVector[2]);
    
    normal = Ray(newStartPoint, newEndPoint);

    return intersection;
}



/* Infinite Plane */
InfinitePlane::InfinitePlane()
: Surface(SurfaceType::INFINITE_PLANE, Color(0.1, 0.0, 0.0), Color(0.7, 0.0, 0.0), Color(0.5, 0.5, 0.5), 0.0)
{
    point = Point(0, -1.0, 3.0);
    normal = Ray(Point(0.0, -1.0, 3.0), Point(0.0, 0.0, 2.9999));
}

InfinitePlane::InfinitePlane(Point planeCenter, Ray planeNormal, Color amb, Color diff, Color spec, float reflectivity)
: Surface(SurfaceType::INFINITE_PLANE, amb, diff, spec, reflectivity)
{
    point = planeCenter;
    normal = planeNormal;
}

float InfinitePlane::intersect(Ray ray, Ray& retNormal) {
    float numerator, denominator, t, d;
    
    std::vector<float> normalVector = this->normal.normalize();
    
    denominator = dotProduct(ray.normalize(), normalVector);
    
    if ( denominator == 0 ) { /* Ray is parallel to plane */
        return -1;
    }
    
    d = -(normalVector[0] * point.getX() + normalVector[1] * point.getY() + normalVector[2] * point.getZ());
    numerator = d - dotProduct(ray.getStartPoint(), this->normal.normalize());
    
    if ( (t = numerator/denominator) < 1 || t == 0 ) {
        return -1;
    } else {
        if (denominator > 0) {  /* Normal of plane pointing away from viewer, so reverse  the directon vector */
            
            retNormal = Ray(normal.getDirectionPoint(), normal.getStartPoint());
            
        } else {    /* Normal of plane pointing towards viewer */
            
            retNormal = this->normal;
            
        }
        return t;
    }
}




