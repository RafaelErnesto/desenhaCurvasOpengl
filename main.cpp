
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include<math.h>

static int slices = 16;
static int stacks = 16;

static int windowWidth = 640;
static int windowHeigth = 480;
static int points_selected = 0;

static int points[100][2];



/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}



float equacao2(int p1, int p2, float t) {
    float resultado = 0.0;
    resultado = ((1 - t) * p1) + (t * p2);
    return resultado;
}

float equacao3(int p1, int p2, int p3, float t) {
    float resultado = 0.0;
    resultado = (pow((1 - t),2) * p1) + (((2*t)*(1 - t)) * p2) + (pow(t,2)*p3);
    return resultado;
}

float equacao4(int p1, int p2, int p3, int p4, float t) {
    float resultado = 0.0;
    resultado = (pow((1 - t),3) * p1) + (((3*t)*pow((1 - t),2)) * p2) + ((3*pow(t,2))*((1-t)*p3)) + (pow(t,2) * p4);
    return resultado;
}

static void desenhaEq2(int x1, int x2, int y1, int y2) {
    glBegin(GL_LINE_STRIP);
        for(float t = 0.0f; t <= 1.0f; t = t + 0.001f){
            float coordx = equacao2(x1,x2, t);
            float coordy = equacao2(y1,y2, t);
            glVertex2d(coordx,coordy);
        }
    glEnd();
    glFlush( );
}

static void desenhaEq3(int x1, int x2, int x3, int y1, int y2, int y3) {
    glBegin(GL_LINE_STRIP);
        for(float t = 0.0f; t <= 1.0f; t = t + 0.001f){
            float coordx = equacao3(x1,x2,x3, t);
            float coordy = equacao3(y1,y2,y3, t);
            glVertex2d(coordx,coordy);
        }
    glEnd();
    glFlush( );
}

static void desenhaEq4(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4) {
    glBegin(GL_LINE_STRIP);
        for(float t = 0.0f; t <= 1.0f; t = t + 0.001f){
            float coordx = equacao4(x1,x2,x3,x4, t);
            float coordy = equacao4(y1,y2,y3,y4, t);
            glVertex2d(coordx,coordy);
        }
    glEnd();
    glFlush();
}

static void desenhaLinha() {
    int separador_equacoes = points_selected;
    glColor3f(0.0,0,1.0);
    int equations[10] = {0};
    int equations_set = 0;
    int rest = points_selected;
    int added = 0;
    int coord[4][2] = {0};
    for(int i = 0; i < points_selected; i++){
        coord[added][0] = points[i][0];
        coord[added][1] = points[i][1];
        added++;
        rest--;
        if(added == 4) {
            desenhaEq4(coord[0][0],coord[1][0],coord[2][0],coord[3][0],coord[0][1],coord[1][1],coord[2][1],coord[3][1]);
            added = 0;
            coord[added][0] = points[i][0];
            coord[added][1] = points[i][1];
            added++;
        }

        if(rest == 0 ) {
            switch(added) {
            case 3:
                desenhaEq3(coord[0][0],coord[1][0],coord[2][0],coord[0][1],coord[1][1],coord[2][1]);
                break;
            case 2:
                desenhaEq2(coord[0][0],coord[1][0],coord[0][1],coord[1][1]);
                break;
            }
        }
    }

}

static void desenhaPontos() {
    for(int i = 0; i < points_selected; i ++) {
        glColor3f(0.0,0,1.0);
        glBegin(GL_POINTS);
            glVertex2d(points[i][0],points[i][1]);
        glEnd();
        /*if(i > 0) {
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_LINES);
            glVertex2d(points[i-1][0],points[i-1][1]);
            glVertex2d(points[i][0],points[i][1]);
        glEnd();
        }*/
        glFlush();
    }
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeigth,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(points_selected > 0) {
         desenhaPontos();
    }
    desenhaLinha();
}



static void idle(void)
{
    glutPostRedisplay();
}


static void onMouse(int button, int state, int x, int y) {
    //state = 0 down, 1 up;
    if(state == 0 && points_selected < 100) {
        points[points_selected][0] = x;
        points[points_selected][1] = y;
        points_selected++;
        glutPostRedisplay();
    }

}
/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth,windowHeigth);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("Curvas");
    //glViewport(0, 0, windowWidth, windowHeigth);
    glPointSize(5.0);
    //glutReshapeFunc(resize);
    glutDisplayFunc(display);

    glClearColor(1,1,1,1);

    glutMouseFunc(onMouse);

    glutMainLoop();

    return EXIT_SUCCESS;
}
