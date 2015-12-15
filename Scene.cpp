/************************************************************************************************
 File: Scene.cpp
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#include "Scene.h"

Scene::Scene()
{
    ambientIntensity.setRGB(0.5, 0.5, 0.5);
}

Scene::Scene(const Scene& scene)
{
    ambientIntensity = scene.getAmbientIntensity();
    surfaces = scene.getSurfaces();
    lights = scene.getLights();
}

Scene::Scene(Color ambientLightIntensity)
{
    ambientIntensity = ambientLightIntensity;
}

void Scene::addLight(Light light)
{
    lights.push_back( light );
}

void Scene::addLight(Point lightPosition, Color lightIntensity)
{
    lights.push_back( Light(lightPosition, lightIntensity) );
}

void Scene::addSphere(Sphere* sphere)
{
    surfaces.push_back(sphere);
}

void Scene::addEllipsoid(Ellipsoid* ellipsoid)
{
    surfaces.push_back(ellipsoid);
}

void Scene::addInfinitePlane(InfinitePlane* plane)
{
    surfaces.push_back(plane);
}

void Scene::addInfiniteCylinder(InfiniteCylinder* cylinder)
{
    surfaces.push_back(cylinder);
}