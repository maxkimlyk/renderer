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
