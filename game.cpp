#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 1024


struct Vector{
    GLfloat x;
    GLfloat y;
    GLfloat z;
}; 

struct Tri{
    Vector v1;
    Vector v2;
    Vector v3;
};


struct Mesh{
    vector<Tri> tris;
};


struct Mat4{
    float m[4][4] = {0};
};

Mesh myMesh;
Mat4 matProj, matRotX, matRotZ, matRotY;
int zoom = 10000000;
float angle = 0;

void initMatProj(){
    float zNear = 0.1;
    float zFar = 1000;
    float fov = 90;
    float aspectRatio = (float)WINDOW_HEIGHT/(float)WINDOW_WIDTH;
    float fovRad = 1 - tan(fov*0.5/180*3.1415926);

    matProj.m[0][0] = aspectRatio*fovRad;
    matProj.m[1][1] = fovRad;
    matProj.m[2][2] = zFar / (zFar - zNear);
    matProj.m[3][2] = (-zFar*zNear) / (zFar - zNear);
    matProj.m[2][3] = 1;
}

void rotMat(){
    //rotation z
    matRotZ.m[0][0] = cosf(angle);
	matRotZ.m[0][1] = sinf(angle);
	matRotZ.m[1][0] = -sinf(angle);
	matRotZ.m[1][1] = cosf(angle);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;
	// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(angle * 0.5);
	matRotX.m[1][2] = sinf(angle * 0.5);
	matRotX.m[2][1] = -sinf(angle * 0.5);
	matRotX.m[2][2] = cosf(angle * 0.5);
	matRotX.m[3][3] = 1;
    // Rotation Y
	matRotY.m[0][0] = cosf(angle*0.2);
	matRotY.m[2][0] = sinf(angle * 0.2);
	matRotY.m[1][1] = 1;
	matRotY.m[0][2] = -sinf(angle * 0.2);
	matRotY.m[2][2] = cosf(angle * 0.2);
    matRotY.m[3][3] = 1;
}


struct Vector matrixMultiply(Vector in, Mat4 m){


    Vector out = Vector();

    out.x = in.x*m.m[0][0] + in.y*m.m[1][0] + in.z*m.m[2][0] + m.m[3][0];
    out.y = in.x*m.m[0][1] + in.y*m.m[1][1] + in.z*m.m[2][1] + m.m[3][1];
    out.z = in.x*m.m[0][2] + in.y*m.m[1][2] + in.z*m.m[2][2] + m.m[3][2];
    float w = in.x*m.m[0][3] + in.y*m.m[1][3] + in.z*m.m[2][3] + m.m[3][3];

    if(w != 0){
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }


    return out;
}





//TODO
void drawMesh(){
    for(auto tri: myMesh.tris){
        Tri projectedTri, translatedTri, rotateXTri, rotateXZTri, rotateXZYTri;
        
        
        rotateXTri.v1 = matrixMultiply(tri.v1, matRotX);
        rotateXTri.v2 = matrixMultiply(tri.v2, matRotX);
        rotateXTri.v3 = matrixMultiply(tri.v3, matRotX);

        rotateXZTri.v1 = matrixMultiply(rotateXTri.v1, matRotZ);
        rotateXZTri.v2 = matrixMultiply(rotateXTri.v2, matRotZ);
        rotateXZTri.v3 = matrixMultiply(rotateXTri.v3, matRotZ);

        rotateXZYTri.v1 = matrixMultiply(rotateXZTri.v1, matRotY);
        rotateXZYTri.v2 = matrixMultiply(rotateXZTri.v2, matRotY);
        rotateXZYTri.v3 = matrixMultiply(rotateXZTri.v3, matRotY);

   
        translatedTri = rotateXZYTri;

        translatedTri.v1.z += 5;
        translatedTri.v2.z += 5;
        translatedTri.v3.z += 5;

        

        projectedTri.v1 = matrixMultiply(translatedTri.v1, matProj);
        projectedTri.v2 = matrixMultiply(translatedTri.v2, matProj);
        projectedTri.v3 = matrixMultiply(translatedTri.v3, matProj);

        glBegin(GL_LINES);
            glVertex2f(projectedTri.v1.x*zoom, projectedTri.v1.y*zoom);
            glVertex2f(projectedTri.v2.x*zoom, projectedTri.v2.y*zoom);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(projectedTri.v2.x*zoom, projectedTri.v2.y*zoom);
            glVertex2f(projectedTri.v3.x*zoom, projectedTri.v3.y*zoom);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(projectedTri.v3.x*zoom, projectedTri.v3.y*zoom);
            glVertex2f(projectedTri.v1.x*zoom, projectedTri.v1.y*zoom);
        glEnd();
        



      
    }
}


struct Mesh generateMeshFromFile(){
    Mesh m = Mesh();//final mesh we're gonna return
    vector<Vector> tempVec;

    //reading file stuff
    string line;
    ifstream myfile; 
    myfile.open("./resources/cube.obj");

    if (myfile.is_open()){
        while (getline(myfile,line)){//read line by line
            stringstream ss;
            char junk;
            float x, y ,z;

            if(line[0] == 'v'){
                Vector v = Vector();      //create a new vector
                ss << line;               //send the string to the stream
                ss >> junk >> x >> y >> z;//use the stream to initialize the variables

                v.x = x;
                v.y = y;  //create the vector
                v.z = z;

                tempVec.push_back(v); //add to list
            }

            if(line[0] =='f'){
                Tri t = Tri();            //create a new tri
                ss << line;               //turn the string into a stream
                ss >> junk >> x >> y >> z;//use the stream to initialize the variables

                t.v1 = tempVec[x-1];
                t.v2 = tempVec[y-1]; //create new tri
                t.v3 = tempVec[z-1];

                m.tris.push_back(t);//add to list
            } 
        }
    myfile.close();
  }else{
      cout << "something went wrong\n";
  }
  return m;
}


void mouse(int button, int state, int x, int y){
    if(button == 4 && zoom >= 100000){
        zoom -= 100000;
    }else if(button == 3){
        zoom += 100000;
    }
    glutPostRedisplay();
}


void display(){
    glClear(GL_COLOR_BUFFER_BIT);//not sure
    
    drawMesh();
    rotMat();
    cout << angle << "hi\n";
    angle += 0.001;

    glutPostRedisplay();//force it to refresh?
    glFlush();//not sure
}





void init(){
    //cout << "initializing\n";
    myMesh = generateMeshFromFile();
        // for(int i=0 ; i< myMesh.tris.size() ; i++){
        //     printf("Triangle %i has points: \n(%f,%f,%f)\n(%f,%f,%f)\n(%f,%f,%f)\n\n",
        //     i,myMesh.tris[i].v1.x,myMesh.tris[i].v1.y,myMesh.tris[i].v1.z,myMesh.tris[i].v2.x,myMesh.tris[i].v2.y,myMesh.tris[i].v2.z,myMesh.tris[i].v3.x,myMesh.tris[i].v3.y,myMesh.tris[i].v3.z);
        // }
    initMatProj();
    glutInitDisplayMode(GLUT_SINGLE);//not sure
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("3ddddddd");
    glClearColor(0.4,0.4,0.4,0);//set bg colour to grey
    
}




int main(int argc, char** argv){
    glutInit(&argc, argv);
    init();
    glutSwapBuffers();//not sure
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}