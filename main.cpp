#include <iostream>
#include <cfloat>
#include "tgaimage.h"
#include "render.h"
#include "model.h"

const TGAColor COLOR_WHITE = TGAColor(255, 255, 255, 255);
const TGAColor COLOR_RED   = TGAColor(255, 0,   0,   255);
const TGAColor COLOR_YELLOW = TGAColor(200, 210, 0, 255);

void LineTest()
{
    TGAImage image(100, 100, TGAImage::RGB);
    Line(vec2i(0, 0), vec2i(99, 99), COLOR_YELLOW, &image);
    Line(vec2i(73, 23), vec2i(9, 99), COLOR_RED, &image);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("lines.tga");
}

void TriangleTest()
{
    TGAImage image(200, 200, TGAImage::RGB);

    Triangle(vec2i(10, 10), vec2i(60, 90), vec2i(90, 10), COLOR_WHITE, &image);
    Triangle(vec2i(110, 90), vec2i(160, 10), vec2i(190, 90), COLOR_WHITE, &image);
    Triangle(vec2i(10, 110), vec2i(10, 190), vec2i(90, 110), COLOR_WHITE, &image);
    Triangle(vec2i(110, 110), vec2i(190, 190), vec2i(190, 110), COLOR_WHITE, &image);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("triangle.tga");
}

void RasterizeTest()
{
    TGAImage image(200, 200, TGAImage::RGB);

    //Rasterize(vec3f(10, 10, 10), vec3f(60, 90, 10), vec3f(90, 10, 10), COLOR_WHITE, &image);
    //Rasterize(vec3f(110, 90, 10), vec3f(160, 10, 10), vec3f(190, 90, 10), COLOR_WHITE, &image);
    //Rasterize(vec3f(10, 110, 10), vec3f(10, 190, 10), vec3f(90, 110, 10), COLOR_WHITE, &image);
    //Rasterize(vec3f(110, 110, 10), vec3f(190, 190, 10), vec3f(190, 110, 10), COLOR_WHITE, &image);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("rasterize.tga");
}

template <typename type> std::ostream& operator<< (std::ostream &stream, mat4<type> &mat)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << mat(i, j) << " ";
        }
        std::cout << "\n";
    }
    return stream;
}

void ModelTest()
{
    const size_t width = 1024;
    const size_t height = 1024;
    const float  ZBUF_VALUE = -10000;
    
    TGAImage image(width, height, TGAImage::RGB);
    float zBuffer[width * height];
    for (size_t i = 0; i < width * height; i++)
    {
        zBuffer[i] = ZBUF_VALUE;
    }

    Model model;
    model.Read("model.obj");

    TGAImage texture;
    texture.read_tga_file("african_head_diffuse.tga");
    texture.flip_vertically();

    vec3f lightDir = Normalize( vec3f(0, 0, 1) );
    float cameraPos = -10.0f;

    mat4f transform = MoveMatrix(0, 0, 1) * RotationMatrixY(3.141592 / 8) * RotationMatrixX(3.141592 / 32);

    std::cout << transform;

    mat4f proj = mat4f::Identity();
    proj._43 = - 1.0f / cameraPos;

    for (uint32_t i = 0; i < model.NumFaces(); i++)
    {
        Face face = model.GetFace(i);
        Vertex vertex1 = face.v[0];
        Vertex vertex2 = face.v[1];
        Vertex vertex3 = face.v[2];

        vec4f v1 (vertex1.geom, 1);
        vec4f v2 (vertex2.geom, 1);
        vec4f v3 (vertex3.geom, 1);

        v1 = proj * transform * v1;
        v2 = proj * transform * v2;
        v3 = proj * transform * v3;

        vertex1.geom = vec3f (v1.x / v1.w, v1.y / v1.w, v1.z / v1.w);
        vertex2.geom = vec3f (v2.x / v2.w, v2.y / v2.w, v2.z / v2.w);
        vertex3.geom = vec3f (v3.x / v3.w, v3.y / v3.w, v3.z / v3.w);

        vertex1.geom.x = (vertex1.geom.x + 1) * width / 2.0f;
        vertex1.geom.y = (vertex1.geom.y + 1) * height / 2.0f;
        vertex2.geom.x = (vertex2.geom.x + 1) * width / 2.0f;
        vertex2.geom.y = (vertex2.geom.y + 1) * height / 2.0f;
        vertex3.geom.x = (vertex3.geom.x + 1) * width / 2.0f;
        vertex3.geom.y = (vertex3.geom.y + 1) * height / 2.0f;

        printf("vertex: %f %f %f\n", v1.x, v2.y, v3.z);

        //Rasterize(vertex1, vertex2, vertex3, zBuffer, &texture, lightDir, &image, i);
        Rasterize(vertex1.geom, vertex2.geom, vertex3.geom,
                  vertex1.tex,  vertex2.tex,  vertex3.tex,
                  vertex1.norm, vertex2.norm, vertex3.norm,
                  zBuffer, &texture, lightDir, &image, i);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

void MatrixTest()
{
    mat4<float> matA = {
        5, 2, 3, 4,
        9, 2, 1, 2,
        0, 3, 8, 3,
        0, 9, 0, 0
    };
    
    mat4<float> matB = {
        1, 2, 8, 3,
        9, 8, 7, 0,
        1, 1, 1, 2,
        6, 0, 0, 1
    };
    
    mat4<float> matAB = matA * matB;
    std::cout << matAB << "\n";

    std::cout << matA << "\n";
    vec4f vec1 (1, 2, 3, 4);
    vec4f vec2 = matA * vec1;

    for (size_t i = 0; i < 4; i++)
    {
        std::cout << vec2(i) << " ";
    }
}

int main(int argc, char** argv)
{
    LineTest();
    TriangleTest();
    RasterizeTest();
    ModelTest();
    //MatrixTest();

    return 0;
}
