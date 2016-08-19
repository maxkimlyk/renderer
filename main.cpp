#include <iostream>
#include <cfloat>
#include "tgaimage.h"
#include "render.h"
#include "model.h"

void ModelTest()
{
    const char * MODEL_NAME = "model.obj";
    const char * TEXTURE_NAME = "african_head_diffuse.tga";
    const size_t WIDTH = 1024;
    const size_t HEIGHT = 1024;
    const float  ZBUF_VALUE = -10000;

    const vec3f EYE = vec3f(-5, 1, 10);
    const vec3f CENTER = vec3f(0, 0, 0);
    const vec3f UP = vec3f(0, 1, 0);

    Model model;
    model.Read(MODEL_NAME);

    TGAImage texture;
    texture.read_tga_file(TEXTURE_NAME);
    texture.flip_vertically();
    
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    
    float zBuffer[WIDTH * HEIGHT];
    for (size_t i = 0; i < WIDTH * HEIGHT; i++)
        zBuffer[i] = ZBUF_VALUE;

    mat4f modelView  = LookAt(EYE, CENTER, UP);
    mat4f normCorr   = Invert(Transpose(modelView));
    mat4f viewMatrix = Viewport(0, 0, WIDTH, HEIGHT) * modelView;

    vec3f lightDir = vec3f(0.5, 0, 0.7);

    for (uint32_t i = 0; i < model.NumFaces(); i++)
    {
        Face face = model.GetFace(i);

        vec3f screen[3];
        vec2f tex[3];
        vec3f norm[3];
        
        for (int k = 0; k < 3; k++)
        {
            vec4f vertex = vec4f (face.v[k].geom, 1);
            vertex = viewMatrix * vertex;
            screen[k] = vec3f (vertex.x / vertex.w, vertex.y / vertex.w, vertex.z / vertex.w);

            tex[k] = face.v[k].tex;

            vec4f rawNorm = vec4f (face.v[k].norm, 1);
            rawNorm = normCorr * rawNorm;
            norm[k] = Normalize( vec3f (rawNorm.x, rawNorm.y, rawNorm.z) );
        }

        Rasterize(screen[0], screen[1], screen[2],
                  tex[0], tex[1], tex[2],
                  norm[0], norm[1], norm[2],
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
