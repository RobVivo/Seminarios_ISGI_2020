/*!
	\file		Geometria.cpp
	\brief		Dibujar primitivas y cambiar atributos en OpenGL

	Programa para dibujar un pentagono con color por vertice
	con arrays de dibujo usando *freeglut* y *Utilidades*. Se
	muestra comentado el uso de listas de dibujo.

	\author		Roberto Vivo' <rvivo@upv.es>
	\version	1.0
	\date		2016-2020
	\copyright	(c) upv.es LGPL
 */
 
// #define PROYECTO "ISGI::S2E02::Listas de dibujo"
#define PROYECTO "ISGI::S2E03::Arrays de dibujo"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

//static GLuint pentagono;

static GLfloat coordenadas[10];
static const GLfloat colores[15] = { 1,1,1, 1,0,1, 0,1,1, 1,1,0, 0,0,1 };
static const GLuint pentagono[5] = { 0,1,2,3,4 };

void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	float radio = 1;
	for (int i = 0; i < 10; i += 2) {
		coordenadas[i] = radio * cosf( (float(i)/2) * 2*PI/5 );
		coordenadas[i + 1] = radio * sinf( (float(i)/2) * 2*PI/5 );
	}

	// Habilitar los arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Relleno los arrays
	glVertexPointer(2, GL_FLOAT, 0, coordenadas);
	glColorPointer(3, GL_FLOAT, 0, colores);


	/*! E02 Uso de Display Lists
		pentagono = glGenLists(1);			// Id de la lista
		float radio = 1;
		// Crear la lista
		glNewList(pentagono, GL_COMPILE);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 5; i++) {
			glVertex3f(radio*cos(i * 2 * PI / 5), radio*sin(i * 2 * PI / 5), 0);
		}
		glEnd();
		glEndList();
	*/

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT);

	ejes();

	//glutWireTeapot(0.5);

	// Mandar a dibujar 
	glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_INT, pentagono);

	/*! E02 Uso de Display Lists
		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3f(1, 0, 0);
		glCallList(pentagono);

		glPushAttrib(GL_CURRENT_BIT);
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1, 1, 1);
		glCallList(pentagono);
		glPopAttrib();

		glPolygonMode(GL_FRONT, GL_POINT);
		glPointSize(10);
		glCallList(pentagono);
	*/

	glFlush();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
}

int main(int argc, char** argv)
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
