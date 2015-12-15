/************************************************************************************************
 File: Scene.h
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#ifndef __Ray_Tracer__C_____Scene__
#define __Ray_Tracer__C_____Scene__

#include <stdio.h>
#include "Surface.h"
#include "Color.h"
#include "Light.h"

class Scene {
public:
    Scene();
    Scene(const Scene& scene);
    Scene(Color ambientLightIntensity);
    
    void addLight(Light light);
    void addLight(Point lightPosition, Color lightIntensity);
    void addSphere(Sphere* sphere);
    void addEllipsoid(Ellipsoid* ellipsoid);
    void addInfinitePlane(InfinitePlane* plane);
    void addInfiniteCylinder(InfiniteCylinder* cylinder);
    
    Color getAmbientIntensity() const { return ambientIntensity; }
    std::vector<Light> getLights() const { return lights; }
    std::vector<Surface*> getSurfaces() const { return surfaces; }
    
private:
    Color ambientIntensity;
    std::vector<Surface*> surfaces;
    
    /* Holds all light information for scene except ambient light which is light-independent */
    std::vector<Light> lights;
};


#endif /* defined(__Ray_Tracer__C_____Scene__) */
