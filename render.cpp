#include "render.h"
#include <cmath>

#include <iostream>

void Line(vec2i p1, vec2i p2, Color color, Canvas *canvas)
{
    bool vertical = false;
    if (abs(p1.y - p2.y) > abs(p1.x - p2.x))
    {
        vertical = true;
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
    }

    if (p1.x > p2.x)
    {
        std::swap(p1, p2);
    }

    int deltax = p2.x - p1.x;
    int deltay = p2.y - p1.y;

    int y = p1.y;
    int yinc = p2.y > p1.y ? 1 : -1;
    int dyXk = 2*deltay;                     // dyXk = dy*k, where k = 2 * deltax
    int errorXk = 0;                         // errorXk = error*k, where k = 2 * deltax
    
    for (int x = p1.x; x <= p2.x; x++)
    {
        if (vertical)
            canvas->set(y, x, color);
        else
            canvas->set(x, y, color);

        errorXk += dyXk;                    // equivalent: error + dy
        if(abs(errorXk) >= deltax)          // equivalent: abs(error) >= 0.5
        {
            y += yinc;
            errorXk -= yinc * deltax * 2;   // equivalent: error -= yinc
        }
    }
}

void Triangle(vec2i p1, vec2i p2, vec2i p3, Color color, Canvas *canvas)
{
    // sort vertices by y
    if (p1.y > p2.y)
        std::swap(p1, p2);
    if (p2.y > p3.y)
        std::swap(p2, p3);
    if (p1.y > p2.y)
        std::swap(p1, p2);

    // degenerate triangle
    if (p1.y == p3.y)
        return;

    int totalHeight = p3.y - p1.y;
    for (int y = p1.y; y <= p3.y; y++)
    {
        bool upperSegment = y > p2.y || p1.y == p2.y;
        int segmentHeight = upperSegment ? p3.y - p2.y : p2.y - p1.y;
    
        float t = (y - p1.y) / (float)totalHeight;
        float p = upperSegment ? (y - p2.y) / (float) segmentHeight : (y - p1.y) / (float) segmentHeight;
        
        int x1 = p1.x + t * (p3.x - p1.x);
        int x2 = upperSegment ? p2.x + p * (p3.x - p2.x) : p1.x + p * (p2.x - p1.x);
        
        if (x1 > x2)
            std::swap(x1, x2);

        for (int x = x1; x <= x2; x++)
        {
            canvas->set(x, y, color);
        }
    }
}

void Rasterize(vec3f p1, vec3f p2, vec3f p3, Color color, Canvas *canvas, float zBuffer[], uint32_t i)
{
    // save from float inaccuracy
    p1.y = (int)p1.y;
    p2.y = (int)p2.y;
    p3.y = (int)p3.y;

    // sort vertices by y
    if (p1.y > p2.y)
        std::swap(p1, p2);
    if (p2.y > p3.y)
        std::swap(p2, p3);
    if (p1.y > p2.y)
        std::swap(p1, p2);

    // degenerate triangle
    if (p1.y == p3.y)
        return;

    int totalHeight = p3.y - p1.y;
    for (int y = p1.y; y <= p3.y; y++)
    {
        bool upperSegment = y > p2.y || p2.y - p2.y;
        int segmentHeight = upperSegment ? p3.y - p2.y : p2.y - p1.y;
    
        float t = (y - p1.y) / (float)totalHeight;
        float u = upperSegment ? (y - p2.y) / (float) segmentHeight : (y - p1.y) / (float) segmentHeight;

        vec3f v1 = p1 + t * (p3 - p1);
        vec3f v2 = upperSegment ? p2 + u * (p3 - p2) : p1 + u * (p2 - p1);
        
        if (v1.x > v2.x)
            std::swap(v1, v2);

        for (int x = v1.x; x <= v2.x; x++)
        {
            float s = (v2.x == v1.x) ? 1.0f : (x - v1.x) / (v2.x - v1.x);
            vec3f p = v1 + s * (v2 - v1);
            
            int index = x + y * canvas->get_width();
            if (p.z > zBuffer[index])
            {
                zBuffer[index] = p.z;
                canvas->set(x, y, color);
            }
        }
    }
}

void Rasterize(Vertex vertex1, Vertex vertex2, Vertex vertex3, float zBuffer[], Canvas *texture, vec3f lightDir, Canvas *canvas, uint32_t i)
{
    // sort vertices by y
    if (vertex1.geom.y > vertex2.geom.y)
        std::swap(vertex1, vertex2);
    if (vertex2.geom.y > vertex3.geom.y)
        std::swap(vertex2, vertex3);
    if (vertex1.geom.y > vertex2.geom.y)
        std::swap(vertex1, vertex2);

    vec3f &p1 = vertex1.geom;
    vec3f &p2 = vertex2.geom;
    vec3f &p3 = vertex3.geom;

    // save from float inaccuracy
    p1.y = (int)p1.y;
    p2.y = (int)p2.y;
    p3.y = (int)p3.y;

    // degenerate triangle
    if (p1.y == p3.y)
        return;

    int totalHeight = p3.y - p1.y;
    for (int y = p1.y; y <= p3.y; y++)
    {
        bool upperSegment = y > p2.y || p2.y - p2.y;
        int segmentHeight = upperSegment ? p3.y - p2.y : p2.y - p1.y;
    
        float t = (y - p1.y) / (float)totalHeight;
        float u = upperSegment ? (y - p2.y) / (float) segmentHeight : (y - p1.y) / (float) segmentHeight;

        vec3f v1 = p1 + t * (p3 - p1);
        vec3f v2 = upperSegment ? p2 + u * (p3 - p2) : p1 + u * (p2 - p1);
        vec2f tex1 = vertex1.tex + t * (vertex3.tex - vertex1.tex);
        vec2f tex2 = upperSegment ? vertex2.tex + u * (vertex3.tex - vertex2.tex) : vertex1.tex + u * (vertex2.tex - vertex1.tex);
        vec3f norm1 = vertex1.norm + t * (vertex3.norm - vertex1.norm);
        vec3f norm2 = upperSegment ? vertex2.norm + u * (vertex3.norm - vertex2.norm) : vertex1.norm + u * (vertex2.norm - vertex1.norm);

        //printf ("norm1 norm2: %f %f %f %f %f %f\n", norm1.x, norm1.y, norm1.z, norm2.x, norm2.y, norm2.z);
        
        if (v1.x > v2.x)
        {
            std::swap(v1, v2);
            std::swap(tex1, tex2);
            std::swap(norm1, norm2);
        }

        for (int x = v1.x; x <= v2.x; x++)
        {
            float s = (v2.x == v1.x) ? 1.0f : (x - v1.x) / (v2.x - v1.x);
            vec3f p = v1 + s * (v2 - v1);
            
            int index = x + y * canvas->get_width();
            if (0 <= x && x < canvas->get_width() && 0 <= y && y < canvas->get_height())
            {
                if (p.z > zBuffer[index])
                {
                    zBuffer[index] = p.z;

                    vec2f tex = tex1 + s * (tex2 - tex1);
                    vec3f norm = norm1 + s * (norm2 - norm1);
                    //printf("norm: %f %f %f\n", norm.x, norm.y, norm.z);
                    Color color = texture->get(tex.x * texture->get_width(), tex.y * texture->get_height());
                    float lightFactor = norm * lightDir;
                    canvas->set(x, y, color * lightFactor);
                }
            }
        }
    }
}
