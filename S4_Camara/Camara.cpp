/*!
	\file		Camara.cpp
	\brief		Uso de la camara en OpenGL

	Programa para mostrar la configuracion de camaras
	ortografica y perspectiva en OpenGL, la forma de
	situarlas en la escena y como se habilita el proceso
	de visibilidad. Se usa *freeglut* y *Utilidades*

	\author		Roberto Vivo' <rvivo@upv.es>
	\version	1.0
	\date		2016 - 2020
	\copyright(c) upv.es LGPL
*/

#define PROYECTO "ISGI::S4E01::Uso de camara"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

	// Configurar el motor de render
	glEnable(GL_DEPTH_TEST);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccion de la matriz modelo-vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situamos y orientamos la camara
	// (punto de vista,punto de interes,vertical subjetiva)
	gluLookAt(2, 5, -3, 0, 0, -1, 0, 1, 0);

	ejes();

	glTranslatef(0, 0, -1);

	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(0, 2, 0);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	glColor3f(1, 1, 1);
	glutSolidTeapot(0.5);
	glColor3f(1, 0, 0);
	glutWireTeapot(0.51);

	glFlush();
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
	// Camara ortografica con isometria y
	// ventana 4x4 del mundo real
	if(ra<1)
	 glOrtho(-2, 2, -2/ra, 2/ra, -1, 10);
	else
	 glOrtho(-2*ra, 2*ra, -2, 2, -1, 10);
	*/

	// Camara perspectiva. La distancia al plano cercano siempre mayor que 0
	gluPerspective(40, ra, 0.1, 100);
}

void main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}
