/**
 * 
 * @file radiance.cpp
 * @author Reiji Tokuda.
 *
 * Implimentation file of Radiance in RT2013 on 2013/08/22.
 * 
 */

#include "radiance.h"

#include "mat_diffuse.h"
#include "mat_reflection.cpp"
#include "mat_transparent.cpp"

#include <random>
#include <chrono>

static void push_rad_ctx_stack(RadianceContextStack &stack,
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
            p = new DiffuseRadianceContext(env,
                                           scene.materials[mat_id],
                                           scene,
                                           std::move(info));
            assert(p);
            stack.push_back(p);
            break;
        case Material::MT_REFLECTION:
            p = new ReflectionRadianceContext(env,
                                              scene.materials[mat_id],
                                              stack.size(),
                                              std::move(info));
            assert(p);
            stack.push_back(p);
            break;
            
        case Material::MT_TRANSPARENT:
            p = new TransparentRadianceContext(env,
                                               scene.materials[mat_id],
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

Vec4 get_radiance(const Ray &ray, const Scene &scene, size_t seed) {
    
    time_t t;
    ctime(&t);
    Environment env(static_cast<Environment::init_type>(t * seed));
    env.refraction_stack.push_back(kAIR_REFRACTION);//bug
    
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