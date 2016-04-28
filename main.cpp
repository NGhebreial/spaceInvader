/*****************************************************/
/*                                                   */
/*                  MAIN SPACE INVADER               */
/*                                                   */
/*****************************************************/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <cstdio>
#include <cstdlib>
#include <iostream>
/*  - Tener en cuenta el tamaño de los objetos en funcion de la pantalla */
using namespace std;
//Declaracion de metodos
void moverMalos(int v);
void dibujarMalos();
void iniciaPosicionMalo();
int compruebaMaloDisparado(int maloX, int maloY);
void iniciaPosicionNave();
void dibujaDisparosNave();
void iniciaPosicionCasas();
void dibujaCasas();
int compruebaCasaDisparada(int casaX, int vidasCasa);
void iniciaPosicionOvni();
void iniciaOvni(int v);
void moverOvni(int v);

void iniciaPosicionNave2();
void dibujaDisparosNave2();

void muestraResultadoFinal(int win);
void keyoperations();
void keySpecialOperations();
void dibujaMarcador();
void dibujaVidas();
void dibujaHUD2();
//Ancho y alto de la ventana
int ancho, alto;
//Posicion de la nave
int posXNave, posYNave,maxAnchoNave;
//Posicion de la nave 2
int posXNave2, posYNave2;
int posXBala2, posYBala2, puntuacion2, vidasNave2;
//Posicion del cuadrado
int posXMalo, posYMalo,maxAltoMalo,maxAnchoMalo;
//Array de disparos
int disparosNave[2];
int sizeArrayDisparoNave;
int i=0;
int j=0;
//Direccion de los malos
BOOLEAN derecha;
/*Matriz de malos
 Cada malo tendra 2 posiciones visuales */
int filasMalos=5;
int columnasMalos=22;
int malos[5][22];
int cuentaMalosMuertos;
//Array de teclas
bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[246];
//Marcador puntos
int puntuacion;
char str[15000];
//Marcador vidas de la nave
int vidasNave;
//Array de casas
int casas[8];
//Posicion de las casas
int posXCasas, posYCasas, numCasas;
//Posicion del ovni
int posXOvni, posYOvni,rango;
BOOLEAN mueveOvni;
BOOLEAN creaDisparoMalo;
BOOLEAN pause;
BOOLEAN finJuego;
int win;
//Para dos jugadores
int numJugadores;
/* Función para inicializar algunos parámetros de OpenGL */
void init(void)
{   /*Se incializa color de fondo y se asignan a las variables globales el ancho y alto de pantalla*/
    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_TEXTURE_2D);
    ancho = glutGet(GLUT_SCREEN_WIDTH);
    alto  = glutGet(GLUT_SCREEN_HEIGHT);
    //Posicion inicial malo
    iniciaPosicionMalo();
    iniciaPosicionNave();
    iniciaPosicionCasas();
    //Los malos comienzan a ir a la derecha
    derecha=true;
    glutTimerFunc(10, moverMalos, 0);
    //Ignorar la repeticion de teclas
    glutIgnoreKeyRepeat(1);
    puntuacion=0;
    vidasNave=3;
    iniciaPosicionOvni();
    mueveOvni=false;
    rango = rand() % 1000000 + 10000;
    glutTimerFunc(rango, iniciaOvni, 0);
    pause=false;
    finJuego=false;
    win=-1;
    cuentaMalosMuertos=0;
    numJugadores=1;
    puntuacion2=0;
    vidasNave2=3;
}

