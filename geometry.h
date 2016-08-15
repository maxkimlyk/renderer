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

#pragma pack(push, 1)
template <typename type> struct vec4
{
    type x, y, z, w;

    vec4()
    {}

    vec4 (type x, type y, type z, type w):
      x(x), y(y), z(z), w(w)
    {}

    vec4 (vec3<type> v3, type w):
      x(v3.x), y(v3.y), z(v3.z), w(w)
    {}

    type& operator() (size_t i)
    {
        type *base = (type*)(this);
        return base[i];
    }

    const type& operator() (size_t i) const
    {
        type *base = (type*)(this);
        return base[i];
    }
};
#pragma pack(pop)

typedef vec2<int>   vec2i;
typedef vec2<float> vec2f;
typedef vec3<int>   vec3i;
typedef vec3<float> vec3f;
typedef vec4<int>   vec4i;
typedef vec4<float> vec4f;

// **************************************************************************
template <typename type> struct mat4
{
    union
    {
        struct
        {
            type _11, _12, _13, _14;
            type _21, _22, _23, _24;
            type _31, _32, _33, _34;
            type _41, _42, _43, _44;
        };
        type _[16];
    };

    type& operator() (size_t i, size_t j)
    {
        return _[i*4 + j];
    }

    const type& operator() (size_t i, size_t j) const
    {
        return _[i*4 + j];
    }

    static mat4<type> Identity()
    {
        mat4<type> I = {
            1,  0,  0,  0,
            0,  1,  0,  0,
            0,  0,  1,  0,
            0,  0,  0,  1
        };
        return I;
    }

    type Cofactor(size_t i, size_t j) const
    {
        type mat3[3][3];

        int k = 0;
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
            {
                if (y != (int)i && x != (int)j)
                {
                    mat3[k/3][k%3] = _[y*4 + x];
                    k++;
                }
            }

        type result = mat3[0][0] * (mat3[1][1] * mat3[2][2] - mat3[1][2] * mat3[2][1]) -
                      mat3[0][1] * (mat3[1][0] * mat3[2][2] - mat3[1][2] * mat3[2][0]) +
                      mat3[0][2] * (mat3[1][0] * mat3[2][1] - mat3[1][1] * mat3[2][0]);

        if (i % 2 != j % 2)
            result *= -1;

        return result;
    }
};

mat4<float> RotationMatrixX(float angle);
mat4<float> RotationMatrixY(float angle);
mat4<float> MoveMatrix(float x, float y, float z);

template <typename type> mat4<type> operator* (const mat4<type> &mat1, const mat4<type> &mat2)
{
    mat4<type> result;

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
        {
            type sum = 0;
            for (size_t k = 0; k < 4; k++)
                sum += mat1(i, k) * mat2(k, j);
            result(i, j) = sum;
        }
    return result;
}

template <typename type> vec4<type> operator * (const mat4<type> &mat, const vec4<type> &vect)
{
    vec4<type> result;
    for (size_t i = 0; i < 4; i++)
    {
        type sum = 0;
        for (size_t k = 0; k < 4; k++)
            sum += mat(i, k) * vect(k);
        result(i) = sum;
    }
    return result;
}

template <typename type> mat4<type> operator * (const mat4<type> &mat, type coef)
{
    mat4<type> result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            result(i, j) = mat(i, j) * coef;
        }
    return result;
}

template <typename type> mat4<type> Transpose (const mat4<type> &mat)
{
    mat4<type> result;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            result(i, j) = mat(j, i);
        }
        
    return result;
}

template <typename type> type Determ (const mat4<type> &mat)
{
    type result = 0;
    for (int i = 0; i < 4; i++)
    {
        result += mat(0, i) * mat.Cofactor(0, i);
    }
    return result;
}

template <typename type> mat4<type> Invert (const mat4<type> &mat)
{
    mat4<type> result;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            result(i, j) = mat.Cofactor(i, j);
        }

    return Transpose(result) * (1.0f / Determ(mat));
}

typedef mat4<float> mat4f;

#endif
