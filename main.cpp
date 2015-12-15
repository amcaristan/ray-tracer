/************************************************************************************************
 File: main.cpp
 Project: Ray Tracer [C++]
 
 Created by: Dr. Scott Schaefer & Adrien Caristan on 9/14/15.
 Copyright (c) 2015 Adrien Mombo-Caristan. All rights reserved.
************************************************************************************************/

#define ImageW 800
#define ImageH 800
#define BG_COLOR Color (0.1, 0.1, 0.1)
#define DEPTH_LIMIT 2

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include "Scene.h"
#include "Color.h"

/* For Mac */
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

/* For Windows */
//#include <Windows.h>
//#include <GL/glut.h>

using namespace std;

/************************************************************************************************
 Notes: Window size is 800 x 800 by default.
************************************************************************************************/
float framebuffer[ImageH][ImageW][3];
float pixelPositions[ImageH][ImageW][3];

Point eyePosition(0.0, 0.0, -5.0);
Point normalizedEyePosition = eyePosition.normalize();

/* Representing a scene as a vector of surfaces that are present within scene */
vector<Scene> scenes;
Scene currentActiveScene;

void clearFramebuffer() {
    for(int i = 0; i < ImageH; i++) {
        for (int j = 0; j < ImageW; j++) {
            framebuffer[i][j][0] = 0.0;
            framebuffer[i][j][1] = 0.0;
            framebuffer[i][j][2] = 0.0;
        }
    }
}

/* Sets pixel x,y to the color RGB I've made a small change to this function to make the pixels 
    match those returned by the glutMouseFunc exactly - Scott Schaefer */
void setFramebuffer(int x, int y, float R, float G, float B) {
    /* Changes the origin from the lower-left corner to the upper-left corner */
    y = ImageH - 1 - y;
    if (R<=1.0)
        if (R>=0.0)
            framebuffer[y][x][0]=R;
        else
            framebuffer[y][x][0]=0.0;
        else
            framebuffer[y][x][0]=1.0;
    if (G<=1.0)
        if (G>=0.0)
            framebuffer[y][x][1]=G;
        else
            framebuffer[y][x][1]=0.0;
        else
            framebuffer[y][x][1]=1.0;
    if (B<=1.0)
        if (B>=0.0)
            framebuffer[y][x][2]=B;
        else
            framebuffer[y][x][2]=0.0;
        else
            framebuffer[y][x][2]=1.0;
}

/* Draws the scene */
void drawit(void) {
    glDrawPixels(ImageW,ImageH,GL_RGB,GL_FLOAT,framebuffer);
    glFlush();
}

void display(void) {
    drawit();
}

/* Calculates pixel positions in screen space */
void calcPixelPositions()
{
    for (int i = 0; i < ImageH; i++) {
        for (int j = 0; j < ImageW; j++) {
            pixelPositions[i][j][0] = (float) ((2.0 * j) / (ImageW - 1.0)) - 1.0 ;
            pixelPositions[i][j][1] = (float) 1.0 - ((2.0 * i) / (ImageH - 1.0)) ;
            pixelPositions[i][j][2] = 0.0;
        }
    }
}


/************************************************************************************************
 RayTracing functions
************************************************************************************************/
Color calcAmbience(Surface* surface) {
    Color surfaceAmbience = surface->getAmbientCoefficients();
    Color sceneAmbience = currentActiveScene.getAmbientIntensity();
    
    return Color(surfaceAmbience.getR() * sceneAmbience.getR(),
                 surfaceAmbience.getG() * sceneAmbience.getG(),
                 surfaceAmbience.getB() * sceneAmbience.getB());
}

Color calcIllumination(Surface* surface, Ray normal, Light lightSource, bool calcAmb) {
    /* Illum = kaA + C( kd(L.N) + ks(R.E)^n ) */
    
    Color final;
    
    if (calcAmb) {
        final += calcAmbience(surface);
    }
    
    /* Evaluate illumination contributions from light source and add to final color */
    Ray surfaceToLightRay(normal.getStartPoint(), lightSource.getPosition());  /* == L */
    
    /* surfaceToLightRay is used for calculating the reflected ray as the getReflectedRay function
       assumes tha the ray is pointing from teh light source to the surface */
    Ray lightToSurfaceRay(lightSource.getPosition(), normal.getStartPoint());
    
    float LN = dotProduct(surfaceToLightRay.normalize(), normal.normalize());
    
    if (LN <= 0.0) {    /* Light is hitting non-visible side of the surface */
        
        return final;
        
    } else {    /* Calculate color on visible side of the surface */
        
        Color diffuseComponent = surface->getDiffuseCoefficients() * LN;
        
        Ray eyeRay(normal.getStartPoint(), eyePosition);
        float RE = dotProduct(lightToSurfaceRay.getReflectedRay(normal).normalize(), eyeRay.normalize());
        
        Color specularComponent = surface->getSpecularCoefficients() * powf(RE, 5);
        
        final += lightSource.getRGBIntensity() * (diffuseComponent + specularComponent);
    }

    return final;
}

