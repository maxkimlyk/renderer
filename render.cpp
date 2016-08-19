#include "render.h"
#include <cmath>

mat4f LookAt(vec3f eye, vec3f center, vec3f up)
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

mat4f Viewport(int x, int y, int width, int height)
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

struct Rect
{
    int left, right, bottom, top;
};

void MinMax(int a, int b, int c, int *min, int *max)
{
    if (a > b)
        std::swap(a, b);
    if (b > c)
        std::swap(b, c);
    if (a > b)
        std::swap(a, b);
    *min = a;
    *max = c;
}

Rect BoundingBox(vec3f p1, vec3f p2, vec3f p3)
{
    Rect rect;
    MinMax(p1.x, p2.x, p3.x, &rect.left, &rect.right);
    MinMax(p1.y, p2.y, p3.y, &rect.bottom, &rect.top);
    return rect;
}

void Rasterize(vec3f vertex1, vec3f vertex2, vec3f vertex3,
                  vec2f tex1, vec2f tex2, vec2f tex3,
                  vec3f norm1, vec3f norm2, vec3f norm3,
                  float *zBuffer, Canvas *texture, vec3f lightDir, Canvas *canvas)
{
    Rect bbox = BoundingBox(vertex1, vertex2, vertex3);

    vec2f v12 = vec2f (vertex2.x - vertex1.x, vertex2.y - vertex1.y);
    vec2f v13 = vec2f (vertex3.x - vertex1.x, vertex3.y - vertex1.y);

    vec3f a = vec3f (v12.x, v13.x, 0);
    vec3f b = vec3f (v12.y, v13.y, 0);
    float denum = v12.x * v13.y - v12.y * v13.x; // the same as Cross(a,b).z
    if (denum == 0) // degenerate triangle
        return;
    float coef = 1.0f / denum;

    vec3f light = vec3f (norm1 * lightDir, norm2 * lightDir, norm3 * lightDir);

    for (int y = bbox.bottom; y <= bbox.top; y++)
    {
        for (int x = bbox.left; x <= bbox.right; x++)
        {
            a.z = vertex1.x - x;
            b.z = vertex1.y - y;
            vec3f tmp = Cross(a, b) * coef;
            vec3f bar = vec3f (1.0f - tmp.x - tmp.y, tmp.x, tmp.y);
            if (bar.x < 0 || bar.y < 0 || bar.z < 0)
                continue;

            float z = bar * vec3f (vertex1.z, vertex2.z, vertex3.z);
            int zIndex = x + y * canvas->get_width();
            if (x < 0 || x >= canvas->get_width() || y < 0 || y >= canvas->get_height() || z < zBuffer[zIndex])
                continue;
                
            zBuffer[zIndex] = z;

            float texX = bar * vec3f (tex1.x, tex2.x, tex3.x);
            float texY = bar * vec3f (tex1.y, tex2.y, tex3.y);
            Color color = texture->get(texX * texture->get_width(), texY * texture->get_height());
            float lightFactor = bar * light;
            canvas->set(x, y, color * lightFactor);
        }
    }
}

void Rasterize(vec3f *screenCoord, Shader *shader, float *zBuffer, Canvas *canvas)
{
    vec3f vertex1 = screenCoord[0];
    vec3f vertex2 = screenCoord[1];
    vec3f vertex3 = screenCoord[2];

    Rect bbox = BoundingBox(vertex1, vertex2, vertex3);

    vec2f v12 = vec2f (vertex2.x - vertex1.x, vertex2.y - vertex1.y);
    vec2f v13 = vec2f (vertex3.x - vertex1.x, vertex3.y - vertex1.y);

    vec3f a = vec3f (v12.x, v13.x, 0);
    vec3f b = vec3f (v12.y, v13.y, 0);
    float denum = v12.x * v13.y - v12.y * v13.x; // the same as Cross(a,b).z
    if (denum == 0) // degenerate triangle
        return;
    float coef = 1.0f / denum;

    for (int y = bbox.bottom; y <= bbox.top; y++)
    {
        for (int x = bbox.left; x <= bbox.right; x++)
        {
            a.z = vertex1.x - x;
            b.z = vertex1.y - y;
            vec3f tmp = Cross(a, b) * coef;
            vec3f bar = vec3f (1.0f - tmp.x - tmp.y, tmp.x, tmp.y);
            if (bar.x < 0 || bar.y < 0 || bar.z < 0)
                continue;

            float z = bar * vec3f (vertex1.z, vertex2.z, vertex3.z);
            int zIndex = x + y * canvas->get_width();
            if (x < 0 || x >= canvas->get_width() || y < 0 || y >= canvas->get_height() || z < zBuffer[zIndex])
                continue;

            Color color;
            if (shader->Fragment(bar, &color))   // Fragment() returns true if want to discard pixel
                continue;
                
            zBuffer[zIndex] = z;
            canvas->set(x, y, color);
        }
    }
}
