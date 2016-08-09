#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <cmath>
#include <cstdlib>

template <typename type> struct vec2
{
    type x, y;

    vec2()
    {}

    vec2(type x, type y):
      x(x), y(y)
    {}
};

template <typename type> float Length (const vec2<type> &v)
{
    return sqrt( v.x*v.x + v.y*v.y );
}

template <typename type> vec2<type> Normalize (vec2<type> v)
{
    float length = Length(v);
    if(length > 0)
    {
        v.x /= length;
        v.y /= length;
    }
    return v;
}

template <typename type> vec2<type> operator+ (const vec2<type> &v1, const vec2<type> &v2)
{
    return vec2<type> (v1.x + v2.x, v1.y + v2.y);
}

template <typename type> vec2<type> operator- (const vec2<type> &v1, const vec2<type> &v2)
{
    return vec2<type> (v1.x - v2.x, v1.y - v2.y);
}

template <typename type> type operator* (const vec2<type> &v1, const vec2<type> &v2)
{
    return  (v1.x * v2.x + v1.y * v2.y);
}

template <typename type> vec2<type> operator* (float coef, const vec2<type> &v)
{
    return vec2<type> (coef * v.x, coef * v.y);
}

template <typename type> vec2<type> operator* (const vec2<type> &v, float coef)
{
    return vec2<type> (v.x * coef, v.y * coef);
}

template <typename type> struct vec3
{
    type x, y, z;

    vec3()
    {}

    vec3(type x, type y, type z):
      x(x), y(y), z(z)
    {}
};

template <typename type> float Length(const vec3<type> &v)
{
    return sqrt( v.x*v.x + v.y*v.y + v.z*v.z );
}

template <typename type> vec3<type> Normalize (vec3<type> v)
{
    float length = Length(v);
    if (length > 0)
    {
        v.x /= length;
        v.y /= length;
        v.z /= length;
    }
    return v;
}

template <typename type> vec3<type> Cross(const vec3<type> &v, const vec3<type> &u)
{
	return vec3<type> (v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z, v.x * u.y - v.y * u.x);
}

template <typename type> vec3<type> operator+ (const vec3<type> &v1, const vec3<type> &v2)
{
    return vec3<type> (v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <typename type> vec3<type> operator- (const vec3<type> &v1, const vec3<type> &v2)
{
    return vec3<type> (v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template <typename type> type operator* (const vec3<type> &v1, const vec3<type> &v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

template <typename type> vec3<type> operator* (float coef, const vec3<type> &v)
{
    return vec3<type> (coef * v.x, coef * v.y, coef * v.z);
}

template <typename type> vec3<type> operator* (const vec3<type> &v, float coef)
{
    return vec3<type> (v.x * coef, v.y * coef, v.z * coef);
}

typedef vec2<int>   vec2i;
typedef vec2<float> vec2f;
typedef vec3<int>   vec3i;
typedef vec3<float> vec3f;

#endif
