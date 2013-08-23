//
//  main.cpp
//  RT2013
//
//  Created by 3xv on 2013/06/12.
//  Copyright (c) 2013年 3xv. All rights reserved.
//


#include "shape/primitive.hpp"
#include "shape/geom.hpp"
#include "shape/intersection.h"
#include "material.h"
#include "scene.h"
#include "radiance.h"
#include "mat_diffuse.h"
#include "mat_reflection.h"
#include "mat_transparent.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <random>
#include <memory>

//unko macro
#define self (*this)

real kAIR_REFRACTION = 1.000292;
real kHOGE_REFRACTION = 1.05;
real kDIAMOND_REFRACTION = 2.42;
real kFREQUENCY_R = 700.0;//nm
real kFREQUENCY_G = 546.1;//nm
real kFREQUENCY_B = 435.8;//nm

inline real K2C(real c) {//kelvin to celsius
    return c - 273.15;
}

real create_refraction_air_edlen(real temp_c, real pressure_hpa, real humidity_1per) {
    return 1.0+3.83639*10.0e-7*
    0.75 * pressure_hpa * ( (1+0.75*pressure_hpa*(0.817-0.0133*temp_c)*10.0e-6)/(1+0.003661*temp_c) )
    -5.607943*10.0e-10*humidity_1per*(4.07859739+0.44301857*temp_c + 0.00232093*temp_c*temp_c + 0.00045785*temp_c*temp_c*temp_c);
}

real create_refraction_air(real refraction, real temp, real pressure, real humidity_10per, real co2_100ppm) {
    return refraction +
    //(temp * -10.0 + pressure * 300.0 + humidity_10per * -100.0 + co2_100ppm * 1000.0)
    (temp * -1.0 + pressure * 0.3 + humidity_10per * -0.1 + co2_100ppm * 0.01)
    * 1.0e-6;
}





void create_cbox1(Scene &scene) {
    
    std::vector<Sphere> &spheres = scene.spheres;
    std::vector<Triangle> &triangles = scene.triangles;
    std::vector<Vertex> &vertexes = scene.vertexes;
    MaterialTable &materials = scene.materials;
    std::vector<Vec3> &lights = scene.lights;
    
    lights.push_back(Vec3(0.0,9.9,0.0));
    //lights.push_back(Vec3(0.0,-9.9,0.0));
    
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

}

inline real to_rad(real degree) {
    return degree * M_PI / 180.0;
}

DMat<4,4> create_mat44_x_axix_rotation(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = 1.0;
    mat.get(1,1) = std::cos(theta);
    mat.get(1,2) = -std::sin(theta);
    mat.get(2,1) = std::sin(theta);
    mat.get(2,2) = std::cos(theta);
    return mat;
}
DMat<4,4> create_mat44_y_axix_rotation(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = std::cos(theta);
    mat.get(0,2) = std::sin(theta);
    mat.get(1,1) = 1.0;
    mat.get(2,0) = -std::sin(theta);
    mat.get(2,2) = std::cos(theta);
    return mat;
}
DMat<4,4> create_mat44_z_axix_rotation(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = std::cos(theta);
    mat.get(0,1) = -std::sin(theta);
    mat.get(1,0) = std::sin(theta);
    mat.get(1,1) = std::cos(theta);
    mat.get(2,2) = 1.0;
    return mat;
}
DMat<4,4> create_mat44_zxy_rotation(real x, real y, real z) {
    return create_mat44_z_axix_rotation(z) *
            create_mat44_x_axix_rotation(x) *
            create_mat44_y_axix_rotation(y);
}

Vec3 operator*(const Vec3 &v, const DMat<4, 4> &mat) {
    return Vec3(
                v.x()*mat.get(0,0)+v.y()*mat.get(1,0)+v.z()*mat.get(2,0)+1.0*mat.get(3,0),
                v.x()*mat.get(0,1)+v.y()*mat.get(1,1)+v.z()*mat.get(2,1)+1.0*mat.get(3,1),
                v.x()*mat.get(0,2)+v.y()*mat.get(1,2)+v.z()*mat.get(2,2)+1.0*mat.get(3,2)
    );
}


struct ScreenBuffer {
    const size_t w;//width
    const size_t h;//heigth
    std::vector<Vec4> argb_;
    std::vector<real> depth_;
    
    ScreenBuffer(size_t w_, size_t h_):
    w(w_),
    h(h_),
    argb_(w*h,Vec4(0.0)),
    depth_(w*h,std::numeric_limits<real>::max()) {}
    
