/**
 * 
 * @file pair_util.hpp
 * @author Reiji Tokuda.
 *
 * Interface file of pair_util in RT2013 on 2013/06/16.
 * 
 */

#ifndef pair_util_H
#define pair_util_H

#include <map>

template<typename T1, typename T2>
struct pair_first_less {
    bool operator()(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2) const {
        return p1.first < p2.first;
    }
};

template<typename T1, typename T2>
struct pair_second_less {
    bool operator()(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2) const {
        return p1.second < p2.second;
    }
};

template<typename T1, typename T2>
struct pair_first_greater {
    bool operator()(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2) const {
        return p1.first > p2.first;
    }
};

template<typename T1, typename T2>
struct pair_second_greater {
    bool operator()(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2) const {
        return p1.second > p2.second;
    }
};

template<typename T1, typename T2>
struct pair_first_equal {
    bool operator()(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2) const {
        return p1.first == p2.first;
    }
};

template<typename T1, typename T2>
struct pair_second_equal {
    bool operator()(const std::pair<T1, T2> &p1, const std::pair<T1, T2> &p2) const {
        return p1.second == p2.second;
    }
};


template<typename T1, typename T2>
struct pair_set : public std::pair<T1, T2> {
    pair_set(const T1 &p1, const T2 &p2)
    :std::pair<T1,T2>(p1,p2) {}
    pair_set(T1 &&p1, T2 &&p2)
    :std::pair<T1,T2>(std::forward<T1>(p1),std::forward<T2>(p2)) {}
    pair_set(const pair_set &ps)
    :pair_set(ps.first, ps.second) {}
    pair_set(pair_set &&ps)
    :pair_set(std::forward<T1>(ps.first), std::forward<T2>(ps.second)) {}
    pair_set(const std::pair<T1,T2> &p)
    :pair_set(p.first, p.second) {}
    pair_set(std::pair<T1,T2> &&p)
    :pair_set(std::forward<T1>(p.first), std::forward<T2>(p.second)) {}
    
    pair_set& operator=(const pair_set &ps) {
        this->first = ps.first; this->second = ps.second;
        return *this;
    }
    pair_set& operator=(pair_set &&ps) {
        this->first = std::forward(ps.first); this->second = std::forward(ps.second);
        return *this;
    }
    bool operator==(const pair_set &ps) const {
        return (this->first == ps.first && this->second == ps.second) ||
        (this->first == ps.second && this->second == ps.first);
    }
    bool operator<(const pair_set &ps) const {
        if(std::min(this->first, this->second) < std::min(ps.first, ps.second)) {
            if(std::min(this->first, this->second) == std::min(ps.first, ps.second)) {
                return std::max(this->first, this->second) < std::max(ps.first, ps.second);
            }
            return true;
        }
        return false;
    }
};


#endif // pair_util_H
