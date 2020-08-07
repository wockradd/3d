#include "./hfiles/controls.h"
#include "./hfiles/main.h"
#include <GL/glut.h>


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
        case 'q':
            xAngle += TURNING_SPEED;
            break;
        case 'e':
            xAngle -= TURNING_SPEED;
            break;
        case 'd':
            yAngle -= TURNING_SPEED;
            break;
        case 'a':
            yAngle += TURNING_SPEED;
            break;
        case 'w':
            zAngle += TURNING_SPEED;
            break;
        case 's':
            zAngle -= TURNING_SPEED;
            break;
    }
    glutPostRedisplay();//force redraw
}
