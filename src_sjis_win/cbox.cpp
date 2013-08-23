/**
 * 
 * @file cbox.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of cbox in RT2013 on 2013/08/24.
 * 
 */

#include "cbox.h"

#include "shape/geom.hpp"
#include "mat_diffuse.h"
#include "mat_reflection.h"
#include "mat_transparent.h"

void create_cbox1(Scene &scene) {
    
    std::vector<Sphere> &spheres = scene.spheres;
    std::vector<Triangle> &triangles = scene.triangles;
    std::vector<Vertex> &vertexes = scene.vertexes;
    MaterialTable &materials = scene.materials;
    
    
    //left wall
    DiffuseMaterial *mat0 = new DiffuseMaterial();
    materials.push_back(mat0);
    mat0->diffuse.a() = 1.0;
    mat0->diffuse.r() = 1.0;
    mat0->diffuse.g() = 0.0;
    mat0->diffuse.b() = 0.0;
    mat0->specular = 0.5;
    mat0->emission = 0.0;
    //right wall
    DiffuseMaterial *mat1 = new DiffuseMaterial();
    materials.push_back(mat1);
    mat1->diffuse.a() = 1.0;
    mat1->diffuse.r() = 0.0;
    mat1->diffuse.g() = 1.0;
    mat1->diffuse.b() = 0.0;
    mat1->specular = 0.5;
    mat1->emission = 0.0;
    //back,ceil,floor wall
    DiffuseMaterial *mat2 = new DiffuseMaterial();
    materials.push_back(mat2);
    mat2->diffuse.a() = 1.0;
    mat2->diffuse.r() = 0.5;
    mat2->diffuse.g() = 0.5;
    mat2->diffuse.b() = 0.5;
    mat2->specular = 0.5;
    mat2->emission = 0.0;
    //sphere1
    ReflectionMaterial *mat3 = new ReflectionMaterial();
    mat3->reflection = 1.0;
    materials.push_back(mat3);
    //sphere2
    TransparentMaterial *mat4 = new TransparentMaterial();
    mat4->refraction = kHOGE_REFRACTION;
    materials.push_back(mat4);
    
    DiffuseMaterial *mat5 = new DiffuseMaterial();
    materials.push_back(mat5);
    mat5->diffuse.a() = 1.0;
    mat5->diffuse.r() = 0.5;
    mat5->diffuse.g() = 0.0;
    mat5->diffuse.b() = 0.5;
    mat5->specular = 1.0;
    mat5->emission = 0.0;
    
    spheres.push_back(Sphere(Vec3(6.5, -7.0, 0.0), 3.0));
    spheres[0].m = 3;
    spheres.push_back(Sphere(Vec3(-5.0, 3.0, 5.0), 4.0));
    spheres[1].m = 4;
    spheres.push_back(Sphere(Vec3(-7.0, -8.0, 0.0), 2.0));
    spheres[2].m = 4;
    spheres.push_back(Sphere(Vec3(0.0, -8.0, 7.5), 2.0));
    spheres[3].m = 5;
    
    
    
    const Vec3 p[8] = {
        Vec3(-10.0, -10.0, -10.0),//o
        Vec3(10.0, -10.0, -10.0),//right, bottom, front
        Vec3(-10.0, 10.0, -10.0),//left, up, front
        Vec3(10.0, 10.0, -10.0),//right, up, front
        Vec3(-10.0, -10.0, 10.0),//left, bottom, rear
        Vec3(10.0, -10.0, 10.0),//right, bottom, rear
        Vec3(-10.0, 10.0, 10.0),//left, up, rear
        Vec3(10.0, 10.0, 10.0),//left, up, rear
    };
    const Vec3 n[1] = {
        Vec3(0.0, 0.0, 0.0),
    };
    const Vec2 t[1] = {
        Vec2(0.0, 0.0),
    };
    //left
    vertexes.push_back(Vertex(p[0], n[0], t[0],0));
    vertexes.push_back(Vertex(p[2], n[0], t[0],0));
    vertexes.push_back(Vertex(p[6], n[0], t[0],0));
    vertexes.push_back(Vertex(p[4], n[0], t[0],0));
    //right
    vertexes.push_back(Vertex(p[1], n[0], t[0],1));
    vertexes.push_back(Vertex(p[5], n[0], t[0],1));
    vertexes.push_back(Vertex(p[7], n[0], t[0],1));
    vertexes.push_back(Vertex(p[3], n[0], t[0],1));
    //back
    vertexes.push_back(Vertex(p[4], n[0], t[0],2));
    vertexes.push_back(Vertex(p[6], n[0], t[0],2));
    vertexes.push_back(Vertex(p[7], n[0], t[0],2));
    vertexes.push_back(Vertex(p[5], n[0], t[0],2));
    //ceil
    vertexes.push_back(Vertex(p[2], n[0], t[0],2));
    vertexes.push_back(Vertex(p[3], n[0], t[0],2));
    vertexes.push_back(Vertex(p[7], n[0], t[0],2));
    vertexes.push_back(Vertex(p[6], n[0], t[0],2));
    //floor
    vertexes.push_back(Vertex(p[0], n[0], t[0],2));
    vertexes.push_back(Vertex(p[4], n[0], t[0],2));
    vertexes.push_back(Vertex(p[5], n[0], t[0],2));
    vertexes.push_back(Vertex(p[1], n[0], t[0],2));
    
    size_t i = 0;
    //left
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //right
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //back
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //ceil
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //floor
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    
    
    //box1
    DiffuseMaterial *mat6 = new DiffuseMaterial();
    materials.push_back(mat6);
    mat6->diffuse.a() = 1.0;
    mat6->diffuse.r() = 1.0;
    mat6->diffuse.g() = 1.0;
    mat6->diffuse.b() = 1.0;
    mat6->specular = 1.0;
    mat6->emission = 0.0;
    
    const Vec3 p2[8] = {
        Vec3(-7.0, -10.0, 3.0),//o
        Vec3(-3.0, -10.0, 3.0),//right, bottom, front
        Vec3(-7.0, -1.0, 3.0),//left, up, front
        Vec3(-3.0, -1.0, 3.0),//right, up, front
        Vec3(-7.0, -10.0, 7.0),//left, bottom, rear
        Vec3(-3.0, -10.0, 7.0),//right, bottom, rear
        Vec3(-7.0, -1.0, 7.0),//left, up, rear
        Vec3(-3.0, -1.0, 7.0),//left, up, rear
    };
    const Vec3 n2[1] = {
        Vec3(0.0, 0.0, 0.0),
    };
    const Vec2 t2[1] = {
        Vec2(0.0, 0.0),
    };
    //left
    vertexes.push_back(Vertex(p2[0], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[4], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[6], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[2], n2[0], t2[0],6));
    //right
    vertexes.push_back(Vertex(p2[1], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[3], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[7], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[5], n2[0], t2[0],6));
    //front
    vertexes.push_back(Vertex(p2[0], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[2], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[3], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[1], n2[0], t2[0],6));
    //back
    vertexes.push_back(Vertex(p2[4], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[5], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[7], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[6], n2[0], t2[0],6));
    //ceil
    vertexes.push_back(Vertex(p2[2], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[6], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[7], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[3], n2[0], t2[0],6));
    //floor
    vertexes.push_back(Vertex(p2[0], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[1], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[5], n2[0], t2[0],6));
    vertexes.push_back(Vertex(p2[4], n2[0], t2[0],6));
    
    //left
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //right
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //front
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //back
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //ceil
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    //floor
    triangles.push_back(Triangle(i,i+1,i+2));
    triangles.push_back(Triangle(i,i+2,i+3));
    i += 4;
    
    //light1
    DiffuseMaterial *mat7 = new DiffuseMaterial();
    materials.push_back(mat7);
    mat7->diffuse = 0.0;
    mat7->specular = 0.0;
    mat7->emission = 1.0;
    
    spheres.push_back(Sphere(Vec3(0.0, 10.0, 0.0), 2.0));
    spheres[4].m = 7;
    
}
