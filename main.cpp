
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include<math.h>

static int windowWidth = 640;
static int windowHeigth = 480;
//variavel que armazena a quantidade de pontos selecionados na tela.
static int points_selected = 0;
//array que armazena as coordenadas dos pontos selecionados
static int points[100][2];

//variavel de controle, se false não desenha o tracejado entre os pontos, se true desenha.
static bool desenha_linha_pontilhada = true;

//função que remapeia as coordenadas quando as dimensões da janela são alteradas.
static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


// equação para dois pontos
float equacao2(int p1, int p2, float t) {
    float resultado = 0.0;
    resultado = ((1 - t) * p1) + (t * p2);
    return resultado;
}

//equação para 3 pontos
float equacao3(int p1, int p2, int p3, float t) {
    float resultado = 0.0;
    resultado = (pow((1 - t),2) * p1) + (((2*t)*(1 - t)) * p2) + (pow(t,2)*p3);
    return resultado;
}

//equação para 4 pontos.
float equacao4(int p1, int p2, int p3, int p4, float t) {
    float resultado = 0.0;
    resultado = (pow((1 - t),3) * p1) + (((3*t)*pow((1 - t),2)) * p2) + ((3*pow(t,2))*((1-t)*p3)) + (pow(t,3) * p4);
    return resultado;
}

// função que desenha a linha entre dois pontos,
//recebe as coordenadas de cada ponto, chama as equações para cada ponto com t variando de 0 a 1
//e desenha a linha com o retorndo das equações
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

// função que desenha a linha entre três pontos,
//recebe as coordenadas de cada ponto, chama as equações passando as coordenadas dos pontos com t variando de 0 a 1
//e desenha a linha com o retorndo das equações
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

// função que desenha a linha entre quatro pontos,
//recebe as coordenadas de cada ponto, chama as equações passando as coordenadas dos pontos com t variando de 0 a 1
//e desenha a linha com o retorndo das equações
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

//função que coordenada os desenhos das linhas.
static void desenhaCurvas() {
    //cor azul para as linhas.
    glColor3f(0.0,0,1.0);
    //resto armazena o valor de quantos pontos ainda precisam ser desenhados
    int resto = points_selected;
    //added é como se fosse uma flag que conta quantos pontos ja foram adicionados à variavel coord
    int added = 0;
    //coord armazena as coordenadas dos pontos que serão desenhados no proximo comando de desenho.
    int coord[4][2] = {0};
    for(int i = 0; i < points_selected; i++){
        //a cada iteração adiciona as coordenadas em coord.
        coord[added][0] = points[i][0];
        coord[added][1] = points[i][1];
        //incrementa added, added sempre terá a quantidade de pontos adicionados em coord
        added++;
        //a cada ponto adicionado em coord, decrementa do resto.
        resto--;
        //se já forma adicionados 4 pontos em coord...
        if(added == 4) {
            //chama a função que desenha a curva entre 4 pontos, passando as coordenadas dos pontos adicionados em coord.
            desenhaEq4(coord[0][0],coord[1][0],coord[2][0],coord[3][0],coord[0][1],coord[1][1],coord[2][1],coord[3][1]);
            //reseta add
            added = 0;
            //adiciona o ponto atual a coord, isso porque a proxima linha tem que incluir o ultimo ponto da ultima linha desenhada,
            //para que a proxima comece onde a anterior terminou
            coord[added][0] = points[i][0];
            coord[added][1] = points[i][1];
            //incrementa added, ja que acabamos de adicionar um ponto em coord.
            added++;
        }

        //se o resto for 0, significa que não há mais nenhum ponto a ser adicionado na proxima iteração
        if(resto == 0 ) {
            //verificamos quantos pontos estao na variavel coord, estes pontos ainda tem que ser desenhados, são os pontos restantes.
            switch(added) {
            case 3:
                //se added =3 chama a função que desenha a curva entre 3 pontos.
                desenhaEq3(coord[0][0],coord[1][0],coord[2][0],coord[0][1],coord[1][1],coord[2][1]);
                break;
            case 2:
                //se added = 2 chama a função que desenha a curva entre 2 pontos
                desenhaEq2(coord[0][0],coord[1][0],coord[0][1],coord[1][1]);
                break;
            }
        }
    }
}

//função que desenha os pontos selecionados na tela
static void desenhaPontos() {
    //points_selected é a quantidade de pontos que já foram selecionados pelo usuario.
    for(int i = 0; i < points_selected; i ++) {
        // pontos na cor preta
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_POINTS);
            //a cada iteração, desenha um ponto selecionado pelo usuario.
            glVertex2d(points[i][0],points[i][1]);
        glEnd();

        //se true, desenha tracejado entre cada ponto
        if(i != 0 && desenha_linha_pontilhada) {
            //cor cinza claro
            glColor3f(0.7,0.7,0.7);
            //configuração utilizada para desenhar linhas pontilhadas
            glPushAttrib(GL_ENABLE_BIT);
            glLineStipple(1, 0xAAAA);
            //ativa linha pontilhada
            glEnable(GL_LINE_STIPPLE);
            glBegin(GL_LINES);
            //a cada iteração, desenha uma linha tracejada entre os pontos.
                glVertex2d(points[i-1][0],points[i-1][1]);
                glVertex2d(points[i][0],points[i][1]);
            glEnd();
            //volta as configurações anteriores.
            glPopAttrib();
        }
        glFlush();
    }
}


static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    // se já existe alguma ponto selecionado, inicia o processo de desenho.
    if(points_selected > 0) {
         desenhaPontos();
         desenhaCurvas();
    }
}



static void idle(void)
{
    glutPostRedisplay();
}

//função que trata os eventos do mouse(click)
static void onMouse(int button, int state, int x, int y) {
    //state = 0 down, 1 up;
    //se o botão do mouse esta pressionado e a quantidade de pontos selecionados é inferior à 100,
    //armazena as coordenadas do ponto, incrementa a quantidade de pontos selecionados(points_selected) e redesenha a tela.
    //coloquei um limite de 100,pq o array points[100][2] esta limitado a 100, mas isso pode ser aumentado caso desejar.
    if(state == 0 && points_selected < 100) {
        points[points_selected][0] = x;
        points[points_selected][1] = y;
        points_selected++;
        glutPostRedisplay();
    }

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth,windowHeigth);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("Curvas");
    //altera o tamanho do ponto par poder ser visualizado na tela
    glPointSize(5.0);
    glutReshapeFunc(resize);
    glutDisplayFunc(display);

    //cor da tela branca
    glClearColor(1,1,1,1);

    glutMouseFunc(onMouse);

    glutMainLoop();

    return EXIT_SUCCESS;
}
