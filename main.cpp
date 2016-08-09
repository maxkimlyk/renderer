#include <iostream>
#include <cfloat>
#include "tgaimage.h"
#include "render.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor yellow = TGAColor(200, 210, 0, 255);

void LineTest()
{
    TGAImage image(100, 100, TGAImage::RGB);
    Line(vec2i(0, 0), vec2i(99, 99), yellow, &image);
    Line(vec2i(73, 23), vec2i(9, 99), red, &image);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("lines.tga");
}

void TriangleTest()
{
    TGAImage image(200, 200, TGAImage::RGB);

    Triangle(vec2i(10, 10), vec2i(60, 90), vec2i(90, 10), white, &image);
    Triangle(vec2i(110, 90), vec2i(160, 10), vec2i(190, 90), white, &image);
    Triangle(vec2i(10, 110), vec2i(10, 190), vec2i(90, 110), white, &image);
    Triangle(vec2i(110, 110), vec2i(190, 190), vec2i(190, 110), white, &image);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("triangle.tga");
}

void RasterizeTest()
{
    TGAImage image(200, 200, TGAImage::RGB);

    //Rasterize(vec3f(10, 10, 10), vec3f(60, 90, 10), vec3f(90, 10, 10), white, &image);
    //Rasterize(vec3f(110, 90, 10), vec3f(160, 10, 10), vec3f(190, 90, 10), white, &image);
    //Rasterize(vec3f(10, 110, 10), vec3f(10, 190, 10), vec3f(90, 110, 10), white, &image);
    //Rasterize(vec3f(110, 110, 10), vec3f(190, 190, 10), vec3f(190, 110, 10), white, &image);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("rasterize.tga");
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

    for (uint32_t i = 0; i < model.NumFaces(); i++)
    {
        Face face = model.GetFace(i);
        Vertex vertex1 = face.v[0];
        Vertex vertex2 = face.v[1];
        Vertex vertex3 = face.v[2];

        vertex1.geom.x = (vertex1.geom.x + 1) * width / 2.0f;
        vertex1.geom.y = (vertex1.geom.y + 1) * height / 2.0f;
        vertex2.geom.x = (vertex2.geom.x + 1) * width / 2.0f;
        vertex2.geom.y = (vertex2.geom.y + 1) * height / 2.0f;
        vertex3.geom.x = (vertex3.geom.x + 1) * width / 2.0f;
        vertex3.geom.y = (vertex3.geom.y + 1) * height / 2.0f;

        Rasterize(vertex1, vertex2, vertex3, zBuffer, &texture, lightDir, &image, i);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

int main(int argc, char** argv)
{
    LineTest();
    TriangleTest();
    RasterizeTest();
    ModelTest();

    return 0;
}
