/*! 
	\file		HolaMundo.cpp
	\brief		Plantilla basica para OpenGL
	
	Programa para generar una ventana OpenGL usando *freeglut*

	\author		Roberto Vivo' <rvivo@upv.es>
	\version	1.0
	\date		2016-2020
	\copyright	(c) upv.es LGPL 
 */
 

#define PROYECTO "ISGI::S1E01::Hola OpenGL"

#include <iostream>			
#include <gl/freeglut.h>

using namespace std;

//! Inicializaciones
void init()
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

//! Callback de dibujo
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

//! Callback de redimensionamiento
void reshape(GLint w, GLint h)
{
}

//! Programa principal
void main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 400);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}