/* Función que se llamará cada vez que se dibuje en pantalla */
void display ( void )
{
    if(!pause && !finJuego){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // Aquí pondríamos la función gluLookAt si el observador se mueve

        // Aquí se definen los objetos y se colocan en la escena
        dibujaCasas();
        //Malo
        dibujarMalos();

        glPushMatrix(); //Con esto nos aseguramos que los cambios afectaran solo a este objeto
         //Posicion inicial de la nave
        glTranslatef(posXNave,posYNave,0);
        //Nave
        glBegin(GL_QUADS);
            //Color con el que se pintaran los objetos dibujados
            glColor3ub(255, 255, 255);
            glVertex2i(15, 15);
            glVertex2i(-30, 15);
            glVertex2i(-30, -5);
            glVertex2i(15, -5);

        glEnd();
        glPopMatrix();
        if(numJugadores==2){
            glPushMatrix(); //Con esto nos aseguramos que los cambios afectaran solo a este objeto
             //Posicion inicial de la nave
            glTranslatef(posXNave2,posYNave2,0);
            //Nave
            glBegin(GL_QUADS);
                //Color con el que se pintaran los objetos dibujados
                glColor3ub(0, 247, 0);
                glVertex2i(15, 15);
                glVertex2i(-30, 15);
                glVertex2i(-30, -5);
                glVertex2i(15, -5);

            glEnd();
            glPopMatrix();

        }
        //Disparos del cañon

        keyoperations();
        keySpecialOperations();
        dibujaDisparosNave();
        dibujaDisparosNave2();

        dibujaHUD2();

        dibujaVidas();
        //glPushMatrix();
        dibujaMarcador();
        //glPopMatrix();

        //glPushMatrix();

        //glPopMatrix();

        //glPushMatrix();

        //glPopMatrix();
        //Ovni
        if(posXOvni<ancho && mueveOvni){
            glPushMatrix(); //Con esto nos aseguramos que los cambios afectaran solo a este objeto
            glTranslatef(posXOvni,posYOvni,0);
            //Casas
            glBegin(GL_QUADS);
                //Color con el que se pintaran los objetos dibujados
                //Rojo
                glColor3ub(255, 0, 0);
                glVertex2i(15, 15);
                glVertex2i(-30, 15);
                glVertex2i(-30, -5);
                glVertex2i(15, -5);
            glEnd();
            glPopMatrix();
            glutTimerFunc(100, moverOvni, 0);
        }
        else{
            iniciaPosicionOvni();
            mueveOvni=false;

        }
        //Limpia buffers
        glutSwapBuffers();
    }
    if(finJuego){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        muestraResultadoFinal(win);
         //Limpia buffers
        glutSwapBuffers();
    }
}

/* Función que se ejecuta cuando no hay eventos */
void idle(void)
{
    if(!pause && !finJuego){
         //Posicion de los disparos
        for(i=0; i<sizeArrayDisparoNave; i=i+2){
            if(disparosNave[i]!=0){
                int posicionY=disparosNave[i+1]+15;
                //Mientras el disparo se visualize
                if(posicionY<alto){
                    disparosNave[i+1]=disparosNave[i+1]+1;
                }
                //Cuando el disparo llega al final limpiar array
                else{
                    disparosNave[i]=0;
                    disparosNave[i+1]=0;
                }
            }
        }
        if(numJugadores==2){
            if(posXBala2!=0){
                int posicionY=posYBala2+15;
                //Mientras el disparo se visualize
                if(posicionY<alto){
                    posYBala2+=1;
                }
                //Cuando el disparo llega al final limpiar array
                else{
                    posXBala2=0;
                    posYBala2=0;
                }
            }
        }
        glutPostRedisplay();
    }
}
void muestraResultadoFinal(int win){

    glRasterPos2i(ancho/2,alto/2);
    glColor3ub(255, 0, 0);
    if(win==1)
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)"YOU WIN");
    else if(win==0){
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)"YOU LOSE");
    }

}

