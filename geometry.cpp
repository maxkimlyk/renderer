#include "geometry.h"

mat4<float> RotationMatrixX(float angle)
{
    mat4<float> result = {
        1,  0,              0,              0,
        0,  cosf(angle),    -sinf(angle),   0,
        0,  sinf(angle),    cosf(angle),    0,
        0,  0,              0,              1
    };
    return result;
}

mat4<float> RotationMatrixY(float angle)
{
    mat4<float> result = {
        cosf(angle),    0,  -sinf(angle),   0,
        0,              1,  0,              0,
        sinf(angle),    0,  cosf(angle),    0,
        0,              0,  0,              1
    };
    return result;
}

mat4<float> MoveMatrix(float x, float y, float z)
{
    mat4<float> result = {
        1,      0,      0,      x,
        0,      1,      0,      y,
        0,      0,      1,      z,
        0,      0,      0,      1
    };
    return result;
}
