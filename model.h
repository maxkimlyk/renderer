#ifndef _MODEL_H_
#define _MODEL_H_

#include "geometry.h"

#include <vector>
#include <cstdio>
#include <cstring>

#pragma pack(push, 1)
struct Vertex
{
    vec3f geom;
    vec2f tex;
    vec3f norm;
};
#pragma pack(pop)

struct Face
{
    Vertex v[3];
};

struct IndexFace
{
    uint32_t v1, t1, n1;
    uint32_t v2, t2, n2;
    uint32_t v3, t3, n3;
};

class Model
{
private:
    std::vector<vec3f> geom;
    std::vector<vec2f> tex;
    std::vector<vec3f> norm;
    std::vector<IndexFace> iface;

    void ParseLine(const char *line)
    {
        switch (line[0])
        {
        case 'v':
            if (line[1] != 't' && line[1] != 'n')
            {
                float x, y, z;
                sscanf(line, "v %f %f %f", &x, &y, &z);
                vec3f vec(x, y, z);
                geom.push_back(vec);
                break;
            }
            else if (line[1] == 't')
            {
                float u, v;
                sscanf(line, "vt %f %f", &u, &v);
                vec2f vec(u, v);
                tex.push_back(vec);
                break;
            }
            else if (line[1] == 'n')
            {
                float x, y, z;
                sscanf(line, "vn %f %f %f", &x, &y, &z);
                vec3f vec(x, y, z);
                norm.push_back(vec);
                break;
            }

        case 'f':
            {
                uint32_t v1, t1, n1, v2, t2, n2, v3, t3, n3;
                sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
                IndexFace face = {v1-1, t1-1, n1-1, v2-1, t2-1, n2-1, v3-1, t3-1, n3-1};
                iface.push_back(face);
                break;
            }
        }
    }
    
public:
    bool Read(const char *filename)
    {
        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
            printf("Could not open file: %s\n", filename);
            return false;
        }

        char line[1024];
        while (fgets(line, sizeof(line), file))
        {
            ParseLine(line);
        }

        if (!feof(file))
        {
            printf("Error while reading file %s\n", filename);
            return false;
        }
        
        fclose(file);

        return true;
    }

    Face GetFace(uint32_t i)
    {
        Face face;
        face.v[0].geom = geom[ iface[i].v1 ];
        face.v[1].geom = geom[ iface[i].v2 ];
        face.v[2].geom = geom[ iface[i].v3 ];
        face.v[0].tex = tex[ iface[i].t1 ];
        face.v[1].tex = tex[ iface[i].t2 ];
        face.v[2].tex = tex[ iface[i].t3 ];
        face.v[0].norm = norm[ iface[i].n1 ];
        face.v[1].norm = norm[ iface[i].n2 ];
        face.v[2].norm = norm[ iface[i].n3 ];
        return face;
    }

    size_t NumFaces()
    {
        return iface.size();
    }
};

#endif 
