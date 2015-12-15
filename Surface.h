/************************************************************************************************
 File: Surface.h
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#ifndef __Ray_Tracer__C_____Surface__
#define __Ray_Tracer__C_____Surface__

#include <stdio.h>
#include "Assets.h"
#include "Ray.h"
#include "Color.h"

class Surface {
public:
    enum SurfaceType { SPHERE, ELLIPSOID, INFINITE_CYLINDER, INFINITE_PLANE, NONE };
    
    Surface() {
        surfaceType = SurfaceType::NONE;
        
        reflectivity = 0.0;
        
        ambienceCoefficients = Color(0.0, 0.0, 0.0);
        diffuseCoefficients = Color(0.0, 0.0, 0.0);
        specularCoefficients = Color(0.0, 0.0, 0.0);
    }
    
    Surface(SurfaceType surface, Color amb, Color diff, Color spec, float reflection) {
        surfaceType = surface;
        
        reflectivity = reflection;
        
        ambienceCoefficients = amb;
        diffuseCoefficients = diff;
        specularCoefficients = spec;
    }
    
    virtual Surface* Clone() = 0;   /* Virtual copy constructor */
    
    /* Returns -1 if no intersection or intersection behind eye, otherwise, returns 
     the magnitude along given ray where intersection occurs and updates 'normal' 
     parameter with the value of the normal ray that extends from the surface's face */
    virtual float intersect(Ray ray, Ray& normal) = 0;
    virtual void setReflectivity(float reflect) { reflectivity = reflect; }
    virtual void setAmbientCoefficients(Color amb) { ambienceCoefficients = amb; }
    virtual void setDiffuseCoefficients(Color diff) { diffuseCoefficients = diff; }
    virtual void setSpecularCoefficients(Color spec) { specularCoefficients = spec; }
    virtual float getReflectivity() { return reflectivity; }
    virtual Color getAmbientCoefficients() { return ambienceCoefficients; }
    virtual Color getDiffuseCoefficients() { return diffuseCoefficients; }
    virtual Color getSpecularCoefficients() { return specularCoefficients; }
    
protected:
    /* For light calculations */
    Color ambienceCoefficients;
    Color diffuseCoefficients;
    Color specularCoefficients;
    float reflectivity;
    SurfaceType surfaceType;
};



class Sphere : public Surface {
public:
    Sphere();
    Sphere(Point sphereCenter, float sphereRadius, Color amb, Color diff, Color spec, float reflection);
    virtual Surface* Clone() { return new Sphere(*this); }  /* Virtual copy constructor */
    
    float intersect(Ray ray, Ray& normal);
    
private:
    Point center;
    float radius;
};



class Ellipsoid : public Surface {
public:
    Ellipsoid();
    Ellipsoid(Point ellipsoidCenter, Point axes, Color amb, Color diff, Color spec, float reflection);
    virtual Surface* Clone() { return new Ellipsoid(*this); }  /* Virtual copy constructor */
    
    float intersect(Ray ray, Ray& normal);
    
private:
    Point center;
    float semiAxes[3];    /* Lengths of semi-axes a, b, c */
};



class InfiniteCylinder : public Surface {
public:
    InfiniteCylinder();
    
    //InfiniteCylinder(float centerCoords[3], float rad, float axisVec[3], float amb[3], float diff[3], float spec[3], float reflectivity);
    InfiniteCylinder(Point cylinderCenter, float rad, float ht, float axisVec[3], Color amb, Color diff, Color spec, float reflectivity);
    virtual Surface* Clone() { return new InfiniteCylinder(*this); }  /* Virtual copy constructor */
    
    float intersect(Ray ray, Ray& normal);
    
private:
    Point center;
    float axisDirection[3];
    float radius;
    float height;
};



class InfinitePlane : public Surface {
public:
    InfinitePlane();
    InfinitePlane(Point planeCenter, Ray planeNormal, Color amb, Color diff, Color spec, float reflection);
    virtual Surface* Clone() { return new InfinitePlane(*this); }  /* Virtual copy constructor */
    
    /* Returns -1 if no intersection or ray is parallel to plane, otherwise, returns value of
     intersection point and normalized normal at point on surface */
    float intersect(Ray ray, Ray& normal);
    
private:
    Point point;
    Ray normal;
};

#endif /* defined(__Ray_Tracer__C_____Surface__) */
