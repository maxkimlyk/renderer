#ifndef _RENDFR_H_
#define _RENDER_H_

#include "geometry.h"
#include "tgaimage.h"
#include "model.h"

mat4f LookAt(vec3f eye, vec3f center, vec3f up);
mat4f Viewport(int x, int y, int width, int height);

void Rasterize(vec3f vertex1, vec3f vertex2, vec3f vertex3,
               vec2f tex1, vec2f tex2, vec2f tex3,
               vec3f norm1, vec3f norm2, vec3f norm3,
               float *zBuffer, Canvas *texture, vec3f lightDir, Canvas *canvas);

#endif
