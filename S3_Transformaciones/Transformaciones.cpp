/*!
	\file		Transformaciones.cpp
	\brief		Uso de transformaciones geometricas en OpenGL

	Programa para dibujar instancias de una flor compuesta
	por listas de dibujo petalo, corola y tallo usando
	*freeglut* y *Utilidades*

	\author		Roberto Vivo' <rvivo@upv.es>
	\version	1.0
	\date		2016-2020
	\copyright	(c) upv.es LGPL
 */

#define PROYECTO "ISGI::S3E01::Transformaciones"

#include <iostream>	
#include <cstdlib>
#include <Utilidades.h>

GLuint petalo, corola, flor;

using namespace std;

void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

	// Petalo
	petalo = glGenLists(1);
	glNewList(petalo, GL_COMPILE);

	glColor3f(1, 1, 1);
	glPushMatrix();
	glScalef(0.15, 0.5, 0.15);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glEndList();

	// Corola
	corola = glGenLists(1);
	glNewList(corola, GL_COMPILE);
	for (auto i = 0; i < 12; i++) {
		glPushMatrix();
		glRotatef(i * 30.0, 0, 0, 1);
		glTranslatef(0, 0.25, 0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(petalo);
		glPopMatrix();
	}
	glColor3f(1, 1, 0);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();
	glEndList();

	// Flor
	flor = glGenLists(1);
	glNewList(flor, GL_COMPILE);

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0, -0.25, 0);
	glScalef(0.05, 0.75, 0.05);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(10, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glCallList(corola);
	glPopMatrix();

	glEndList();
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i = 0; i < 100; i++) {
		glPushMatrix();
		glTranslatef(
			(rand() % 200 - 100) / 100.0,
			(rand() % 100 - 50) / 100.0,
			0.0
		);
		glScalef(0.5, 0.5, 0.5);
		glCallList(flor);
		glPopMatrix();
	}


	glFlush();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
}

void main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}
