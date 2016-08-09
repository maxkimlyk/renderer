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

    vec3f lightDir = Normalize( vec3f(0.2, 0.3, 0.7) );

    for (uint32_t i = 0; i < model.NumFaces(); i++)
    {
        Face face = model.GetFace(i);
        vec3f vertex1 = face.v[0].geom;
        vec3f vertex2 = face.v[1].geom;
        vec3f vertex3 = face.v[2].geom;

        vec3f dir1 = vertex2 - vertex1;
        vec3f dir2 = vertex3 - vertex1;

        vec3f norm = Normalize( Cross(dir1, dir2) );
        float lightFactor = norm * lightDir;
        if (lightFactor < 0)
            continue;

        Color color = Color (255 * lightFactor, 255 * lightFactor, 255 * lightFactor);

        vec3f screen1 ((vertex1.x + 1) * width / 2.0, (vertex1.y + 1) * height / 2.0, vertex1.z);
        vec3f screen2 ((vertex2.x + 1) * width / 2.0, (vertex2.y + 1) * height / 2.0, vertex2.z);
        vec3f screen3 ((vertex3.x + 1) * width / 2.0, (vertex3.y + 1) * height / 2.0, vertex3.z);

        printf ("%f %f %f\n", screen1.y, screen2.y, screen3.y);

        Rasterize(screen1, screen2, screen3, color, &image, zBuffer, i);
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
