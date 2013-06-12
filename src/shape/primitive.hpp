/**
 * 
 * @file primitive.h
 * @author Reiji Tokuda.
 *
 * Interface file of primitive in RT2013 on 2013/06/12.
 * 
 */

#ifndef primitive_H
#define primitive_H

#include <array>
#include <initializer_list>
//#include <exception>
#include <cassert>

template<typename T>
struct type_traits {
    static T zero();
    static T one();
};
template<>
struct type_traits<float> {
    static float zero() {return 0.0f;}
    static float one() {return 1.0f;}
};
template<>
struct type_traits<double> {
    static double zero() {return 0.0;}
    static double one() {return 1.0;}
};


template<typename ScalarT, std::size_t M , std::size_t N>
struct Mat {
    
    typedef ScalarT scalar_t;
    
    //std::array is movable only if its contained objects are movable.
    std::auto_ptr<std::array<ScalarT, M*N> > p;
    
    Mat()
    :p(new std::array<ScalarT, M*N>()) {}
    Mat(const ScalarT val)
    :Mat()
    {std::fill(p.get()->begin(), p.get()->end(), val);}
    Mat(const std::initializer_list<ScalarT> &list)
    :Mat() {
        static_assert(M != 0 && N != 0, "Mat is initialized with invalid num of row or num of col.");
        assert(list.size() == M*N);
        std::copy(list.begin(), list.end(), p->begin());
    }
    Mat(const Mat &input)
    :p(new std::array<ScalarT, M*N>(*(input.p.get()))) {}
    Mat(Mat &&input)
    :p(std::move(input.p)) {}
    
    size_t m() const {return M;}
    size_t n() const {return N;}
    
    ScalarT &get(size_t m, size_t n) {
        assert(m < M && n < N);
        return (*p)[m*N+n];
    }
    ScalarT get(size_t m, size_t n) const {
        assert(m < M && n < N);
        return p[m*N+n];
    }
    
    static Mat &&Identity() {
        assert(M == N);
        Mat mat(type_traits<ScalarT>::zero());
        for (size_t i = 0; i < M; ++i) {
            mat.get(i,i) == type_traits<ScalarT>::one();
        }
        return mat;//NRVO
    }
};


template<typename ScalarT>
struct Vec3 : public Mat<ScalarT, 3, 1> {
    
    Vec3(ScalarT val = type_traits<ScalarT>::zero())
    :Mat<ScalarT,3,1>(val) {}
    Vec3(ScalarT x, ScalarT y, ScalarT z)
    :Mat<ScalarT,3,1>({x,y,z}) {}
    Vec3(const std::initializer_list<ScalarT> &list)
    :Mat<ScalarT,3,1>(list) {}
    Vec3(const Vec3 &input)
    :Mat<ScalarT,3,1>(input) {}
    Vec3(Vec3 &&input)
    :Mat<ScalarT,3,1>(std::move(input)) {}
    
    ScalarT& x() {return this->p->at(0);}
    ScalarT& y() {return this->p->at(1);}
    ScalarT& z() {return this->p->at(2);}
    ScalarT x() const {return this->p->at(0);}
    ScalarT y() const {return this->p->at(1);}
    ScalarT z() const {return this->p->at(2);}
    
    Vec3& operator=(const Vec3 &input) {
        this->p = input.p;
        return *this;
    }
    Vec3 &operator=(Vec3 &&input) {
        this->p = std::move(input.p);
        return *this;
    }
    
};

struct Shape {
    Shape() {}
};

struct Triangle : Shape {
    std::array<size_t, 3> v;//indexes
    
    Triangle(size_t v0, size_t v1, size_t v2)
    :v({{v0,v1,v2}}) {}
    Triangle(const std::initializer_list<size_t> &list)
    :v() {assert(list.size() == 3); std::copy(list.begin(), list.end(), v.begin());}
};

template<typename ScalarT>
struct Sphere : Shape {
    Vec3<ScalarT> pos;
    ScalarT r;
    Sphere(const Vec3<ScalarT> &input_pos, ScalarT input_r)
    :pos(input_pos),r(input_r) {}
};


template<typename ScalarT>
struct AABB {
    
    Vec3<ScalarT> min_corner;
    Vec3<ScalarT> max_corner;
    
    AABB() {}
    AABB(const Vec3<ScalarT> &input_min_corner, const Vec3<ScalarT> &input_max_corner)
    :min_corner(input_min_corner),max_corner(input_max_corner) {}
    AABB(const AABB &input)
    :min_corner(input.min_corner),max_corner(input.max_corner) {}
    AABB(AABB &&input)
    :min_corner(std::move(input.min_corner)),max_corner(std::move(input.max_corner)) {}
    
    template<typename Contena>
    AABB(const Triangle &input, const Contena &vertexes) {
        static_assert(std::is_same<typename Contena::iterator::iterator_category, std::random_access_iterator_tag>::value == true,
                      "AABB triangle initializer doesnt accept if it is not random access iterator.");
        min_corner = input.v[0]; max_corner = input.v[0];
        for(size_t i = 1; i < 3; ++i) {
            for(size_t j = 0; j < 3; ++j) {
                min_corner.p->at(j) = std::min(min_corner.p->at(j), vertexes[input.v[i]].p->at(j));
                max_corner.p->at(j) = std::max(max_corner.p->at(j), vertexes[input.v[i]].p->at(j));
            }
        }
    }
    
    AABB(const Sphere<ScalarT> &input)
    :min_corner(input.pos.x() - input.r, input.pos.y() - input.r, input.pos.z() - input.r),
    max_corner(input.pos.x() + input.r, input.pos.y() + input.r, input.pos.z() + input.r) {}
    
    void unionize(const AABB &input) {
        for(size_t i = 0; i < 3; ++i) {
            this->min_corner.p->at(i) = std::min(*(this->min_corner.p)[i], *(input.min_corner.p)[i]);
            this->max_corner.p->at(i) = std::max(*(this->max_corner.p)[i], *(input.max_corner.p)[i]);
        }
    }
};



#endif // primitive_H
