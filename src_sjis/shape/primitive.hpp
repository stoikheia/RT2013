/**
 * 
 * @file primitive.h
 * @author Reiji Tokuda.
 *
 * Interface file of primitive in RT2013 on 2013/06/12.
 * NoN NRVO.
 * all classes in this file are POD.
 */

#ifndef primitive_H
#define primitive_H

#include <algorithm>
#include <limits>
#include <cmath>
//#include <exception>
#include <cassert>

typedef double real;

//unko macro
#define self (*this)

template<typename T>
struct type_traits {
    static T zero();
    static T one();
    static T inf();
};
//template<typename T> struct zero {static const T value;};//this way is mendoi
template<> struct type_traits<float> {
    static float zero() {return 0.0f;}
    static float one() {return 1.0f;}
    static float inf() {return std::numeric_limits<float>::max();}
};
template<>
struct type_traits<double> {
    static double zero() {return 0.0;}
    static double one() {return 1.0;}
    static double inf() {return std::numeric_limits<double>::max();}
};

template <typename T> struct is_commutative_property_plus;
template <> struct is_commutative_property_plus<float> : public std::true_type {};
template <> struct is_commutative_property_plus<double> : public std::true_type {};

template <typename T> struct is_commutative_property_mul;
template <> struct is_commutative_property_mul<float> : public std::true_type {};
template <> struct is_commutative_property_mul<double> : public std::true_type {};


// < delived classes cannot aggregated-initialize and these cannot replace initializer-list.
// < std::array cannot be used because there is no initializer_list in VC.
template<size_t M, size_t N>
struct DMat {//density matrix
        
    static_assert(M != 0 && N != 0, "DMat class cannot allow zero row or zero col.");
    typedef real* iterator;
    typedef const real* const_iterator;
        
    real e[M*N];
    
    DMat() = default;
    DMat(const DMat &mat) = default;
    DMat(DMat &&) = default;
    ~DMat() = default;
    DMat& operator=(const DMat &) = default;
    DMat& operator=(DMat &&) = default;
    
    DMat(real val)
    {std::fill(begin(), end(), val);}
    DMat(const real p[])
    {std::copy(p, p+M*N, begin());}
        
    static const size_t m = M;
    static const size_t n = N;
    
    real& get(size_t i,size_t j) {
        assert(i < M && j < N);
        return e[i*N + j];
    }
    real get(size_t i,size_t j) const {
        assert(i < M && j < N);
        return e[i*N + j];
    }
    iterator begin() {return e;}
    iterator end() {return (&e[0])+M*N;}
    const_iterator begin() const {return e;}
    const_iterator end() const {return (&e[0])+M*N;}
    
    real& operator[](size_t ij) {assert(ij < M*N); return e[ij];}
    real operator[](size_t ij) const {assert(ij < M*N); return e[ij];}
    
    DMat& operator=(real val) {
        std::fill(begin(), end(), val);
        return self;
    }
    
    static DMat identity() {
        assert(M == N);
        DMat ret(0.0);
        for(size_t i = 0; i < M; ++i) {
            ret.get(i,i) = 1.0;
        }
        return ret;
    }
    
    template<typename INDMat>
    DMat<M,INDMat::n> operator*(const INDMat &mat) {
        static_assert(N == INDMat::m, "Must be Num of Left Row == Num of Right Col");
        DMat<M,INDMat::n> ret(0.0);
        for(size_t l = 0; l < mat.n; ++l) {//ret-col r-col
            for(size_t i = 0; i < M; ++i) {//l-row ret-row
                for(size_t j = 0; j < N; ++j) {//l-col r-row
                    ret.get(i,l) += self.get(i,j) * mat.get(j,l);
                }
            }
        }
        return ret;
    }
    DMat& operator*=(const DMat &mat) {
        self = self * mat;
        return self;
    }
};

struct Vec2 : DMat<1,2> {
    
    typedef real* iterator;
    typedef const real* const_iterator;
    static const size_t N = 2;
        
    //POD
    Vec2() = default;
    Vec2(const Vec2 &) = default;
    Vec2(Vec2 &&) = default;
    ~Vec2() = default;
    Vec2& operator=(const Vec2 &) = default;
    Vec2& operator=(Vec2 &&) = default;
    
    Vec2(real val) {std::fill(begin(), end(), val);}
    Vec2(real x, real y) {e[0]=x;e[1]=y;}
    Vec2(const real *p) {std::copy(p, p+N, begin());}
    
    bool operator==(const Vec2 &vec) const {
        return 0==memcmp(this, &vec, sizeof(Vec2));
    }
    
    bool equal_aproximation(const Vec2 &vec) const {
        return (std::abs(self.e[0] - vec.e[0]) < 1.0e-10 &&
                std::abs(self.e[1] - vec.e[1]) < 1.0e-10);
    }
    
