#include <iostream>
#include <cfloat>
#include "tgaimage.h"
#include "render.h"
#include "model.h"

mat4f lookAt(vec3f eye, vec3f center, vec3f up)
{
    vec3f z = Normalize(eye - center);
    vec3f x = Normalize(Cross(up, z));
    vec3f y = Normalize(Cross(z, x));

    mat4f minv = mat4f::Identity();
    mat4f tr = mat4f::Identity();

    minv._11 = x.x; minv._12 = x.y; minv._13 = x.z;
    minv._21 = y.x; minv._22 = y.y; minv._23 = y.z;
    minv._31 = z.x; minv._32 = z.y; minv._33 = z.z;

    tr._14 = -center.x;
    tr._24 = -center.y;
    tr._34 = -center.z;
    
    return minv * tr;
}

mat4f viewport(int x, int y, int width, int height)
{
    mat4f matrix = mat4f::Identity();
    matrix._11 = width / 2.0f;
    matrix._22 = height / 2.0f;
    matrix._33 = 1.0f;
    matrix._14 = x + width / 2.0f;
    matrix._24 = y + height / 2.0f;
    matrix._34 = 1.0f;
    return matrix;
}

void ModelTest()
{
    const size_t width = 1024;
    const size_t height = 1024;
    const float  ZBUF_VALUE = -10000;

    Model model;
    model.Read("model.obj");

    TGAImage texture;
    texture.read_tga_file("african_head_diffuse.tga");
    texture.flip_vertically();
    
    TGAImage image(width, height, TGAImage::RGB);
    float zBuffer[width * height];
    for (size_t i = 0; i < width * height; i++)
    {
        zBuffer[i] = ZBUF_VALUE;
    }

    mat4f modelView = lookAt(vec3f(-5, 1, 10), vec3f(0, 0, 0), vec3f(0, 1, 0));
    mat4f normMat = Invert(Transpose(modelView));
    mat4f viewMatrix = viewport(0, 0, width, height) * modelView;

    vec3f lightDir = vec3f(0.5, 0, 0.7);

    for (uint32_t i = 0; i < model.NumFaces(); i++)
    {
        Face face = model.GetFace(i);

        vec4f v1 (face.v[0].geom, 1);
        vec4f v2 (face.v[1].geom, 1);
        vec4f v3 (face.v[2].geom, 1);

        v1 = viewMatrix * v1;
        v2 = viewMatrix * v2;
        v3 = viewMatrix * v3;

        vec3f screen1 = vec3f (v1.x / v1.w, v1.y / v1.w, v1.z / v1.w);
        vec3f screen2 = vec3f (v2.x / v2.w, v2.y / v2.w, v2.z / v2.w);
        vec3f screen3 = vec3f (v3.x / v3.w, v3.y / v3.w, v3.z / v3.w);

        vec4f norm1 = vec4f (face.v[0].norm.x, face.v[0].norm.y, face.v[0].norm.z, 0);
        vec4f norm2 = vec4f (face.v[1].norm.x, face.v[1].norm.y, face.v[1].norm.z, 0);
        vec4f norm3 = vec4f (face.v[2].norm.x, face.v[2].norm.y, face.v[2].norm.z, 0);

        norm1 = normMat * norm1;
        norm2 = normMat * norm2;
        norm3 = normMat * norm3;

        vec3f screenNorm1 = Normalize( vec3f (norm1.x, norm1.y, norm1.z) );
        vec3f screenNorm2 = Normalize( vec3f (norm2.x, norm2.y, norm2.z) );
        vec3f screenNorm3 = Normalize( vec3f (norm3.x, norm3.y, norm3.z) );

        BarRasterize(screen1, screen2, screen3,
                     face.v[0].tex, face.v[1].tex, face.v[2].tex,
                     screenNorm1, screenNorm2, screenNorm3,
                     zBuffer, &texture, lightDir, &image);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

int main(int argc, char** argv)
{
    ModelTest();

    return 0;
}