    //vecrtical/horizontal
    Vec4& color(size_t v_pos, size_t h_pos) {return argb_[h_pos+v_pos*w];}
    const Vec4& const_color(size_t v_pos, size_t h_pos) const {return argb_[h_pos+v_pos*w];}
    real& depth(size_t v_pos, size_t h_pos) {return depth_[h_pos+v_pos*w];}
    const real& const_depth(size_t v_pos, size_t h_pos) const {return depth_[h_pos+v_pos*w];}
};

struct Camera {
    real fov;//horizontal fov
    real aspect;
    Vec3 pos;
    DMat<4,4> mat;
    
private:
    real v_fov;
    real plane_width;
    real plane_height;
    real plane_origin_h;
    real plane_origin_v;
public:
    
    void setup() {
        v_fov = fov / aspect;
        plane_width = 2.0 * (sin(fov/2.0)/cos(fov/2.0));
        plane_height = 2.0 * (sin(v_fov/2.0)/cos(v_fov/2.0));
        plane_origin_h = -plane_width/2.0;
        plane_origin_v = plane_height/2.0;
    }
    
    Ray get_ray(const ScreenBuffer &buff, size_t i, size_t j) {
        real x = plane_origin_h + i * plane_width / buff.w;
        real y = plane_origin_v - j * plane_height / buff.h;
        Vec3 dir = (Vec3(x,y,1.0) * mat).to_normal();
        return Ray(pos, dir);
    }
};

uint8_t convert_display_color(real val) {
    if(0.0 > val) {
        return 0.0;
    } else if(1.0 <= val){
        return 255;
    } else {
        return val * 255.0;
    }
}

void write_bitmap(const ScreenBuffer &buff, std::ofstream &ofs) {
    
    const uint32_t file_header_size = 14;
    const uint32_t info_header_size = 40;
    
    //file header
    ofs.write("BM", 2);//bfType
    assert(buff.w <= std::numeric_limits<uint16_t>::max());
    assert(buff.h <= std::numeric_limits<uint16_t>::max());
    uint32_t buff_size = (uint32_t)buff.w * (uint32_t)buff.h * 3;
    uint32_t fsize = buff_size + file_header_size + info_header_size;
    uint32_t zero = 0;
    ofs.write(reinterpret_cast<const char*>(&fsize), 4);//bfSize
    ofs.write(reinterpret_cast<const char*>(&zero), 2);//bfReserved1
    ofs.write(reinterpret_cast<const char*>(&zero), 2);//bfReserved2
    uint32_t offset = file_header_size + info_header_size;
    ofs.write(reinterpret_cast<char*>(&offset), 4);//bfOffBits
    //info header
    ofs.write(reinterpret_cast<const char*>(&info_header_size), 4);//biSize
    ofs.write(reinterpret_cast<const char*>(&buff.w), 4);//biWidth
    ofs.write(reinterpret_cast<const char*>(&buff.h), 4);//biHeight
    uint32_t num_plane = 1;
    ofs.write(reinterpret_cast<const char*>(&num_plane), 2);//biPlanes
    uint32_t bit_count = 24;
    ofs.write(reinterpret_cast<const char*>(&bit_count), 2);//biBitCount
    uint32_t comp = 0;
    ofs.write(reinterpret_cast<const char*>(&comp), 4);//biCompression
    ofs.write(reinterpret_cast<const char*>(&buff_size), 4);//biSizeImage
    uint32_t dpi = 96;
    ofs.write(reinterpret_cast<const char*>(&dpi), 4);//biXPixPerMeter
    ofs.write(reinterpret_cast<const char*>(&dpi), 4);//biYPixPerMeter
    uint32_t palette = (1 << 24) -1;
    ofs.write(reinterpret_cast<const char*>(&palette), 4);//biClrUsed
    uint32_t important = 0;
    ofs.write(reinterpret_cast<const char*>(&important), 4);//biCirImportant
    //data
    for(size_t j = buff.h; j > 0; --j) {//vertical
        for(size_t i = 0; i < buff.w; ++i) {//horizontal
            const Vec4 &argb = buff.const_color(j-1,i);
            uint8_t r = convert_display_color(argb.r());
            uint8_t g = convert_display_color(argb.g());
            uint8_t b = convert_display_color(argb.b());
            ofs.write(reinterpret_cast<const char*>(&b),1);
            ofs.write(reinterpret_cast<const char*>(&g),1);
            ofs.write(reinterpret_cast<const char*>(&r),1);
        }
    }
}


struct Environment {
    std::vector<real> refraction_stack;
};

