/**
 * 
 * @file main_phong.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation of main_phong in RT2013 on 2013/07/16.
 * 
 */


#include "shape/primitive.hpp"
#include "shape/geom.hpp"
#include "shape/intersection.h"

#include <iostream>
#include <vector>
#include <fstream>


struct Material {
    Vec4 diffuse;
    Vec4 specular;
    Vec4 emissive;
    real reflection;
    real refractive;
    
    //POD
    Material() = default;
    Material(const Material &) = default;
    Material(Material &&) = default;
    ~Material() = default;
    Material& operator=(const Material &) = default;
    Material& operator=(Material &&) = default;
    
    Material(real val)
    :diffuse(val),specular(val),emissive(val),
    reflection(val),refractive(val) {}
};
static_assert(std::is_pod<Material>::value, "Material is not POD.");


void create_cbox1(std::vector<Sphere> &spheres,
                  std::vector<Triangle> &triangles,
                  std::vector<Vertex> &vertexes,
                  std::vector<Material> &materials) {
    
    materials.resize(5);
    //left wall
    materials[0].diffuse.a() = 1.0;
    materials[0].diffuse.r() = 1.0;
    materials[0].diffuse.g() = 0.0;
    materials[0].diffuse.b() = 0.0;
    materials[0].specular = 0.5;
    materials[0].emissive = 0.0;
    materials[0].reflection = 0.0;
    materials[0].refractive = 0.0;
    //right wall
    materials[1].diffuse.a() = 1.0;
    materials[1].diffuse.r() = 0.0;
    materials[1].diffuse.g() = 1.0;
    materials[1].diffuse.b() = 0.0;
    materials[1].specular = 0.5;
    materials[1].emissive = 0.0;
    materials[1].reflection = 0.0;
    materials[1].refractive = 0.0;
    //back,ceil,floor wall
    materials[2].diffuse.a() = 1.0;
    materials[2].diffuse.r() = 0.5;
    materials[2].diffuse.g() = 0.5;
    materials[2].diffuse.b() = 0.5;
    materials[2].specular = 0.5;
    materials[2].emissive = 0.0;
    materials[2].reflection = 0.0;
    materials[2].refractive = 0.0;
    //sphere1
    materials[3].diffuse.a() = 1.0;
    materials[3].diffuse.r() = 0.0;
    materials[3].diffuse.g() = 0.5;
    materials[3].diffuse.b() = 0.5;
    materials[3].specular = 1.0;
    materials[3].emissive = 0.0;
    materials[3].reflection = 0.0;
    materials[3].refractive = 0.0;
    //sphere2
    materials[4].diffuse.a() = 1.0;
    materials[4].diffuse.r() = 0.5;
    materials[4].diffuse.g() = 0.0;
    materials[4].diffuse.b() = 0.5;
    materials[4].specular = 1.0;
    materials[4].emissive = 0.0;
    materials[4].reflection = 0.0;
    materials[4].refractive = 0.0;
    
    spheres.push_back(Sphere(Vec3(6.5, -7.0, 0.0), 3.0));
    spheres[0].m = 3;
    spheres.push_back(Sphere(Vec3(-5.0, -6.0, 5.0), 4.0));
    spheres[1].m = 4;
    
    
    
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
    
    //left
    triangles.push_back(Triangle(0,1,2));
    triangles.push_back(Triangle(0,2,3));
    //right
    triangles.push_back(Triangle(4,5,6));
    triangles.push_back(Triangle(4,6,7));
    //back
    triangles.push_back(Triangle(8,9,10));
    triangles.push_back(Triangle(8,10,11));
    //ceil
    triangles.push_back(Triangle(12,13,14));
    triangles.push_back(Triangle(12,14,15));
    //floor
    triangles.push_back(Triangle(16,17,18));
    triangles.push_back(Triangle(16,18,19));
    
}

DMat<4,4> create_mat44_x_rotate(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = 1.0;
    mat.get(1,1) = std::cos(theta);
    mat.get(1,2) = -std::sin(theta);
    mat.get(2,1) = std::sin(theta);
    mat.get(2,2) = std::cos(theta);
    return mat;
}
DMat<4,4> create_mat44_y_rotate(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = std::cos(theta);
    mat.get(0,2) = std::sin(theta);
    mat.get(1,1) = 1.0;
    mat.get(2,0) = -std::sin(theta);
    mat.get(2,2) = std::cos(theta);
    return mat;
}
DMat<4,4> create_mat44_z_rotate(real theta) {
    DMat<4, 4> mat(0.0);
    mat.get(0,0) = std::cos(theta);
    mat.get(0,1) = -std::sin(theta);
    mat.get(1,0) = std::sin(theta);
    mat.get(1,1) = std::cos(theta);
    mat.get(2,2) = 1.0;
    return mat;
}
DMat<4,4> create_mat44_xyz_rotate(real x, real y, real z) {
    return create_mat44_x_rotate(x) *
    create_mat44_y_rotate(y) *
    create_mat44_z_rotate(z);
}

Vec3 mul_vec3_mat44(const Vec3 &v, const DMat<4, 4> &mat) {
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
};


