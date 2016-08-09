#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <assert.h>
#include <iostream>
#include <cmath>

template <typename number, size_t dimensions>
struct vec
{
	number coord[dimensions];

	vec()
	{}

	number& operator[](size_t i)
	{
		assert(i < dimensions);
		return coord[i];
	}
};

template <typename number>
struct vec<number, 2>
{
	number x, y;

	vec()
	{}

	vec(number x, number y) :
		x(x), y(y) {}

	number& operator[](size_t i)
	{
		assert(i < 2);
		return (&x)[i];
	}
};

template <typename number>
struct vec<number, 3>
{
	number x, y, z;

	vec()
	{}

	vec(number x, number y, number z) :
		x(x), y(y), z(z) {}

	number& operator[](size_t i)
	{
		assert(i < 3);
		return (&x)[i];
	}
};

template <typename num, size_t dim>
vec<num, dim> operator+ (vec<num, dim> v1, vec<num, dim> &v2)
{
	for (size_t i = 0; i < dim; i++)
		v1[i] += v2[i];
	return v1;
}

template <typename num, size_t dim>
vec<num, dim> operator- (vec<num, dim> v1, vec<num, dim> &v2)
{
	for (size_t i = 0; i < dim; i++)
		v1[i] -= v2[i];
	return v1;
}

template <typename num, size_t dim, typename constant>
vec<num, dim> operator* (vec<num, dim> v, constant c)
{
	for (size_t i = 0; i < dim; i++)
		v[i] *= c;
	return v;
}

template <typename num, size_t dim, typename constant>
vec<num, dim> operator* (constant c, vec<num, dim> v)
{
	for (size_t i = 0; i < dim; i++)
		v[i] *= c;
	return v;
}

template <typename num, size_t dim, typename constant>
vec<num, dim> operator/ (vec<num, dim> v, constant c)
{
	for (size_t i = 0; i < dim; i++)
		v[i] /= c;
	return v;
}

template <typename num, size_t dim>
num operator* (vec<num, dim> &v1, vec<num, dim> &v2)
{
	num sum = 0;
	for (size_t i = 0; i < dim; i++)
		sum += v1[i] * v2[i];
	return sum;
}

template <typename num, size_t dim>
std::ostream& operator<< (std::ostream& stream, vec<num, dim> &v)
{
	stream << "(";
	for (size_t i = 0; i < dim - 1; i++)
		stream << v[i] << ",";
	stream << v[dim - 1] << ")";
	return stream;
}

template <typename num>
vec<num, 3> Cross(vec<num, 3> &v, vec<num, 3> &u)
{
	return vec<num, 3>(v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z, v.x * u.y - v.y * u.x);
}

template <typename num, size_t dim>
vec<num, dim> Normalize(vec<num, dim> v)
{
	float length = sqrt(v * v);
	if (length > 0)
		v = v / length;
	return  v;
}

template <typename num, size_t dim>
vec<num, dim + 1> Embed(vec<num, dim> &v, num fill = 1)
{
	vec<num, dim + 1> res;
	for (size_t i = 0; i < dim; i++)
		res[i] = v[i];
	res[dim] = fill;
	return res;
}

template <typename num, size_t dim>
vec<num, dim - 1> Eject(vec<num, dim> &v)
{
	vec<num, dim - 1> res;
	for (size_t i = 0; i < dim - 1; i++)
		res[i] = v[i];
	return res;
}

template <typename num, size_t dim>
vec<int, dim> Round(vec<num, dim> &v)
{
    vec<int, dim> res;
    for (size_t i = 0; i < dim; i++)
        res[i] = round(v[i]);
    return res;
}

typedef vec<int, 2>   vec2i;
typedef vec<float, 2> vec2f;
typedef vec<int, 3>   vec3i;
typedef vec<float, 3> vec3f;

#endif
