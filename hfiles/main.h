#ifndef MAIN_H
#define MAIN_H
using namespace std;

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
    Vector normal;
};

struct Mesh{
    vector<Tri> tris;
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