    iterator begin() {return e;}
    iterator end() {return (&e[0])+N;}
    const_iterator begin() const {return e;}
    const_iterator end() const {return (&e[0])+N;}
    
    real& operator[](size_t i) {assert(i < N); return e[i];}
    real operator[](size_t i) const {assert(i < N); return e[i];}

    real& x() {return self[0];}
    real& y() {return self[1];}
    real x() const {return self[0];}
    real y() const {return self[1];}
    
    real& u() {return self[0];}
    real& v() {return self[1];}
    real u() const {return self[0];}
    real v() const {return self[1];}
    
};
static_assert(std::is_pod<Vec2>::value, "Vec2 is not POD.");


struct Vec3 : DMat<1,3> {
    
    typedef real* iterator;
    typedef const real* const_iterator;
    static const size_t N = 3;
        
    //POD
    Vec3() = default;
    Vec3(const Vec3 &) = default;
    Vec3(Vec3 &&) = default;
    ~Vec3() = default;
    Vec3& operator=(const Vec3 &) = default;
    Vec3& operator=(Vec3 &&) = default;
    
    Vec3(real val) {std::fill(begin(), end(), val);}
    Vec3(real x, real y, real z) {e[0]=x;e[1]=y;e[2]=z;}
    Vec3(const real *p) {std::copy(p, p+N, begin());}
    
    bool operator==(const Vec3 &vec) const {
        return 0==memcmp(this, &vec, sizeof(Vec3));
    }
    
    bool equal_aproximation(const Vec3 &vec) const {
        return (std::abs(self.e[0] - vec.e[0]) < 1.0e-10 &&
                std::abs(self.e[1] - vec.e[1]) < 1.0e-10 &&
                std::abs(self.e[2] - vec.e[2]) < 1.0e-10);
    }
    
    iterator begin() {return e;}
    iterator end() {return (&e[0])+N;}
    const_iterator begin() const {return e;}
    const_iterator end() const {return (&e[0])+N;}
    
    real& operator[](size_t i) {assert(i < N); return e[i];}
    real operator[](size_t i) const {assert(i < N); return e[i];}
    
    
    real& x() {return e[0];}
    real& y() {return e[1];}
    real& z() {return e[2];}
    real x() const {return e[0];}
    real y() const {return e[1];}
    real z() const {return e[2];}
    
    //Plus Vec3
    Vec3 &operator+=(const Vec3 &vec) {
        self.x()+=vec.x(); self.y()+=vec.y(); self.z()+=vec.z();
        return self;
    }
    Vec3 operator+(const Vec3 &vec) const {
        return Vec3(self.x()+vec.x(), self.y()+vec.y(), self.z()+vec.z());
    }

    //Plus real
    Vec3 &operator+=(real val) {
        self.x()+=val; self.y()+=val; self.z()+=val;
        return self;
    }
    Vec3 operator+(real val) const {
        return Vec3(self.x()+val, self.y()+val, self.z()+val);
    }
    
    //Sub Vec3
    Vec3 &operator-=(const Vec3 &vec) {
        self.x()-=vec.x(); self.y()-=vec.y(); self.z()-=vec.z();
        return self;
    }
    Vec3 operator-(const Vec3 &vec) const {
        return Vec3(self.x()-vec.x(), self.y()-vec.y(), self.z()-vec.z());
    }
    
    //Sub real
    Vec3 &operator-=(real val) {
        self.x()-=val; self.y()-=val; self.z()-=val;
        return self;
    }
    Vec3 operator-(real val) const {
        return Vec3(self.x()-val, self.y()-val, self.z()-val);
    }
    
    //Mul real
    Vec3 &operator*=(real &val) {
        self.x()*=val; self.y()*=val; self.z()*=val;
        return self;
    }
    Vec3 operator*(real val) const {
        return Vec3(self.x()*val, self.y()*val, self.z()*val);
    }
    
    //Div real
    Vec3 &operator/=(real &val) {
        self.x()/=val; self.y()/=val; self.z()/=val;
        return self;
    }
    Vec3 operator/(real val) const {
        return Vec3(self.x()/val, self.y()/val, self.z()/val);
    }
    
    real dot(const Vec3 &vec) const {
        return self.x()*vec.x()+self.y()*vec.y()+self.z()*vec.z();
    }
    Vec3 cross(const Vec3 &vec) const {
        return Vec3(self.y()*vec.z()-self.z()*vec.y(),
                    self.z()*vec.x()-self.x()*vec.z(),
                    self.x()*vec.y()-self.y()*vec.x());
    }
    real norm() const {
        return self.x()*self.x()+self.y()*self.y()+self.z()*self.z();
    }
    real length() const {
        return std::sqrt(norm());
    }
    Vec3& to_normal() {
        real len = length();
        if(type_traits<real>::zero() != len) {
            self /= len;
        } else {
            assert(0);
        }
        return self;
    }

};
static_assert(std::is_pod<Vec3>::value, "Vec3 is not POD.");

