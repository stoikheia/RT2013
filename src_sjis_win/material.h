/**
 * 
 * @file material.h
 * @author Reiji Tokuda.
 *
 * Interface file of Material in RT2013 on 2013/08/22.
 * 
 */

#ifndef material_H
#define material_H

#include <vector>
#include "shape/primitive.hpp"


class Material {
    
public:
    enum MatType {
        MT_DIFFUSE,
        MT_REFLECTION,
        MT_TRANSPARENT,
    };
    
    virtual ~Material() {};
    virtual MatType mat_type() const = 0;
};

class MaterialTable : public std::vector<Material*> {
public:
    MaterialTable():std::vector<Material*>() {}
    ~MaterialTable() {
        for(auto it = this->begin(); it != this->end(); ++it) {
            delete *it;
        }
        this->clear();
    }
    MaterialTable(const MaterialTable &mt) {assert(0);}
    MaterialTable(MaterialTable &&mt) {assert(0);}
    MaterialTable& operator=(const MaterialTable &mt) {assert(0);}
    MaterialTable& operator=(MaterialTable &&mt) {assert(0);}
};

#endif // material_H