void write_bitmap(const ScreenBuffer &buff) {
    std::ofstream ofs("./result.bmp", std::ios::binary | std::ios::trunc);
    if(!ofs) {
        assert(0);
        return;
    }
    
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
            uint8_t r = argb.r() < 1.0 ? argb.r()*255 : 255;
            uint8_t g = argb.g() < 1.0 ? argb.g()*255 : 255;
            uint8_t b = argb.b() < 1.0 ? argb.b()*255 : 255;
            ofs.write(reinterpret_cast<const char*>(&b),1);
            ofs.write(reinterpret_cast<const char*>(&g),1);
            ofs.write(reinterpret_cast<const char*>(&r),1);
        }
    }
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
    
    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Vertex> vertexes;
    std::vector<Material> materials;
    
    create_cbox1(spheres,
                 triangles,
                 vertexes,
                 materials);
    
    ScreenBuffer buff(640,480);
    
    Camera cam;
    cam.fov = 60.0 * M_PI/180.0;//to rad
    cam.aspect = (real)buff.w / (real)buff.h;
    cam.pos = Vec3(0.0, 0.0, -10.0 + std::cos(cam.fov/2.0) * (-10.0/std::sin(cam.fov/2.0)));
    cam.mat = DMat<4,4>::identity();
    
    Vec3 LIGHT(0.0,9.9,0.0);
    
    
    {
        TimeCount tc("ray tracing time");
        
        real v_fov = cam.fov / cam.aspect;
        real plane_width = 2.0 * (sin(cam.fov/2.0)/cos(cam.fov/2.0));
        real plane_height = 2.0 * (sin(v_fov/2.0)/cos(v_fov/2.0));
        real plane_origin_h = -plane_width/2.0;
        real plane_origin_v = plane_height/2.0;
        for(size_t j = 0; j < buff.h; ++j) {//vertical
            for(size_t i = 0; i < buff.w; ++i) {//horizontal
                real x = plane_origin_h + i * plane_width / buff.w;
                real y = plane_origin_v - j * plane_height / buff.h;
                Vec3 dir = mul_vec3_mat44(Vec3(x,y,1.0), cam.mat).to_normal();
                Ray ray(cam.pos, dir);
                
                Ray p2l;
                std::vector<Triangle>::iterator last_hit_triangle_it = triangles.end();
                std::vector<Sphere>::iterator last_hit_sphere_it = spheres.end();
                real min_distance = std::numeric_limits<real>::max();
                for (auto it = triangles.begin(); it != triangles.end(); ++it) {
                    Vec3 point;
                    real length, angle;
                    if(ray.get_intersection_point(*it, vertexes, point, length, angle)) {
                        //real distance = (point - cam.pos).length();
                        if(length < min_distance) {
                            min_distance = length;
                            p2l = Ray::create_start_end(point, LIGHT);
                            last_hit_triangle_it = it;
                            Vec3 normal = it->create_plane(vertexes).n;
                            //diffuse
                            real diffuse_angle = p2l.n.dot(normal);
                            if(0 <= diffuse_angle) {
                                buff.color(j, i) = materials[vertexes[it->ids[0]].m].diffuse * diffuse_angle;
                            } else {
                                buff.color(j, i) = 0.0;
                            }
                            //specular
                            Vec3 ref_lignt = normal * 2.0 * p2l.n.dot(normal) - p2l.n;
                            real specular_power = pow(std::min(0.0, ray.n.dot(ref_lignt)), 50);
                            buff.color(j, i) += materials[vertexes[it->ids[0]].m].specular * specular_power;
                        }
                    }
                }
                for (auto it = spheres.begin(); it != spheres.end(); ++it) {
                    std::vector<Vec3> points;
                    std::vector<real> lengths;
                    if(ray.get_intersection_point(*it, points, lengths)) {
                        real distance = lengths[0];
                        Vec3 point = points[0];
                        if(1 < lengths.size() && lengths[1] <= distance) {
                            assert(0 <= lengths[1]);
                            distance = lengths[1];
                            point = points[1];
                        }
                        if(distance < min_distance) {
                            min_distance = distance;
                            p2l = Ray::create_start_end(point, LIGHT);
                            last_hit_sphere_it = it;
                            Ray c2p = Ray::create_start_end(it->p, point);
                            //diffuse
                            real diffuse_angle = p2l.n.dot(c2p.n);
                            if(0 <= diffuse_angle) {
                                buff.color(j, i) = materials[it->m].diffuse * diffuse_angle;
                            } else {
                                buff.color(j, i) = 0.0;
                            }
                            //specular
                            Vec3 ref_lignt = c2p.n * 2.0 * p2l.n.dot(c2p.n) - p2l.n;
                            real specular_power = pow(std::min(0.0, ray.n.dot(ref_lignt)), 50);
                            buff.color(j, i) += materials[it->m].specular * specular_power;
                        }
                    }
                }
                //shadow
                if(last_hit_triangle_it != triangles.end() || last_hit_sphere_it != spheres.end()) {
                    bool shadow_flag = false;
                    for (auto it = triangles.begin(); !shadow_flag && it != triangles.end(); ++it) {
                        if(it == last_hit_triangle_it) {
                            continue;
                        }
                        Vec3 point;
                        if(p2l.get_intersection_point(*it, vertexes, point)) {
                            buff.color(j, i) = buff.color(j, i) * 0.5;
                            shadow_flag = true;
                        }
                    }
                    for (auto it = spheres.begin(); !shadow_flag && it != spheres.end(); ++it) {                        if(it == last_hit_sphere_it) {
                        continue;
                    }
                        std::vector<Vec3> points;
                        std::vector<real> lengths;
                        if(p2l.get_intersection_point(*it, points, lengths)) {
                            buff.color(j, i) = buff.color(j, i) * 0.5;
                            shadow_flag = true;
                        }
                    }
                }
            }
        }
        tc.display_now();
    }
    
    {
        TimeCount tc("saving time");
        write_bitmap(buff);
        tc.display_now();
    }
    
    return 0;
}