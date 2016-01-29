#pragma once

#include <vector>
#include <algorithm>

template <typename number>
inline number clip(number n, number min, number max)
{
    return std::min(std::max(n, min), max);
}

template <typename number>
class vec3
{
public:
    static vec3<number> invalidVector() { return vec3{false}; }
    vec3() : x{0}, y{0}, z{0} {};
    vec3(bool valid) : x{0}, y{0}, z{0}, valid{valid} {};
    vec3(number fill) : x{fill}, y{fill}, z{fill} {};

    template <typename container_type>
    vec3(container_type container)
    {
        assert(container.size() == 3);
        auto it = container.begin();
        x = *(it++);
        y = *(it++);
        z = *(it++);
    }


    vec3(number x, number y, number z) : x{x}, y{y}, z{z} {};
    ~vec3(){};

    vec3<number> operator+(const vec3<number> &v) const
    {
        return vec3{x + v.x, y + v.y, z + v.z};
    }
    vec3<number> operator-(const vec3<number> &v) const
    {
        return vec3{x - v.x, y - v.y, z - v.z};
    }
    vec3<number> operator*(const vec3<number> &v) const
    {
        return vec3{x * v.x, y * v.y, z * v.z};
    }
    vec3<number> operator/(const vec3<number> &v) const
    {
        return vec3{x / v.x, y / v.y, z / v.z};
    }
    vec3<number> operator+(const number &fill) const
    {
        return operator+(vec3{fill});
    }
    vec3<number> operator-(const number &fill) const
    {
        return operator-(vec3{fill});
    }
    vec3<number> operator*(const number &fill) const
    {
        return operator*(vec3{fill});
    }
    vec3<number> operator/(const number &fill) const
    {
        return operator/(vec3{fill});
    }

    vec3<number> &operator+=(const vec3<number> &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    number dot(const vec3<number> &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    vec3<number> cross(const vec3<number> &v) const
    {
        return vec3{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

    double length() const { return sqrt(x * x + y * y + z * z); }
    float lengthSquared() const { return x * x + y * y + z * z; }
    vec3<number> clamp(const number &min, const number &max) const
    {
        return vec3<number>{clip(x, min, max), clip(y, min, max),
                            clip(z, min, max)};
    }

    // I - 2.0 * dot(N, I) * N
    vec3<number> reflect(const vec3<number> &normal) const
    {
        return operator-(normal * static_cast<number>(2.0 * dot(normal)));
    }

    vec3<number> normalize() const { return operator/(length()); }
    number operator[](const int &i) const
    {
        switch (i)
        {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
        }
        return 0;
    }

    bool isValid() const { return valid; }
    number max() const { return std::max(x, std::max(y, z)); }
    number x;
    number y;
    number z;

private:
    bool valid = true;
};

template <typename number>
std::ostream &operator<<(std::ostream &os, vec3<number> const &v)
{
    const std::string representation = "vec3(" + std::to_string(v.x) + ", " +
                                       std::to_string(v.y) + ", " +
                                       std::to_string(v.z) + ")";
    return os << representation;
}
