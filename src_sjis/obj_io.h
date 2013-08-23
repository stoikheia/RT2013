/**
 * 
 * @file obj_io.h
 * @author Reiji Tokuda.
 *
 * Interface file of obj_io in RT2013 on 2013/07/03.
 * 
 */

#ifndef obj_io_H
#define obj_io_H

#include <fstream>

#include "shape/geom.hpp"

bool load_obj_vnf(const std::string &path, std::vector<Triangle> &triangles, std::vector<Vertex> &vertexes);

bool load_obj_vnf(std::istream &source, std::vector<Triangle> &triangles, std::vector<Vertex> &vertexes);

#endif // obj_io_H
