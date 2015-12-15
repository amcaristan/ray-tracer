/************************************************************************************************
 File: Color.h
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#ifndef __Ray_Tracer__C_____Color__
#define __Ray_Tracer__C_____Color__

#include <stdio.h>

class Color {
public:
    Color();
    Color (const Color& color);
    Color(float red, float green, float blue);

    void resetRGB();
    void setRGB(float red, float green, float blue);
    inline float getR() const { return r; }
    inline float getG() const { return g; }
    inline float getB() const { return b; }
    inline void setR(float red) { r = red; }
    inline void setG(float green) { g = green; }
    inline void setB(float blue) { b = blue; }
    
    friend Color operator+(const Color& color1, const Color& color2)
    {
        return Color(color1.getR() + color2.getR(), color1.getG() + color2.getG(), color1.getB() + color2.getB());
    }

    friend Color operator*(const Color& color1, const Color& color2)
    {
        return Color(color1.getR() * color2.getR(), color1.getG() * color2.getG(), color1.getB() * color2.getB());
    }
    
    Color operator*(float multiplier);
    void operator*=(float multiplier);
    void operator/=(float divisor);
    void operator+=(const Color& color);
    bool operator==(const Color& color);
    bool operator<(const Color& color);
    
private:
    float r, g, b;
};


#endif /* defined(__Ray_Tracer__C_____Color__) */
