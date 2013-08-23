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
#include "cbox.h"
#include "screen_buffer.h"
#include "camera.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <random>
#include <memory>
#include <sstream>
#include <thread>
#include <atomic>

//unko macro
#define self (*this)

const size_t kWIDTH = 320;
const size_t kHEIGHT = 240;
const size_t kNUM_SAMPLES = 500;
const size_t kNUM_SUB_SAMPLES = 4;//random sub sampling

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

struct JobInfo {
    std::mutex mutex;
    std::atomic<size_t> thread_count;
    std::atomic<size_t> finished_line_count;
    std::condition_variable condition;
    JobInfo():thread_count(0),finished_line_count(0) {}
    void wait() {
        std::unique_lock<std::mutex> lock(mutex);
        condition.wait(lock);
    }
    void notify_one() {
        condition.notify_one();
    }
    void join_all() {
        while(thread_count != 0) {
            wait();
        }
    }
};

void setup_camera(Camera &cam, size_t w, size_t h, size_t samples, size_t sub_samples);
std::thread start_time_result(bool &flag, std::condition_variable &cond, ScreenBuffer &sb);
typedef std::pair<size_t, size_t> VRange;
void setup_job_queue(size_t height, std::vector<VRange> &job_queue);
void start_job(const Camera &cam, const Scene &scene, ScreenBuffer &sb, VRange range, JobInfo &job_info);

int main(int argc, const char * argv[])
{
    
    Scene scene;
    create_cbox1(scene);
    
    ScreenBuffer buff(kWIDTH, kHEIGHT);
    
    Camera cam;
    setup_camera(cam, kWIDTH, kHEIGHT, kNUM_SAMPLES, kNUM_SUB_SAMPLES);
    
    //creating time result threads
    bool thread_flag = true;
    std::condition_variable time_result_condition;
    std::thread th_time_result = start_time_result(thread_flag, time_result_condition, buff);
    
    std::mt19937 engin(1234);
    
    //job_queue
    std::vector<VRange> job_queue;//stack...
    setup_job_queue(kHEIGHT, job_queue);
    
    
    {
        TimeCount tc("ray tracing");

        JobInfo job_info;
        while(job_queue.size()) {
            while(std::thread::hardware_concurrency() > job_info.thread_count && job_queue.size()) {
                VRange &range = job_queue.back();
                start_job(cam, scene, buff, range, job_info);
                job_queue.pop_back();
            }
            
            //wait
            job_info.wait();
        }
        //wait for all
        job_info.join_all();
        
        tc.display_now();
    }
    
    {
        TimeCount tc("saving");
        std::ofstream ofs("./result.bmp", std::ios::binary | std::ios::trunc);
        if(!ofs) {
            assert(0);
        }
        buff.write_bitmap(ofs);
        tc.display_now();
    }
    
    thread_flag = false;
    time_result_condition.notify_one();
    th_time_result.join();
    
    return 0;
}

void setup_camera(Camera &cam, size_t w, size_t h, size_t samples, size_t sub_samples)
{
    cam.fov = 60.0 * M_PI/180.0;//to rad
    cam.aspect = (real)w / (real)h;
    cam.pos = Vec3(0.0, 0.0, -10.0 + std::cos(cam.fov/2.0) * (-10.0/std::sin(cam.fov/2.0)));
    cam.mat = DMat<4,4>::identity();
    cam.samples = samples;
    cam.sub_samples = sub_samples;
    cam.setup();
}


std::thread start_time_result(bool &flag, std::condition_variable &cond, ScreenBuffer &sb)
{
    return std::thread([&flag, &cond, &sb](){
        size_t count = 0;
        while(flag) {
            std::mutex mutex;
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait_for(lock, std::chrono::minutes(1));
            std::stringstream sst;
            sst << "./result" << count << ".bmp";
            std::ofstream ofs(sst.str(), std::ios::binary | std::ios::trunc);
            if(!ofs) {
                assert(0);
            }
            sb.write_bitmap(ofs);
            count++;
        }
    });
}

void setup_job_queue(size_t height, std::vector<VRange> &job_queue)
{
    size_t num_thread = std::thread::hardware_concurrency();
    size_t slice_size = height / (num_thread * 5);
    size_t slice_rem = height;
    size_t slice_start = 0;
    while (slice_rem >= slice_size) {
        job_queue.push_back(VRange(slice_start, slice_size));
        slice_rem -= slice_size;
        slice_start += slice_size;
    }
    if(slice_rem) {
        job_queue.push_back(VRange(slice_start, slice_rem));
    }
}

void start_job(const Camera &cam, const Scene &scene, ScreenBuffer &sb, VRange range, JobInfo &job_info)
{
    job_info.thread_count++;
    std::thread th([&, range]() {
        
        std::mt19937 mt(static_cast<std::mt19937::result_type>(12345 * range.first));
        std::uniform_real_distribution<real> dist(0.0, 1.0);
        
        for(size_t j = range.first; j < range.first + range.second; ++j) {//vertical
            for(size_t i = 0; i < sb.w; ++i) {//horizontal
                for(size_t sub_sample = 0; sub_sample < cam.sub_samples ; ++sub_sample) {
                    Ray ray = cam.get_ray(sb, i, j, dist(mt), dist(mt));
                    for(size_t sample = 0; sample < cam.samples; ++sample) {
                        sb.color(j,i) += get_radiance(ray, scene, mt()) / (cam.samples * cam.sub_samples);
                    }
                }
            }
            job_info.finished_line_count++;
            size_t count = job_info.finished_line_count;
            printf("%zd / %zd\n", count, kHEIGHT);//log
        }
        
        job_info.thread_count--;
        job_info.notify_one();
    });
    th.detach();
}