void push_rad_ctx_stack(RadianceContextStack &stack,
                        const Scene &scene,
                        Environment &env,
                        std::unique_ptr<const Scene::IntersectionInformation> &&info) {
    size_t mat_id;
    if(info->is_triangle) {
        mat_id = scene.vertexes[scene.triangles[info->geom_id].ids[0]].m;
    } else {
        mat_id = scene.spheres[info->geom_id].m;
    }
    RadianceContext *p;
    switch (scene.materials[mat_id]->mat_type()) {
        case Material::MT_DIFFUSE:
            p = new DiffuseRadianceContext(scene.materials[mat_id],
                                           scene,
                                           std::move(info));
            assert(p);
            stack.push_back(p);
            break;
        case Material::MT_REFLECTION:
            p = new ReflectionRadianceContext(scene.materials[mat_id],
                                              stack.size(),
                                              std::move(info));
            assert(p);
            stack.push_back(p);
            break;
            
        case Material::MT_TRANSPARENT:
            p = new TransparentRadianceContext(scene.materials[mat_id],
                                               stack.size(),
                                               env.refraction_stack,
                                               std::move(info));
            assert(p);
            stack.push_back(p);
            break;
            
        default:
            assert(0);
            break;
    }
    
}

Vec4 get_radiance(const Ray &ray, const Scene &scene) {
    
    
    Environment env;
    env.refraction_stack.push_back(kAIR_REFRACTION);
    
    RadianceContextStack rad_ctx_stack;
    ResultRadianceContext *p_ret_ctx = new ResultRadianceContext(ray);
    assert(p_ret_ctx);
    rad_ctx_stack.push_back(p_ret_ctx);
    
    std::unique_ptr<Scene::IntersectionInformation> first_info(new Scene::IntersectionInformation());
    if(scene.get_intersecton(ray, *first_info)) {
        push_rad_ctx_stack(rad_ctx_stack, scene, env, std::move(first_info));
    }
    
    Vec4 last_radiance(0.0);
    while (1 < rad_ctx_stack.size()) {
        Ray step_ray;
        if(rad_ctx_stack.back()->step_start(step_ray)) {
            
            std::unique_ptr<Scene::IntersectionInformation> info(new Scene::IntersectionInformation());
            if(scene.get_intersecton(step_ray, *info)) {
                push_rad_ctx_stack(rad_ctx_stack, scene, env, std::move(info));
            } else {
                rad_ctx_stack.back()->step_end(Vec4(0.0));
            }
            
        } else {
            rad_ctx_stack.back()->step_end(last_radiance);
            last_radiance = rad_ctx_stack.back()->result();
            rad_ctx_stack.pop_back_with_delete();
            rad_ctx_stack.back()->step_end(last_radiance);
        }
        
    }

    return p_ret_ctx->result();
}


struct TimeCount {
    std::chrono::high_resolution_clock::time_point time_point;
    std::string msg;
    TimeCount(const std::string &str):time_point(std::chrono::high_resolution_clock::now()),msg(str) {}
    void display_now() {
        auto duration = std::chrono::high_resolution_clock::now() - time_point;
        auto count = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        std::cout << msg << " time : " << count/1000000 << "." << count%1000000 <<" sec" << std::endl;
    }
};

int main(int argc, const char * argv[])
{
    
    Scene scene;
    
    create_cbox1(scene);
    
    ScreenBuffer buff(640,480);
 
    Camera cam;
    cam.fov = 60.0 * M_PI/180.0;//to rad
    cam.aspect = (real)buff.w / (real)buff.h;
    cam.pos = Vec3(0.0, 0.0, -10.0 + std::cos(cam.fov/2.0) * (-10.0/std::sin(cam.fov/2.0)));
    cam.mat = DMat<4,4>::identity();
    
    std::mt19937 engin;
    std::uniform_real_distribution<real> dist(0.0, 1.0);
        
    {
        TimeCount tc("ray tracing");
        
        cam.setup();

        for(size_t j = 0; j < buff.h; ++j) {//vertical
            for(size_t i = 0; i < buff.w; ++i) {//horizontal
                Ray ray = cam.get_ray(buff, i, j);
                if(i == 233 && j == 169) {
                    printf("aa");
                }
                buff.color(j,i) = get_radiance(ray, scene);
            }
        }
        tc.display_now();
    }
    
    {
        TimeCount tc("saving");
        std::ofstream ofs("./result.bmp", std::ios::binary | std::ios::trunc);
        if(!ofs) {
            assert(0);
        }
        write_bitmap(buff, ofs);
        tc.display_now();
    }
    
    return 0;
}

