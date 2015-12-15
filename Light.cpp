/************************************************************************************************
 File: Light.cpp
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#include "Light.h"

Light::Light()
{
    position = Point();
    rgbIntensity = Color();
}

Light::Light(const Light& light)
{
    position = light.getPosition();
    rgbIntensity = light.getRGBIntensity();
}

Light::Light(Point _position, Color _rgbIntensity)
{
    position = _position;
    rgbIntensity = _rgbIntensity;
}