void iniciaPosicionOvni(){
    posXOvni=0;
    posYOvni=alto-50;
}
void iniciaOvni(int v){
    mueveOvni=true;
    rango = rand() % 1000000 + 10000;
    glutTimerFunc(rango, iniciaOvni, 0);
   // glutTimerFunc(10, moverOvni, 0);
}
void moverOvni(int v){
    if(!pause)
        posXOvni=posXOvni+1;
    //glutTimerFunc(100, moverOvni, 0);
}
void iniciaPosicionCasas(){
    numCasas=8;
    posXCasas=alto/3;
    posYCasas=alto/4;
    //Posicion de las casas
    for(i=0; i<numCasas; i=i+2){
        //pos x
        casas[i]=posXCasas;
        //Num de vidas
        casas[i+1]=4;
        posXCasas=posXCasas+250;
    }
}
int compruebaCasaDisparada(int casaX, int vidasCasa){
    if(disparosNave[0]>0 && vidasCasa>0){
        int disparoX=disparosNave[0];
        int disparoY=disparosNave[1];
        if((casaX<=disparoX && disparoX<=casaX+80) &&
           (posYCasas==disparoY+15)) {
            //Disparado
            vidasCasa=vidasCasa-1;
            disparosNave[0]=0;
            disparosNave[1]=0;
        }
    }
    if(posXBala2>0 && vidasCasa>0){
        if((casaX<=posXBala2 && posXBala2<=casaX+80) &&
           (posYCasas==posYBala2+15)) {
            //Disparado
            vidasCasa=vidasCasa-1;
            posXBala2=0;
            posYBala2=0;
        }
    }
    return vidasCasa;
}
void dibujaCasas(){
    //Casas
    for(i=0; i<numCasas; i=i+2){
        casas[i+1]=compruebaCasaDisparada(casas[i],casas[i+1]);
        if(casas[i+1]>0){
            int red, green, blue;
            if(casas[i+1]==4){
                //Verde
                red=0;
                green=247;
                blue=0;
            }
            if(casas[i+1]==3){
                //Amarillo
                red=252;
                green=252;
                blue=0;
            }
            if(casas[i+1]==2){
                //Naranja
                red=255;
                green=117;
                blue=20;
            }
            if(casas[i+1]==1){
                //red
                red=248;
                green=0;
                blue=0;
            }
            glPushMatrix(); //Con esto nos aseguramos que los cambios afectaran solo a este objeto
            glTranslatef(casas[i],posYCasas,0);
            //Casas
            glBegin(GL_QUADS);
                //Color con el que se pintaran los objetos dibujados
                glColor3ub(red, green, blue);
                glVertex2i(0, 0);
                glVertex2i(80, 0);
                glVertex2i(80, 80);
                glVertex2i(0, 80);
            glEnd();
            glPopMatrix();
        }
    }
}
void dibujaMarcador(){
    glPushMatrix();
    glRasterPos2i(15,alto-15);
    glColor3ub(255, 255, 255);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)"Jugador 1");
    glRasterPos2i(120,alto-15);
    glColor3ub(255, 255, 255);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)"PUNTUACION TOTAL: ");
    glRasterPos2i(320,alto-15);
    glColor3ub(255, 255, 255);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) itoa(puntuacion,str,10) );
    glPopMatrix();
}
void dibujaVidas(){
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glRasterPos2i(360,alto-15);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)" VIDAS: ");
    glRasterPos2i(440,alto-15);
    glColor3ub(255, 255, 255);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) itoa(vidasNave,str,10) );
    glPopMatrix();
}
void dibujaHUD2(){
    if(numJugadores==1){
        glPushMatrix();

        glRasterPos2i(ancho-300,alto-15);
        glColor3ub(0, 247, 0);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)"PULSA INTRO PARA JUGADOR 2");

        glPopMatrix();
    }
    else if(numJugadores==2){
        glPushMatrix();
        glRasterPos2i(ancho-475,alto-15);
        glColor3ub(0, 247, 0);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)"Jugador 2");
        glRasterPos2i(ancho-380,alto-15);
        glColor3ub(0, 247, 0);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)"PUNTUACION TOTAL: ");
        glRasterPos2i(ancho-180,alto-15);
        glColor3ub(0, 247, 0);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) itoa(puntuacion2,str,10) );
        glRasterPos2i(ancho-140,alto-15);
        glColor3ub(0, 247, 0);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)" VIDAS: ");
        glRasterPos2i(ancho-60,alto-15);
        glColor3ub(0, 247, 0);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) itoa(vidasNave2,str,10) );
        glPopMatrix();
    }
}
void iniciaPosicionMalo(){
    posYMalo=alto-100;
    posXMalo=100;
    maxAltoMalo=(alto/10)+30;
    maxAnchoMalo=ancho;
    //Posicion de los malos
    for(i=0; i<filasMalos; i=i+1){
         for(j=0; j<columnasMalos; j=j+2){
            //por cada columna hay que aumentar la posicion x
            malos[i][j]=posXMalo;
            malos[i][j+1]=posYMalo;
            posXMalo=posXMalo+60;
         }
         posXMalo=100;
         posYMalo=posYMalo-60;
    }
    posYMalo=alto-200;
    posXMalo=100;
}

