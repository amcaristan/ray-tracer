/************************************************************************************************
 File: Light.h
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#ifndef __Ray_Tracer__C_____Light__
#define __Ray_Tracer__C_____Light__

#include <stdio.h>
#include "Point.h"
#include "Color.h"

class Light {
public:
    
    Light();
    Light(const Light& light);
    Light(Point _position, Color _rgbIntensity);
    
    void setPosition(Point newPosition) { position = newPosition; };
    void setRGBIntensity(Color newRGBIntensity) { rgbIntensity = newRGBIntensity; };
    Point getPosition() const { return position; };
    Color getRGBIntensity() const { return rgbIntensity; };
    
private:
    Point position;
    Color rgbIntensity;
};

#endif /* defined(__Ray_Tracer__C_____Light__) */
