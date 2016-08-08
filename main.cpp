#include <iostream>
#include "tgaimage.h"
#include "render.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char** argv)
{
    TGAImage image(1024, 1024, TGAImage::RGB);
    
    Model model;
    model.Read("model.obj");

    for (uint32_t i = 0; i < model.NumFaces(); i++)
    {
        Face face = model.GetFace(i);
        vec2f proj1 = Eject(face.v[0].geom);
        vec2f proj2 = Eject(face.v[1].geom);
        vec2f proj3 = Eject(face.v[2].geom);
        size_t width = 1024;
        size_t height = 1024;
        vec2i screen1 ((proj1.x + 1) * width / 2, (proj1.y + 1) * height/2);
        vec2i screen2 ((proj2.x + 1) * width / 2, (proj2.y + 1) * height/2);
        vec2i screen3 ((proj3.x + 1) * width / 2, (proj3.y + 1) * height/2);
        Line(screen1, screen2, white, &image);
        Line(screen2, screen3, white, &image);
        Line(screen3, screen1, white, &image);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    return 0;
}