int compruebaMaloDisparado(int maloX, int maloY){
    if(disparosNave[0]>0 && maloX>0){
        int disparoX=disparosNave[0];
        int disparoY=disparosNave[1];
        if((maloX-15<=disparoX && disparoX<=maloX+15) &&
           (maloY<=disparoY+15 && maloY>=disparoY)) {
            //Disparado
            maloX=0;
            disparosNave[0]=0;
            disparosNave[1]=0;
            puntuacion=puntuacion+100;
        }
    }
    if(posXBala2>0 && maloX>0){
        if((maloX-15<=posXBala2 && posXBala2<=maloX+15) &&
           (maloY<=posYBala2+15 && maloY>=posYBala2)) {
            //Disparado
            maloX=0;
            posXBala2=0;
            posYBala2=0;
            puntuacion2=puntuacion2+100;
        }
    }
    return maloX;
}

void dibujarMalos(void)
{
    //Colores
    int red, green, blue;
    //Verde
    red=31;
    green=199;
    blue=22;
    cuentaMalosMuertos=0;
    for(i=0; i<filasMalos; i=i+1){
         for(j=0; j<columnasMalos; j=j+2){
            malos[i][j]=compruebaMaloDisparado(malos[i][j], malos[i][j+1]);
            //Dibujar mientras no este disparado
            if(malos[i][j]>0){
                //Para recoger el ultimo valor del ultimo malo valido
                posYMalo=malos[i][j+1];
                if(derecha){
                    //Asignar valor solo si la posicion proporcionada es mayor
                    if(posXMalo<malos[i][j])
                        posXMalo=malos[i][j];
                }
                //Si va para la izquierda hay que comprobar que haya llegado
                //al limite cualquiera de ellos
                else if(posXMalo>50){
                    //Mientras no haya llegado al final recoger el valor
                    if(malos[i][j]>=50){
                        posXMalo=malos[i][j];
                    }
                    //Si ya llego a 50 -> asignar directamente ese valor para dar la vuelta
                    else{
                        posXMalo=50;
                    }
                }
                int xActual=malos[i][j];
                int yActual=malos[i][j+1];
//                if(creaDisparoMalo)
//                    dibujaDisparosMalo(xActual,yActual);
                glPushMatrix(); //Con esto nos aseguramos que los cambios afectaran solo a este objeto
                glTranslatef(xActual,yActual,0);
                //Malo
                glBegin(GL_QUADS);
                    //Color con el que se pintaran los objetos dibujados
                    glColor3ub(red, green, blue);
                    //Centrado respecto al eje de coordenadas
                    glVertex2i(15, 15);
                    glVertex2i(-15, 15);
                    glVertex2i(-15, -15);
                    glVertex2i(15, -15);

                glEnd();
                glPopMatrix();
            }
            else{
                cuentaMalosMuertos=cuentaMalosMuertos+1;
                malos[i][j+1]=0;
            }
         }
         if(i==1){
            //Azul
            red=22;
            green=78;
            blue=199;
         }
         if(i==3){
            //Morado
            red=141;
            green=14;
            blue=173;
         }
         //Has ganado
         if(cuentaMalosMuertos>=55){
            win=1;
            finJuego=true;
         }
    }
}

