/**
 * 
 * @file scene.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of scene in RT2013 on 2013/06/12.
 * 
 */

#include "scene.h"

const real kAIR_REFRACTION = 1.000292;
const real kHOGE_REFRACTION = 1.05;
const real kDIAMOND_REFRACTION = 2.42;
const real kFREQUENCY_R = 700.0;//nm
const real kFREQUENCY_G = 546.1;//nm
const real kFREQUENCY_B = 435.8;//nm