Color rayTrace(Ray ray, int depth) {
    
    vector<Surface*> surfaces = currentActiveScene.getSurfaces();
    Surface* closestSurface = NULL;
    Ray closestSurfaceNormal;
    float closestIntersection = INFINITY;
    int closestSurfaceIndex = INFINITY;
    
    /* Find the surface that has the closest intersection with 'ray' */
    for (int i = 0; i < surfaces.size(); i++) {
        Ray surfaceNormal;
        float intersection = surfaces[i]->intersect(ray, surfaceNormal);
        
        if (intersection < 1) { /* No intersection or intersection not visible */
            continue;
        } else if (intersection < closestIntersection) {    /* Store intersection information */
            closestSurfaceIndex = i;
            closestIntersection = intersection;
            closestSurface = surfaces[i];
            closestSurfaceNormal = surfaceNormal;
        }
    }
    
    Color finalColor;
    
    if (closestSurface == NULL) {   /* If there are no intersections return background color */
        
        return BG_COLOR;
        
    } else {
        
        Point adjustedIntersectionPoint = closestSurfaceNormal.getStartPoint();
        
        vector<Light> sceneLights = currentActiveScene.getLights();
        
        /* Cast shadow ray from surface to each light source to determine if point is in shadow */
        bool calcAmb = true;
        for (int j = 0; j < sceneLights.size(); j++) {
            Ray lightRay(sceneLights[j].getPosition(), adjustedIntersectionPoint);
            
            Ray tempNormal;
            closestIntersection = surfaces[closestSurfaceIndex]->intersect(lightRay, tempNormal);
            
            bool inShadow = false;
            
            for (int k = 0; k < surfaces.size(); k++) {
                float intersection = surfaces[k]->intersect(lightRay, tempNormal);
                
                if (intersection > 1 && intersection < closestIntersection) {
                    inShadow = true;
                }
            }
            
            if (inShadow) {
                if (calcAmb) {
                    finalColor += calcAmbience(closestSurface);
                    calcAmb = false;
                }
            } else {
                /* Calculate illumination at intersection point */
                finalColor += calcIllumination(closestSurface, closestSurfaceNormal, sceneLights[j], calcAmb);
                calcAmb = false;
            }
        }

        /* Cast reflection ray if object is reflective */
        if (depth <= DEPTH_LIMIT && closestSurface->getReflectivity() > 0.0) {
            Ray reflectedRay = ray.getReflectedRay(closestSurfaceNormal);
            finalColor += (rayTrace(reflectedRay, depth + 1) * closestSurface->getReflectivity());
        }
        
    }
    
    return finalColor;
}


/************************************************************************************************
 GLUT functions
************************************************************************************************/
void keyboard ( unsigned char key, int x, int y ) {
    switch (key) {
        case '1':
        {
            clearFramebuffer();
            
            currentActiveScene = scenes[0];
            
            cout << "Drawing scene 1... ";
            
            for (int i = 0; i < ImageH; i++) {
                
                for (int j = 0; j < ImageW; j++) {
                    
                    /* For testing purposes: stop at center of display window or draw single white pixel there */
                    if (i == 462 && j == 400) {
                        int k = 0; k++;
                        setFramebuffer(j, i, 1,0,0);
                        //continue;
                    }/**/
                    
                    Point pixelPoint(pixelPositions[i][j][0], pixelPositions[i][j][1], pixelPositions[i][j][2]);
                    Color pixelColor = rayTrace( Ray(eyePosition, pixelPoint), 0 );
                    
                    setFramebuffer(j, i, pixelColor.getR(), pixelColor.getG(), pixelColor.getB());
                    
                    pixelColor.resetRGB();
                    
                }
                
            }
            
            display();
            
            cout << "Done.\n";
            
            break;
        }
            
        case '2':
        {
            clearFramebuffer();
            
            currentActiveScene = scenes[1];
            
            cout << "Drawing scene 2... ";
            
            for (int i = 0; i < ImageH; i++) {
                
                for (int j = 0; j < ImageW; j++) {
                    
                    Point pixelPoint(pixelPositions[i][j][0], pixelPositions[i][j][1], pixelPositions[i][j][2]);
                    Color pixelColor = rayTrace( Ray(eyePosition, pixelPoint), 0 );
                    
                    setFramebuffer(j, i, pixelColor.getR(), pixelColor.getG(), pixelColor.getB());
                    
                    pixelColor.resetRGB();
                    
                }
                
            }
            
            display();
            
            cout << "Done.\n";
            
            break;
        }
            
        case '3':
        {
            clearFramebuffer();
            
            currentActiveScene = scenes[2];
            
            cout << "Drawing scene 3... ";
            
            for (int i = 0; i < ImageH; i++) {
                
                for (int j = 0; j < ImageW; j++) {
                    
                    Point pixelPoint(pixelPositions[i][j][0], pixelPositions[i][j][1], pixelPositions[i][j][2]);
                    Color pixelColor = rayTrace( Ray(eyePosition, pixelPoint), 0 );
                    
                    setFramebuffer(j, i, pixelColor.getR(), pixelColor.getG(), pixelColor.getB());
                    
                    pixelColor.resetRGB();
                    
                }
                
            }
            
            display();
            
            cout << "Done.\n";
            
            break;
        }
            
        case '4':
        {
            clearFramebuffer();
            
            currentActiveScene = scenes[3];
            
            cout << "Drawing scene 4... ";
            
            for (int i = 0; i < ImageH; i++) {
                
                for (int j = 0; j < ImageW; j++) {
                    
                    Point pixelPoint(pixelPositions[i][j][0], pixelPositions[i][j][1], pixelPositions[i][j][2]);
                    Color pixelColor = rayTrace( Ray(eyePosition, pixelPoint), 0 );
                    
                    setFramebuffer(j, i, pixelColor.getR(), pixelColor.getG(), pixelColor.getB());
                    
                    pixelColor.resetRGB();
                }
                
            }
            
            display();
            
            cout << "Done.\n";
            
            break;
        }
            
        default:
            break;
    }
    
}

