#pragma once
#include <vector>
namespace MeshIOLib
{
    class SymetricMatrix {
        public:

        public:
            double m[10];

    //public:

    //    // Constructor

    //    SymetricMatrix(double c = 0) { loopi(0, 10) m[i] = c; }

    //    SymetricMatrix(double m11, double m12, double m13, double m14,
    //        double m22, double m23, double m24,
    //        double m33, double m34,
    //        double m44) {
    //        m[0] = m11;  m[1] = m12;  m[2] = m13;  m[3] = m14;
    //        m[4] = m22;  m[5] = m23;  m[6] = m24;
    //        m[7] = m33;  m[8] = m34;
    //        m[9] = m44;
    //    }

    //    // Make plane

    //    SymetricMatrix(double a, double b, double c, double d)
    //    {
    //        m[0] = a * a;  m[1] = a * b;  m[2] = a * c;  m[3] = a * d;
    //        m[4] = b * b;  m[5] = b * c;  m[6] = b * d;
    //        m[7] = c * c; m[8] = c * d;
    //        m[9] = d * d;
    //    }

    //    double operator[](int c) const { return m[c]; }

    //    // Determinant

    //    double det(int a11, int a12, int a13,
    //        int a21, int a22, int a23,
    //        int a31, int a32, int a33)
    //    {
    //        double det = m[a11] * m[a22] * m[a33] + m[a13] * m[a21] * m[a32] + m[a12] * m[a23] * m[a31]
    //            - m[a13] * m[a22] * m[a31] - m[a11] * m[a23] * m[a32] - m[a12] * m[a21] * m[a33];
    //        return det;
    //    }

    //    const SymetricMatrix operator+(const SymetricMatrix& n) const
    //    {
    //        return SymetricMatrix(m[0] + n[0], m[1] + n[1], m[2] + n[2], m[3] + n[3],
    //            m[4] + n[4], m[5] + n[5], m[6] + n[6],
    //            m[7] + n[7], m[8] + n[8],
    //            m[9] + n[9]);
    //    }

    //    SymetricMatrix& operator+=(const SymetricMatrix& n)
    //    {
    //        m[0] += n[0];   m[1] += n[1];   m[2] += n[2];   m[3] += n[3];
    //        m[4] += n[4];   m[5] += n[5];   m[6] += n[6];   m[7] += n[7];
    //        m[8] += n[8];   m[9] += n[9];
    //        return *this;
    //    }
    };

    typedef long index_t;

    typedef struct triangle_tag {

        index_t _vertexID[3];
        Vec3 _normal;
        // attribute for mesh simplification
        // // error matrix 
        // // is deleted?
        // // is dirty?
        double _error[4]; 
        int _deleted;
        int _dirty;

        index_t& i() { return _vertexID[0]; }
        const index_t& i() const { return _vertexID[0]; }

        index_t& j() { return _vertexID[1]; }
        const index_t& j() const { return _vertexID[1]; }

        index_t& k() { return _vertexID[2]; }
        const index_t& k() const { return _vertexID[2]; }

        triangle_tag()
        {
            _vertexID[0] = _vertexID[1] = _vertexID[2] = -1;
            for(double& i : _error)
                i = 0.0;
            _deleted = _dirty = 0;
        }

    } Triangle;

    typedef struct vertex_tag {

        vertex_tag() {}
        vertex_tag(float x, float y, float z) {
            _position.x = x;
            _position.y = y;
            _position.z = z;
        }

        Vec3 _position;
        size_t _triangleID;
        size_t _vid; // _triangleID의 face 내에서 vertex 위치 (0,1,2)
        std::vector<size_t> _ptrTriIDs; // _triangleID의 face 내에서 vertex 위치 (0,1,2) 집합들
        std::vector<size_t> _ptrVids; // _triangleID의 face 내에서 vertex 위치 (0,1,2) 집합들

        bool operator==(const vertex_tag& rhs) const
        {
            return (_position.x == rhs._position.x && _position.y == rhs._position.y && _position.z == rhs._position.z);
        }

        bool operator!=(const vertex_tag& rhs) const
        {
            return (_position.x != rhs._position.x || _position.y != rhs._position.y || _position.z != rhs._position.z);
        }

        bool operator<(const vertex_tag& rhs) const
        {
            if (_position.x != rhs._position.x)
                return _position.x < rhs._position.x;
            else if (_position.y != rhs._position.y)
                return _position.y < rhs._position.y;
            else if (_position.z != rhs._position.z)
                return _position.z < rhs._position.z;
            else
                return false;
        }

    } Vertex;

    typedef struct edge_tag {

        index_t _vertexID[2];

        index_t& i() { return _vertexID[0]; }
        const index_t& i() const { return _vertexID[0]; }

        index_t& j() { return _vertexID[1]; }
        const index_t& j() const { return _vertexID[1]; }

        edge_tag()
        {
            _vertexID[0] = _vertexID[1] = -1;
        }

        edge_tag(const index_t v1, const index_t v2)
        {
            _vertexID[0] = v1;
            _vertexID[1] = v2;
        }
    } Edge;
};
