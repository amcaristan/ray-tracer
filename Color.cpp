/************************************************************************************************
 File: Color.cpp
 Project: Ray Tracer [C++]
 
 Created by: Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#include "Color.h"

Color::Color()
{
    r = g = b = 0.0;
}

Color::Color (const Color& color)
{
    r = color.getR();
    g = color.getG();
    b = color.getB();
}

Color::Color(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

void Color::resetRGB()
{
    r = g = b = 0.0;
}

void Color::setRGB(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

Color Color::operator*(float multiplier)
{
    return Color(multiplier * r, multiplier * g, multiplier * b);
}

void Color::operator*=(float multiplier)
{
    r *= multiplier;
    g *= multiplier;
    b *= multiplier;
}


void Color::operator/=(float divisor)
{
    r /= divisor;
    g /= divisor;
    b /= divisor;
}


void Color::operator+=(const Color& color)
{
    r += color.getR();
    g += color.getG();
    b += color.getB();
}


bool Color::operator==(const Color& color)
{
    return ((r == color.getR()) && (g == color.getG()) && (b == color.getB())) ? true : false;
}


bool Color::operator<(const Color& color)
{
    return ((r < color.getR()) || (g < color.getG()) || (b < color.getB())) ? true : false;
}



