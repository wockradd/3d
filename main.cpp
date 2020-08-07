#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
using namespace std;

#include "./hfiles/structs.h"

#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 1024

#define TURNING_SPEED 0.05



/*
    TODO
    move stuff into different files, input, filestuff, rendering
    add gridlines
    double check controls 
    part 2 of tut
    part 3 of tut
    part 4 of tut
*/





//global variables
Mesh myMesh;
Mat4 matProj, matRotX, matRotZ, matRotY;
float zoom = 3;
float xAngle=0, yAngle=0, zAngle=0;




void initMatProj(){
    float zNear = -0.1;
    float zFar = 1000;
    float fov = 90;
    float aspectRatio = (float)WINDOW_HEIGHT/(float)WINDOW_WIDTH;
    float fovRad = 1 / tan(fov*0.5/180*3.1415926);

    matProj.m[0][0] = aspectRatio*fovRad;
    matProj.m[1][1] = fovRad;
    matProj.m[2][2] = zFar / (zFar - zNear);
    matProj.m[3][2] = (-zFar*zNear) / (zFar - zNear);
    matProj.m[2][3] = 1;
}

void updateRotationMats(){
    //rotation z
    matRotZ.m[0][0] = cosf(zAngle);
	matRotZ.m[0][1] = sinf(zAngle);
	matRotZ.m[1][0] = -sinf(zAngle);
	matRotZ.m[1][1] = cosf(zAngle);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;
	// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(xAngle);
	matRotX.m[1][2] = sinf(xAngle);
	matRotX.m[2][1] = -sinf(xAngle);
	matRotX.m[2][2] = cosf(xAngle);
	matRotX.m[3][3] = 1;
    // Rotation Y
	matRotY.m[0][0] = cosf(yAngle);
	matRotY.m[2][0] = sinf(yAngle);
	matRotY.m[1][1] = 1;
	matRotY.m[0][2] = -sinf(yAngle);
	matRotY.m[2][2] = cosf(yAngle);
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




void drawMesh(){
    for(auto tri: myMesh.tris){
        Tri projectedTri, translatedTri, rotateXTri, rotateXZTri, rotateXZYTri;
        
        //rotate
        rotateXTri.v1 = matrixMultiply(tri.v1, matRotX);
        rotateXTri.v2 = matrixMultiply(tri.v2, matRotX);
        rotateXTri.v3 = matrixMultiply(tri.v3, matRotX);

        rotateXZTri.v1 = matrixMultiply(rotateXTri.v1, matRotZ);
        rotateXZTri.v2 = matrixMultiply(rotateXTri.v2, matRotZ);
        rotateXZTri.v3 = matrixMultiply(rotateXTri.v3, matRotZ);

        rotateXZYTri.v1 = matrixMultiply(rotateXZTri.v1, matRotY);
        rotateXZYTri.v2 = matrixMultiply(rotateXZTri.v2, matRotY);
        rotateXZYTri.v3 = matrixMultiply(rotateXZTri.v3, matRotY);


   
        //translate, move it further from the "camera"
        translatedTri = rotateXZYTri;

        translatedTri.v1.z += zoom;
        translatedTri.v2.z += zoom;
        translatedTri.v3.z += zoom;

        
        //project, make it 2d
        projectedTri.v1 = matrixMultiply(translatedTri.v1, matProj);
        projectedTri.v2 = matrixMultiply(translatedTri.v2, matProj);
        projectedTri.v3 = matrixMultiply(translatedTri.v3, matProj);


        //draw
        glBegin(GL_LINES);
            glVertex2f(projectedTri.v1.x, projectedTri.v1.y);
            glVertex2f(projectedTri.v2.x, projectedTri.v2.y);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(projectedTri.v2.x, projectedTri.v2.y);
            glVertex2f(projectedTri.v3.x, projectedTri.v3.y);
        glEnd();
        glBegin(GL_LINES);
            glVertex2f(projectedTri.v3.x, projectedTri.v3.y);
            glVertex2f(projectedTri.v1.x, projectedTri.v1.y);
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
    //scroll to scale object
    if(button == 4 && zoom >= 0.01){
        zoom -= 0.05;
    }else if(button == 3){
        zoom += 0.05;
    }
    glutPostRedisplay();
}

void buttons(unsigned char key, int x, int y){
    switch(key){
        case 'w':
            xAngle += TURNING_SPEED;
            break;
        case 's':
            xAngle -= TURNING_SPEED;
            break;
        case 'd':
            yAngle -= TURNING_SPEED;
            break;
        case 'a':
            yAngle += TURNING_SPEED;
            break;
        case 'q':
            zAngle += TURNING_SPEED;
            break;
        case 'e':
            zAngle -= TURNING_SPEED;
            break;
    }
    glutPostRedisplay();//force redraw
}


void display(){
    glClear(GL_COLOR_BUFFER_BIT);//clear the screen
    
    drawMesh();
    updateRotationMats();
    xAngle += 0.0005;
    zAngle += 0.0003;

    glutPostRedisplay();//force it to redraw
    glFlush();//not sure
}





void init(){
    myMesh = generateMeshFromFile();
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
    glutKeyboardFunc(buttons);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}