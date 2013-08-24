/**
 * 
 * @file screen_buffer.h
 * @author Reiji Tokuda.
 *
 * Interface file of screen_buffer in RT2013 on 2013/08/24.
 * 
 */

#ifndef screen_buffer_H
#define screen_buffer_H

#include "shape/primitive.hpp"

#include <vector>
#include <fstream>

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
    
    uint8_t convert_display_color(real val) {
        if(0.0 > val) {
            return 0;
        } else if(1.0 <= val){
            return 255;
        } else {
            return pow(val, 1.0/2.2) * 255.0 + 0.5;
        }
    }
    
    void write_bitmap(std::ofstream &ofs) {
        
        const uint32_t file_header_size = 14;
        const uint32_t info_header_size = 40;
        
        //file header
        ofs.write("BM", 2);//bfType
        assert(w <= std::numeric_limits<uint16_t>::max());
        assert(h <= std::numeric_limits<uint16_t>::max());
        uint32_t buff_size = (uint32_t)w * (uint32_t)h * 3;
        uint32_t fsize = buff_size + file_header_size + info_header_size;
        uint32_t zero = 0;
        ofs.write(reinterpret_cast<const char*>(&fsize), 4);//bfSize
        ofs.write(reinterpret_cast<const char*>(&zero), 2);//bfReserved1
        ofs.write(reinterpret_cast<const char*>(&zero), 2);//bfReserved2
        uint32_t offset = file_header_size + info_header_size;
        ofs.write(reinterpret_cast<char*>(&offset), 4);//bfOffBits
        //info header
        ofs.write(reinterpret_cast<const char*>(&info_header_size), 4);//biSize
        ofs.write(reinterpret_cast<const char*>(&w), 4);//biWidth
        ofs.write(reinterpret_cast<const char*>(&h), 4);//biHeight
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
        for(size_t j = h; j > 0; --j) {//vertical
            for(size_t i = 0; i < w; ++i) {//horizontal
                const Vec4 &argb = this->const_color(j-1,i);
                uint8_t r = convert_display_color(argb.r());
                uint8_t g = convert_display_color(argb.g());
                uint8_t b = convert_display_color(argb.b());
                ofs.write(reinterpret_cast<const char*>(&b),1);
                ofs.write(reinterpret_cast<const char*>(&g),1);
                ofs.write(reinterpret_cast<const char*>(&r),1);
            }
        }
    }
};



#endif // screen_buffer_H
