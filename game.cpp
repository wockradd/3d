#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
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

Mesh myMesh;


struct Mesh generateMeshFromFile(){
    Mesh m = Mesh();//final mesh we're gonna return
    vector<Vector> tempVec;

    string line;
    ifstream myfile; //reading file stuff
    myfile.open("./resources/cube.obj");

    if (myfile.is_open()){
        while (getline(myfile,line)){
            stringstream ss;
            char junk;
            float x, y ,z;

            if(line[0] == 'v'){
                Vector v = Vector();      //create a new vector
                ss << line;               //turn the string into a stream
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
  }
  return m;
}



void drawMesh(){

}





void display(){
    glClear(GL_COLOR_BUFFER_BIT);//not sure
    glutSwapBuffers();//not sure
    
    drawMesh();

    glutPostRedisplay();//force it to refresh?
    glFlush();//not sure
}





void init(){
    glutInitDisplayMode(GLUT_SINGLE);//not sure
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("3ddddddd");
    glClearColor(0.4,0.4,0.4,0);//set bg colour to grey
    gluOrtho2D(0,WINDOW_WIDTH,WINDOW_HEIGHT,0);//sets x, y co-ords
}




int main(int argc, char** argv){
    myMesh = generateMeshFromFile();
    // for(int i=0 ; i< myMesh.tris.size() ; i++){
    //     printf("Triangle %i has points: \n(%f,%f,%f)\n(%f,%f,%f)\n(%f,%f,%f)\n\n",
    //     i,myMesh.tris[i].v1.x,myMesh.tris[i].v1.y,myMesh.tris[i].v1.z,myMesh.tris[i].v2.x,myMesh.tris[i].v2.y,myMesh.tris[i].v2.z,myMesh.tris[i].v3.x,myMesh.tris[i].v3.y,myMesh.tris[i].v3.z);
    // }


    glutInit(&argc, argv);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}