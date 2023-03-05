#pragma once
namespace MeshIOLib
{
    typedef struct triangle_tag {

        int _vertexID[3];
        Vec3 _normal;
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
        size_t* _ptrTriIDs = NULL; // _triangleID의 face 내에서 vertex 위치 (0,1,2) 집합들
        size_t _vid; // _triangleID의 face 내에서 vertex 위치 (0,1,2)
        size_t* _ptrVids = NULL ; // _triangleID의 face 내에서 vertex 위치 (0,1,2) 집합들

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
};
