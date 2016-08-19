#include <iostream>
#include <cfloat>
#include "tgaimage.h"
#include "render.h"
#include "model.h"

struct GouraudShader : Shader
{
    Model *model;
    mat4f viewMat;
    mat4f normCorrMat;
    vec3f lightDir;

    float lightIntensity[3];

    vec4f Vertex(
        int numFace,     // index number of face
        int numVertex)   // index number of vertex in triangle
    {
        vec4f norm4 = vec4f (model->GetFace(numFace).v[numVertex].norm, 0);
        norm4 = normCorrMat * norm4;
        vec3f norm = vec3f (norm4.x, norm4.y, norm4.z);
        lightIntensity[numVertex] = norm * lightDir;
        
        vec4f vertex = vec4f (model->GetFace(numFace).v[numVertex].geom, 1);
        return viewMat * vertex;
    }

    bool Fragment(vec3f bar, Color *color)
    {
        float intensity = bar * vec3f(lightIntensity[0], lightIntensity[1], lightIntensity[2]);
        *color = Color(255, 255, 255) * intensity;
        return false;    // don't discard the pixel
    }
};

struct TextureShader : Shader
{
    Model  *model;
    Canvas *textureDiff;
    Canvas *textureNorm;
    mat4f  viewMat;
    mat4f  normCorrMat;
    vec3f  lightDir;

    vec2f texCoord[3];

    vec4f Vertex(int numFace, int numVertex)
    {
        vec2f tex = model->GetFace(numFace).v[numVertex].tex;
        texCoord[numVertex] = tex;
        texCoord[numVertex] = tex;
        
        vec4f vertex = vec4f (model->GetFace(numFace).v[numVertex].geom, 1);
        return viewMat * vertex;
    }

    bool Fragment(vec3f bar, Color *color)
    {
        float texX      = bar * vec3f(texCoord[0].x,     texCoord[1].x,     texCoord[2].x);
        float texY      = bar * vec3f(texCoord[0].y,     texCoord[1].y,     texCoord[2].y);

        Color normColor = textureNorm->get(texX * textureNorm->get_width(), texY * textureNorm->get_height());
        vec3f norm = Normalize(vec3f(normColor[2], normColor[1], normColor[0]));
        vec4f norm4 = vec4f(norm, 0);
        norm4 = normCorrMat * norm4;
        norm = Normalize(vec3f (norm4.x, norm4.y, norm4.z));

        float intensity = norm * lightDir;
        
        Color textureColor = textureDiff->get(texX * textureDiff->get_width(), texY * textureDiff->get_height());
        *color = textureColor * intensity;
        return false;
    }
};

void DrawModel()
{
    const char * MODEL_NAME = "model.obj";
    const char * TEXTURE_DIFF_NAME = "african_head_diffuse.tga";
    const char * TEXTURE_NORM_NAME = "african_head_nm.tga";
    const size_t WIDTH = 1024;
    const size_t HEIGHT = 1024;
    const float  ZBUF_VALUE = -10000;

    const vec3f EYE = vec3f(-5, 1, 10);
    const vec3f CENTER = vec3f(0, 0, 0);
    const vec3f UP = vec3f(0, 1, 0);

    Model model;
    model.Read(MODEL_NAME);

    TGAImage textureDiff;
    textureDiff.read_tga_file(TEXTURE_DIFF_NAME);
    textureDiff.flip_vertically();

    TGAImage textureNorm;
    textureNorm.read_tga_file(TEXTURE_NORM_NAME);
    textureNorm.flip_vertically();
    
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    
    float zBuffer[WIDTH * HEIGHT];
    for (size_t i = 0; i < WIDTH * HEIGHT; i++)
        zBuffer[i] = ZBUF_VALUE;

    mat4f modelView   = LookAt(EYE, CENTER, UP);
    mat4f normCorrMat = Invert(Transpose(modelView));
    mat4f viewMatrix  = Viewport(0, 0, WIDTH, HEIGHT) * modelView;

    TextureShader shader;
    shader.model = &model;
    shader.textureDiff = &textureDiff;
    shader.textureNorm = &textureNorm;
    shader.viewMat = viewMatrix;
    shader.normCorrMat = normCorrMat;
    shader.lightDir = Normalize( vec3f(-4, 0, 10) );

    for (uint32_t i = 0; i < model.NumFaces(); i++)
    {
        vec3f screenCoord[3];
        for (int k = 0; k < 3; k++)
        {
            vec4f coord = shader.Vertex(i, k);
            screenCoord[k] = vec3f (coord.x / coord.w, coord.y / coord.w, coord.z / coord.w);
        }
        Rasterize(screenCoord, &shader, zBuffer, &image);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
}

int main(int argc, char** argv)
{
    DrawModel();

    return 0;
}