void moverMalos(int v)
{

    //Si el malo no ha llegado a la linea de la nave
    if(posYMalo>maxAltoMalo && !pause && !finJuego){
        int anchoMax=maxAnchoMalo-50;
        //Si va a la derecha
        if(derecha){
            //Ir a la derecha mientras no llegue al final
            if(posXMalo<=anchoMax){
                for(i=0; i<filasMalos; i=i+1){
                     for(j=0; j<columnasMalos; j=j+2){
                        if(malos[i][j]>0)
                            malos[i][j]=malos[i][j]+1;
                     }
                }
            }
            //Ahora a la izquierda y bajar de linea
            else{
                derecha=false;
                for(i=0; i<filasMalos; i=i+1){
                     for(j=0; j<columnasMalos; j=j+2){
                        if(malos[i][j]>0)
                            malos[i][j+1]=malos[i][j+1]-15;
                     }
                }
            }
        }
        //Izquierda
        else{
            //Si no ha llegado al final de la linea
            if(posXMalo>50){
                for(i=0; i<filasMalos; i=i+1){
                     for(j=0; j<columnasMalos; j=j+2){
                        if(malos[i][j]>0)
                            malos[i][j]=malos[i][j]-1;
                     }
                }
                posXMalo=posXMalo-1;
            }
            //Ahora a la derecha y bajar de linea
            else{
                derecha=true;
                for(i=0; i<filasMalos; i=i+1){
                     for(j=0; j<columnasMalos; j=j+2){
                        if(malos[i][j]>0)
                            malos[i][j+1]=malos[i][j+1]-15;
                     }
                }
            }
        }
    }
    //Si los malos alcanzan la linea -> quitar una vida y volver a empezar
    else if(vidasNave>0  && !pause && !finJuego){
        vidasNave=vidasNave-1;
        if(numJugadores==2 && vidasNave2>0) vidasNave2-=1;
        else if(numJugadores==2 && vidasNave2<=0){
            win=0;
            finJuego=true;
        }
        iniciaPosicionMalo();
    }
    //Si no quedan vidas -> has perdido
    else if(vidasNave<=1){
        win=0;
        finJuego=true;
    }
    glutTimerFunc(10, moverMalos, 0);
}
void iniciaPosicionNave(){
    //Posicion inicial de la nave
    posXNave=ancho/2;
    posYNave=alto/10;
    maxAnchoNave=ancho;
    //Numero de disparos
    sizeArrayDisparoNave=sizeof disparosNave/sizeof(int);
    //Inicializo los disparos
    for(i=0; i<sizeArrayDisparoNave; i=i+1){
        disparosNave[i]=0;
    }
}
void iniciaPosicionNave2(){
    //Posicion inicial de la nave
    posXNave2=ancho/2;
    posYNave2=alto/10;

    //Inicializo los disparos
    posXBala2=0;
    posYBala2=0;
}
void dibujaDisparosNave(){
    //Disparos
    for(i=0; i<sizeArrayDisparoNave; i=i+2){
        if(disparosNave[i]!=0){
            glPushMatrix(); //Con esto nos aseguramos que los cambios afectaran solo a este objeto
            //Disparos
            glBegin(GL_QUADS);
                //Color con el que se pintaran los objetos dibujados
                glColor3ub(255, 255, 255);
                glVertex2i(disparosNave[i], disparosNave[i+1]);
                glVertex2i(disparosNave[i]+5, disparosNave[i+1]);
                glVertex2i(disparosNave[i]+5, disparosNave[i+1]+15);
                glVertex2i(disparosNave[i], disparosNave[i+1]+15);
            glEnd();
            glPopMatrix();
        }
    }
}
void dibujaDisparosNave2(){
    //Disparos
    if(posXBala2!=0){
        glPushMatrix(); //Con esto nos aseguramos que los cambios afectaran solo a este objeto
        glTranslatef(posXBala2,posYBala2,0);
        //Malo
        glBegin(GL_QUADS);
            //Color con el que se pintaran los objetos dibujados
            glColor3ub(0, 247, 0);
            //Centrado respecto al eje de coordenadas
            glVertex2i(5, 5);
            glVertex2i(10, 5);
            glVertex2i(10, 20);
            glVertex2i(5, 20);

        glEnd();
        glPopMatrix();
    }

}
/* Función que se llamará cada vez que se redimensione la ventana */
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
}
/* Función que controla los eventos de teclado */
void keyboard ( unsigned char key, int x, int y )
{
    keyStates[key] = true;
    switch ( key )
    {
        case 27:  exit(0); /* tecla escape*/
                  break;
        case 'f':
        case 'F':
            //Posicion de la nave despues de redimensionar
            posXNave=ancho/2;
            posYNave=alto/10;
            maxAnchoNave=ancho;
            //Posicion malo
            posYMalo=alto-100;
            posXMalo=100;
            maxAltoMalo=(alto/10)+30;
            maxAnchoMalo=ancho;
            glutFullScreen();
                  break;
        case 'w':
        case 'W':
            //Posicion de la nave despues de redimensionar
            posXNave=ancho/4;
            posYNave=alto/12;
            maxAnchoNave=ancho/2;
            //Posicion malo
            posYMalo=(alto/2)-100;
            posXMalo=100;
            maxAltoMalo=(alto/12)+30;
            maxAnchoMalo=ancho/2;
            glutReshapeWindow(ancho/2, alto/2);
                  break;
        case 'p':
        case 'P':
            pause=!pause;
            break;

        case 13:
            numJugadores=2;
            iniciaPosicionNave2();
            break;
    }

    glutPostRedisplay();
}
void keyboardUp ( unsigned char key, int x, int y )
{
    keyStates[key] = false;
    glutPostRedisplay();
}

