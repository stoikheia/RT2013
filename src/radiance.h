/**
 * 
 * @file radiance.h
 * @author Reiji Tokuda.
 *
 * Interface file of Radiance in RT2013 on 2013/08/22.
 * 
 */

#ifndef radiance_H
#define radiance_H

#include "shape/geom.hpp"
#include "shape/intersection.h"
#include "material.h"
#include "scene.h"

struct RadianceContext {
    virtual ~RadianceContext() {};
    /** gets ray for current step. returns false if its end of step. */
    virtual bool step_start(Ray &next_ray) = 0;
    /** adds radiance of current step. (without weight calculation) */
    virtual bool step_end(const Vec4 &radiance) = 0;
    virtual Vec4 result() = 0;
};

struct ResultRadianceContext : public RadianceContext {
    
    Ray ray;
    bool first_step;
    Vec4 radiance;
    
    ResultRadianceContext(Ray ray_):ray(ray_), first_step(true), radiance(0.0) {}
    virtual ~ResultRadianceContext() {}
    
    virtual bool step_start(Ray &next_ray) {
        if(first_step) {
            next_ray = ray;
            first_step = false;
            return true;
        } else {
            return false;
        }
    }
    virtual bool step_end(const Vec4 &radiance_) {
        radiance = radiance_;
        return false;
    }
    virtual Vec4 result() {
        return radiance;
    }
};

class RadianceContextStack : public std::vector<RadianceContext*> {
public:
    RadianceContextStack():std::vector<RadianceContext*>() {}
    ~RadianceContextStack() {
        for(auto it = this->begin(); it != this->end(); ++it) {
            delete *it;
        }
        this->clear();
    }
    RadianceContextStack(const RadianceContextStack &mt) {assert(0);}
    RadianceContextStack(RadianceContextStack &&mt) {assert(0);}
    RadianceContextStack& operator=(const RadianceContextStack &mt) {assert(0);}
    RadianceContextStack& operator=(RadianceContextStack &&mt) {assert(0);}
    
    void pop_back_with_delete() {
        delete this->back();
        this->pop_back();
    }

};


#endif // radiance_H
