#ifndef CW2_H
#define CW2_H

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

#define TURNING_SPEED 0.05
extern Mesh myMesh;
extern Mat4 matProj, matRotX, matRotZ, matRotY;
extern float zoom;
extern float xAngle, yAngle, zAngle;


#endif