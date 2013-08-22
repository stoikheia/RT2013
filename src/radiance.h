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

class RadianceContextStock : public std::vector<RadianceContext*> {
public:
    RadianceContextStock():std::vector<RadianceContext*>() {}
    ~RadianceContextStock() {
        for(auto it = this->begin(); it != this->end(); ++it) {
            delete *it;
        }
        this->clear();
    }
    RadianceContextStock(const RadianceContextStock &mt) {assert(0);}
    RadianceContextStock(RadianceContextStock &&mt) {assert(0);}
    RadianceContextStock& operator=(const RadianceContextStock &mt) {assert(0);}
    RadianceContextStock& operator=(RadianceContextStock &&mt) {assert(0);}
    
    void pop_back_with_delete() {
        delete this->back();
        this->pop_back();
    }

};


#endif // radiance_H
