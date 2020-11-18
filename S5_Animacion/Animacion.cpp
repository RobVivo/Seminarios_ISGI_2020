/*!
	\file		Animacion.cpp
	\brief		Animacion temporalmente coherente

	Muestra dos teteras girando a distinta velocidad angular
	usando timers para controlar los FPS maximos. Se muestra 
	la tasa FPS en tiempo real en la barra de titulo

	\author		Roberto Vivo' <rvivo@upv.es>
	\version	1.0
	\date		2016 - 2020
	\copyright(c) upv.es LGPL
*/

#define PROYECTO "ISGI::S5E01::Animacion"

#include <iostream>	
#include <sstream>
#include <Utilidades.h>

using namespace std;

#define tasaFPS 40

// Variable dependiente del tiempo
static float alfa = 0;

void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

	// Configurar el motor de render
	glEnable(GL_DEPTH_TEST);
}

void FPS()
{
	// Muestra los FPS en la barra de titulo
	int ahora, tiempoT;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;

	fotogramas++;

	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempoT = ahora - antes;

	if (tiempoT > 1000) {
		// modificar el titulo
		stringstream titulo;
		titulo << "FPS =" << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());

		// reiniciar la cuenta
		fotogramas = 0;
		antes = ahora;
	}
}
void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccion de la matriz modelo-vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situamos y orientamos la camara
	gluLookAt(2, 2, -3, 0, 0, 0, 0, 1, 0);

	ejes();

	// teteras
	glPushMatrix();
	glTranslatef(-0.5, 0, 0);
	glColor3f(1, 1, 1);
	glRotatef(alfa, 0, 1, 0);
	glutWireTeapot(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(0.5, 0, 0);
	glRotatef(alfa / 2, 0, 1, 0);
	glutWireTeapot(0.51);
	glPopMatrix();

	glutSwapBuffers();

	FPS();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	float ra = (float)w / h;

	// Fijamos el marco dentro de la ventana de dibujo
	glViewport(0, 0, w, h);

	// Seleccionar la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	// Camara ortografica con isometria
	if(ra<1)
	 glOrtho(-2, 2, -2/ra, 2/ra, -1, 10);
	else
	 glOrtho(-2*ra, 2*ra, -2, 2, -1, 10);
	*/

	// Camara perspectiva
	gluPerspective(40, ra, 0.1, 100);
}

void update()
{
	// Callback de atencion al evento idle

	// Cambiar la variable temporal sin atender al tiempo
	// alfa += 0.1;

	// Control del tiempo transcurrido
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	float tiempoTranscurrido = float(ahora - antes) / 1000;

	// Velocidad angular constante
	static const float omega = 1; // Vueltas/sg.

	alfa += omega * 360 * tiempoTranscurrido;

	// Actualizar la hora
	antes = ahora;

	// Encolar un evento de display
	glutPostRedisplay();
}

void onTimer(int tiempo)
{
	// Encolar un nuevo temporizador
	glutTimerFunc(tiempo, onTimer, tiempo);

	// Llamar a update
	update();
}

void main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutIdleFunc(update);   // Captura idle
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);

	// Bucle de atencion a eventos
	glutMainLoop();
}
