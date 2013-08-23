/**
 * 
 * @file obj_io.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of obj_io in RT2013 on 2013/07/03.
 * 
 */

#include "obj_io.h"

#include <sstream>
#include <map>
#include <cassert>
#include <stdint.h>


bool load_obj_vnf(const std::string &path, std::vector<Triangle> &triangles, std::vector<Vertex> &vertexes) {
    
    bool ret = false;
    std::ifstream ifs(path.c_str(), std::ifstream::in | std::ifstream::binary);
    
    if(!ifs) {
        return false;
    }
    
    ifs.exceptions(std::ios_base::badbit);
    
    ret = load_obj_vnf(ifs, triangles, vertexes);
    
    ifs.close();//throws unhandled exception
    
    return ret;
}

bool load_obj_vnf(std::istream &source, std::vector<Triangle> &triangles, std::vector<Vertex> &vertexes) {
    std::vector<Vec3> v;
    std::vector<Vec3> vn;
    std::map<std::string, size_t> vertexes_map;
    
    bool g = false;//first group flag
    try {
        while(!source.eof()) {
                        
            std::string line;
            if(!std::getline(source, line)) {
                break;
            }
            
            if(!line.size()) {
                continue;
            }
            
            if(line[0] == 'v') {
                
                g = true;
                
                std::stringstream sst(line);
                
                std::string type;
                sst >> type;
                
                if(0 == type.compare("v")) {
                    real x,y,z;
                    sst >> x >> y >> z;
                    v.push_back(Vec3(x,y,z));
                } else if(0 == type.compare("vt")) {
                    //nop
                } else if(0 == type.compare("vn")) {
                    real x,y,z;
                    sst >> x >> y >> z;
                    vn.push_back(Vec3(x,y,z));
                } else {
                    throw std::fstream::failure("Unknown Type.");
                }
                
            } else if(line[0] == 'f') {
                
                if(!g) {
                    throw std::fstream::failure("Invalid Format.");
                }
                
				std::stringstream sst(line);
                
				std::string type;
                std::string part_str[3];
				sst >> type;
                for(size_t i = 0; i < 3; ++i) {
                    sst >> part_str[i];
                }
                
				if(0 != type.compare("f")) {
					throw std::fstream::failure("Unknown Type.");
				}
                
				Triangle tri;
				for(int i = 0; i < 3; i++) {
                    
                    std::stringstream part_sst(part_str[i]);
					uint32_t vi,vni;
					char slash[2];
					part_sst >> vi >> slash[0] >> slash[1] >> vni;
                    
					//check
					if ('/' != slash[0] || '/' != slash[1]) {
						throw std::fstream::failure("Delimiter has Not Found.");
					}
                    
					//range check
					if(v.size() < vi || vn.size() < vni) {
						throw std::fstream::failure("Index is Out of Range.");
					}
                    //zero check
                    if(vi == 0 || vni == 0) {
                        throw std::fstream::failure("Index is Out of Range.(0)");
                    }
                    
					//obj index to array index
					--vi; --vni;
                    
					//to vertexes of triangle
                    if(vertexes_map.count(part_str[i])) {
                        tri.ids[i] = vertexes_map[part_str[i]];
                    } else {
                        vertexes_map[part_str[i]] = tri.ids[i] = vertexes.size();
                        vertexes.push_back(Vertex(v[vi], vn[vni], Vec2(0.0,0.0), 0));
                    }
				}
				//add
				triangles.push_back(tri);
                
            } else if(line[0] == 'g') {
                if(g) {
                    //second group has detected.
                    break;
                }
                g = true;
            }
            
        }
        
        //check empty
        if(!triangles.size()) {
            throw std::fstream::failure("No Triangle.");
        }
    } catch (std::fstream::failure e) {
        assert(0);
        return false;
    }
    
    return true;
}