struct Vec4 : DMat<1,4> {
    
    typedef real* iterator;
    typedef const real* const_iterator;
    static const size_t N = 4;
        
    //POD
    Vec4() = default;
    Vec4(const Vec4 &) = default;
    Vec4(Vec4 &&) = default;
    ~Vec4() = default;
    Vec4& operator=(const Vec4 &) = default;
    Vec4& operator=(Vec4 &&) = default;
    
    Vec4(real val) {std::fill(begin(), end(), val);}
    Vec4(real x, real y, real z, real w) {e[0]=x;e[1]=y;e[2]=z;e[3]=w;}
    Vec4(const real *p) {std::copy(p, p+N, begin());}
    
    bool operator==(const Vec4 &vec) const {
        return 0==memcmp(this, &vec, sizeof(Vec4));
    }
    
    bool equal_aproximation(const Vec4 &vec) const {
        return (std::abs(self.e[0] - vec.e[0]) < 1.0e-10 &&
                std::abs(self.e[1] - vec.e[1]) < 1.0e-10 &&
                std::abs(self.e[2] - vec.e[2]) < 1.0e-10 &&
                std::abs(self.e[3] - vec.e[3]) < 1.0e-10);
    }
    
    iterator begin() {return e;}
    iterator end() {return (&e[0])+N;}
    const_iterator begin() const {return e;}
    const_iterator end() const {return (&e[0])+N;}
    
    real& operator[](size_t i) {assert(i < N); return e[i];}
    real operator[](size_t i) const {assert(i < N); return e[i];}
    
    real& x() {return e[0];}
    real& y() {return e[1];}
    real& z() {return e[2];}
    real& w() {return e[3];}
    real x() const {return e[0];}
    real y() const {return e[1];}
    real z() const {return e[2];}
    real w() const {return e[3];}
    
    real& a() {return e[0];}
    real& r() {return e[1];}
    real& g() {return e[2];}
    real& b() {return e[3];}
    real a() const {return e[0];}
    real r() const {return e[1];}
    real g() const {return e[2];}
    real b() const {return e[3];}
    
    //Plus Vec4
    Vec4 &operator+=(const Vec4 &vec) {
        self.x()+=vec.x(); self.y()+=vec.y(); self.z()+=vec.z(); self.w()+=vec.w();
        return self;
    }
    Vec4 operator+(const Vec4 &vec) const {
        return Vec4(self.x()+vec.x(), self.y()+vec.y(), self.z()+vec.z(), self.w()+vec.w());
    }
    //Plus real
    Vec4 &operator+=(real val) {
        self.x()+=val; self.y()+=val; self.z()+=val; self.w()+=val;
        return self;
    }
    Vec4 operator+(real val) const {
        return Vec4(self.x()+val, self.y()+val, self.z()+val, self.w()+val);
    }
    //Sub real
    Vec4 &operator-=(real val) {
        self.x()-=val; self.y()-=val; self.z()-=val; self.w()+=val;
        return self;
    }
    Vec4 operator-(real val) const {
        return Vec4(self.x()-val, self.y()-val, self.z()-val, self.w()-val);
    }
    //Mul real
    Vec4 &operator*=(real &val) {
        self.x()*=val; self.y()*=val; self.z()*=val; self.w()*=val;
        return self;
    }
    Vec4 operator*(real val) const {
        return Vec4(self.x()*val, self.y()*val, self.z()*val, self.w()*val);
    }
    //Div real
    Vec4 &operator/=(real &val) {
        self.x()/=val; self.y()/=val; self.z()/=val; self.w()/=val;
        return self;
    }
    Vec4 operator/(real val) const {
        return Vec4(self.x()/val, self.y()/val, self.z()/val, self.w()/val);
    }
    
};
static_assert(std::is_pod<Vec4>::value, "Vec4 is not POD.");


struct Vertex {
    
    typedef real scalar_t;
    
    Vec3 p;//pos
    Vec3 n;//normal
    Vec2 t;//tex
    size_t m;//mat id
    
    //POD
    Vertex() = default;
    Vertex(const Vertex &) = default;
    Vertex(Vertex &&) = default;
    ~Vertex() = default;
    Vertex& operator=(const Vertex &) = default;
    Vertex& operator=(Vertex &&) = default;
    
    Vertex(const Vec3 &p_, const Vec3 &n_, const Vec2 &t_, size_t m_)
    :p(p_),n(n_),t(t_),m(m_) {}
    Vertex(const real *p_, const real *n_, const real *t_, size_t m_)
    :p(p_),n(n_),t(t_),m(m_) {}
    Vertex(real p_, real n_, real t_, size_t m_)
    :p(p_),n(n_),t(t_),m(m_) {}
};
static_assert(std::is_pod<Vertex>::value, "Vertex is not POD.");

#undef self

#endif // primitive_H