/* Set up all scenes */
void init(void) {
    clearFramebuffer();
    
    /* Calculating pixel positions in 3D space */
    calcPixelPositions();
    
    /* Building scene 1 */
    scenes.push_back( Scene() );
    scenes.back().addLight( Light( Point(1.0, 3.0, 2.0), Color(1.0, 1.0, 1.0) ) );
    //scenes.back().addLight( Light( Point(-1.0, 1.0, 0.0), Color(1.0, 1.0, 1.0) ) );
    
    Point sphere1Center(0.0, 0.0, 5.0);
    Color sphere1Ambience(0.0, 0.1, 0.0);
    Color sphere1Diffuse(0.0, 0.7, 0.0);
    Color sphere1Specular(0.75, 0.75, 0.75);
    
    Point sphere2Center(0.5, 1.0, 4.0);
    Color sphere2Ambience(0.1, 0.0, 0.0);
    Color sphere2Diffuse(0.7, 0.0, 0.0);
    Color sphere2Specular(0.75, 0.75, 0.75);
    
    Point sphere3Center(0.0, -0.85, 1.69);
    Color sphere3Ambience(0.5, 0.0, 0.5);
    Color sphere3Diffuse(0.5, 0.0, 0.5);
    Color sphere3Specular(0.75, 0.75, 0.75);
    
    /*Ray planeNorm(Point(0.0, -1.0, 3.0), Point(0.0, 0.0, 2.9999));
    float planeAmbience[3] = {0.1, 0.0, 0.0};
    float planeDiffuse[3] = {0.7, 0.0, 0.0};
    float planeSpecular[3] = {0.75, 0.75, 0.75};*/
    
    scenes.back().addSphere( new Sphere(sphere1Center, 0.5, sphere1Ambience, sphere1Diffuse, sphere1Specular, 0.0) );
    scenes.back().addSphere( new Sphere(sphere2Center, 0.25, sphere2Ambience, sphere2Diffuse, sphere2Specular, 0.0) );
    //scenes.back().addSphere( new Sphere(sphere3Center, 0.15, sphere3Ambience, sphere3Diffuse, sphere3Specular, 0.0) );
    //scenes.back().addInfinitePlane( new InfinitePlane() );
    //scenes.back().addInfinitePlane( new InfinitePlane(planePoint, planeNorm, planeAmbience, planeDiffuse, planeSpecular, 0.0) );
    
    
    /* Building scene 2 */
    scenes.push_back( Scene() );
    
    
    /* Building scene 3 */
    scenes.push_back( Scene() );
    
    
    /* Building scene 4 */
    scenes.push_back( Scene() );
}


/************************************************************************************************
 Main function
************************************************************************************************/
int main(int argc, char* argv[]) {
    cout << "Testing...\n\n";
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(ImageW,ImageH);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Adrien Mombo-Caristan - Homework 5");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    
    //Ray testRay(Point(-1, 1, 1), Point(0, 0, 0));
    //Ray reflectedRay = testRay.getReflectedRay( Ray( Point(0,0,0), Point(0,1,0) ));
    //testRay.normalize();
    
    return 0;
}