/* Función que controla los eventos de teclado */
void specialKeyboard ( int key, int x, int y )
{
    keySpecialStates[key]=true;
    //Redibujar la ventana
    glutPostRedisplay();
}
void specialKeyboardUp(int key, int x, int y){
    keySpecialStates[key]=false;
    glutPostRedisplay();
}
void keyoperations(void)
{
    if (keyStates[' '] && numJugadores==1) {
       for(i=0; i<sizeArrayDisparoNave; i=i+2){
            if(disparosNave[i]==0){
                //Posicion x del disparo
                disparosNave[i]=posXNave-5;
                //Posicion y
                disparosNave[i+1]=posYNave+10;
                i=sizeof(disparosNave);
            }
        }
    }
    if (keyStates['.'] && numJugadores==2) {
       for(i=0; i<sizeArrayDisparoNave; i=i+2){
            if(disparosNave[i]==0){
                //Posicion x del disparo
                disparosNave[i]=posXNave-5;
                //Posicion y
                disparosNave[i+1]=posYNave+10;
                i=sizeof(disparosNave);
            }
        }
    }
    if (keyStates['c'] || keyStates['C']){
        if(numJugadores==2 && posXBala2==0){
            posXBala2=posXNave2-5;
            posYBala2=posYNave+10;
        }
    }
    if (keyStates['a'] || keyStates['A']){
        if(numJugadores==2){
            //Desplazar el objeto siempre que no haya llegado al limite
            if(posXNave2-40>0){
                posXNave2=posXNave2-(ancho/1200);
            }
        }
    }
    if (keyStates['s'] || keyStates['S']){
        if(numJugadores==2){
            int posicion=posXNave2+40;
            if(posicion<=maxAnchoNave){
                posXNave2=posXNave2+(ancho/1200);
            }
        }
    }
}
void keySpecialOperations(void) {
    if (keySpecialStates[GLUT_KEY_LEFT]) { // If the left arrow key has been pressed
        //Desplazar el objeto siempre que no haya llegado al limite
        if(posXNave-40>0){
            posXNave=posXNave-(ancho/1200);
        }
    }
    if (keySpecialStates[GLUT_KEY_RIGHT]) { // If the left arrow key has been pressed
        //Desplazar el objeto siempre que no haya llegado al limite
        //-40 por el tamaño de la nave
        int posicion=posXNave+40;
        if(posicion<=maxAnchoNave){
            posXNave=posXNave+(ancho/1200);
        }
    }
}
/*Funcion para manejo de raton*/
void raton( int button, int state,int x, int y){
    //Para funciones de pulsado
    if(state==GLUT_UP){
        switch ( button )
        {   //Boton derecho del raton
            case GLUT_RIGHT_BUTTON:  exit(0);
                      break;
          /*  case GLUT_MIDDLE_BUTTON:
                glutCreateWindow("Teclas de medio");
                break;*/
        }
    }

    //redibujar la pantalla
    glutPostRedisplay();
}
/* Función principal */
int main(int argc, char** argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    //Tamanio de pantalla y posicion
    glutInitWindowSize(500, 500);
    //glutInitWindowPosition(10, 10);
    //Se inicializa la pantalla, el string es el nombre
    glutCreateWindow("Hola ETSHISHI");
    //El modo pantalla completa debe hacerse despues de crear la ventana
    glutFullScreen();

    //Llamada a funcion init
    init();
    /*Estas funciones reciben por parametro una funcion*/
    //Primera llamada a la funcion de pintado por pantalla
    //Siempre se llama a esta funcion para pintar por primera vez
    if(!pause && !finJuego)
        glutDisplayFunc(display);
    //Redimensiona la pantalla
    glutReshapeFunc(reshape);
    //Funciones de teclado pulsado
    glutKeyboardFunc(keyboard);
    //Funciones de teclado up
    glutKeyboardUpFunc(keyboardUp);
    //Funciones especiales de teclado
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    //Funciones de raton
    glutMouseFunc(raton);
    //Proceso ocioso
    glutIdleFunc(idle);
    //Bucle para repetir estos procesos
    glutMainLoop();

    return 0;
}
