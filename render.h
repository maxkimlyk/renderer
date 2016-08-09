#ifndef _RENDFR_H_
#define _RENDER_H_

#include "geometry.h"
#include "tgaimage.h"
#include "model.h"

void Line(vec2i p1, vec2i p2, Color color, Canvas *canvas);
void Triangle(vec2i p1, vec2i p2, vec2i p3, Color color, Canvas *canvas);
void Rasterize(vec3f p1, vec3f p2, vec3f p3, Color color, Canvas *canvas, float zBuffer[], uint32_t i);
void Rasterize(Vertex vertex1, Vertex vertex2, Vertex vertex3, float zBuffer[], Canvas *texture, vec3f lightDir, Canvas *canvas, uint32_t i);

#endif