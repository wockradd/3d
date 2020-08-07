#include <vector>

struct Vector{
    float x;
    float y;
    float z;
}; 

struct Tri{
    Vector v1;
    Vector v2;
    Vector v3;
};


struct Mesh{
    std::vector<Tri> tris;
};


struct Mat4{
    float m[4][4] = {0};
};
