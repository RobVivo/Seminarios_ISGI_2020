/*!
	\file		Iluminacion.cpp
	\brief		Uso de luces y materiales en OpenGL

	Muestra dos figuras GLUT con interfaz de inspeccion.
	Una de ellas se muestra en sombrado constante y la otra
	con sombreado de Gouraud, ambas con su propio material.
	Se puede activar y desactivar luces desde teclado -0 y 1-

	\author		Roberto Vivo' <rvivo@upv.es>
	\version	1.0
	\date		2016 - 2020
	\copyright(c) upv.es LGPL
*/

#define PROYECTO "ISGI::S7E01::Iluminacion"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

static float girox = 0, giroy = 0;
static int xanterior, yanterior;


void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;
	cout << endl <<
		"Pulse 0 para encender/apagar LIGHT0" << endl <<
		"Pulse 1 para encender/apagar LIGHT1" << endl;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Luces (caracteristicas cromaticas)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, BLANCO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, GRISOSCURO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, GRISCLARO);


	// Configurar el motor de render
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccion de la matriz modelo-vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Si situo las luces aqui son solidarias a la camara
	// (luz de minero)

	static GLfloat P1[] = { 0,0,-1,1 }; // puntual a 1 delante de la camara
	glLightfv(GL_LIGHT1, GL_POSITION, P1);

	// Situamos y orientamos la camara
	gluLookAt(2, 2, 3, 0, 0, 0, 0, 1, 0);

	// Si situo las luces aqui son fijas en la escena
	static GLfloat P0[] = { -5,5,0,0 }; // direccional
	glLightfv(GL_LIGHT0, GL_POSITION, P0);


	ejes();


	// Dos donuts
	glRotatef(girox, 1, 0, 0);
	glRotatef(giroy, 0, 1, 0);

	//glColor3f(1, 1, 0);
	// Material brillante
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ORO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, AMARILLO);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glShadeModel(GL_SMOOTH);
	glutSolidTorus(0.2, 0.5, 50, 50);

	glRotatef(90, 1, 0, 0);
	//glColor3f(0, 0, 1);
	// Material opaco
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLANCO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glShadeModel(GL_FLAT);
	glutSolidTorus(0.2, 0.5, 20, 20);

	glutSwapBuffers();
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

	// Camara perspectiva
	gluPerspective(40, ra, 0.1, 100);
}

void onClick(int boton, int estado, int x, int y)
{
	// Callback de atencion al pulsado de un boton del raton

	// Almacenar la posicion cuando click
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
	}
}

void onDrag(int x, int y)
{
	// Calbback de arrastre del raton pulsado

	const float pixel2grados = 1;
	// Al mover el raton hacia la derecha la x aumenta
	// y el giro es alrededor del eje y positivo 
	giroy += (x - xanterior) * pixel2grados;
	// Al mover el raton hacia abajo la y aumenta 
	// y el giro es alrededor del eje x positivo
	girox += (y - yanterior) * pixel2grados;

	xanterior = x;
	yanterior = y;
	// Encolar un evento de redibujo
	glutPostRedisplay();
}

//! Apaga y enciende luces
void onKey(unsigned char tecla, int x, int y)
{
	static bool L0 = true;
	static bool L1 = true;
	switch (tecla) {
	case '0': 
		L0 ? glDisable(GL_LIGHT0) : glEnable(GL_LIGHT0);
		L0 = !L0;
		break;
	case '1':
		L1 ? glDisable(GL_LIGHT1) : glEnable(GL_LIGHT1);
		L1 = !L1;
		break;
	}
	glutPostRedisplay();
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
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);
	glutKeyboardFunc(onKey);

	// Bucle de atencion a eventos
	glutMainLoop();
}
