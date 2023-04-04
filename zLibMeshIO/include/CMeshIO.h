#pragma once
#include "Vector.h"
#include "CMeshIOData.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace MeshIOLib
{
    class CMeshIO {

    public:
        CMeshIO();
        ~CMeshIO();

        void LoadOBJ(const char* filename);
        void LoadSTL(const char* filename);

        void WriteOBJ(const char* filename);
        void WriteOBJ(const char* filename, const std::vector<Vertex>& vecVerts, const std::vector<Triangle>& vecTris);
        
        void WriteSTL(const char* filename);
        void WriteSTL(const char* filename, const std::vector<Vertex>& vecVerts, const std::vector<Triangle>& vecTris);

    public:
        inline std::vector<Triangle> GetTriangleList() {
            return m_vecTriangles;
        }
        inline std::vector<Vertex> GetVertexList() {
            return m_vecVertices;
        }
        inline void SetTriangleList(std::vector<Triangle> tris) {
            m_vecTriangles = tris;
        }
        inline void SetVertexList(std::vector<Vertex> verts) {
            m_vecVertices = verts;
        }
    private:
        // thanks to https://github.com/mkeeter/fstl/blob/master/src/loader.cpp
        std::vector<Vertex> LoadSTL_Vertices(const char* filename);
        std::vector<Vertex> Load_ASCII(const char* filename);
        std::vector<Vertex> Load_Binary(const char* filename);
   
    private:
        inline void write_float(float f, FILE* file) {
            fwrite(&f, sizeof(f), 1, file);
        }
        inline void write_vertex_stl(Vec3 v, FILE* file) {
            write_float(v.x, file);
            write_float(v.y, file);
            write_float(v.z, file);
        }
        inline std::string trim(std::string& str)
        {
            str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
            str.erase(str.find_last_not_of(' ') + 1);         //surfixing spaces
            return str;
        }
        inline Vertex get_vector(std::string& str)
        {
            float x, y, z;
            if (::sscanf_s(str.c_str(), "vertex %f %f %f", &x, &y, &z) != 3)
            {
                printf("ascii stl format is not valid.\n");
                exit(1);
            }
            return Vertex(x, y, z);
        }

    private:
        std::vector<Triangle> m_vecTriangles;
        std::vector<Vertex> m_vecVertices;
    };
};

