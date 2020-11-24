/*!
	\file		Interaccion.cpp
	\brief		Captura de eventos de usuario

	Muestra dos teteras sensibles a la selección (picking).
	El usuario puede examinarlas separadamente con una
	interfaz de inspección (click & drag). Se puede cambiar
	el aspecto de las teteras mediante las teclas 'a','s' y
	'd' o con un menú de contexto (pop up).

	\author		Roberto Vivo' <rvivo@upv.es>
	\version	1.0
	\date		2016 - 2020
	\copyright(c) upv.es LGPL
*/


#define PROYECTO "ISGI::S6E01::Interaccion"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

static enum { ALAMBRICO, SOLIDO, DOBLE } modo;
//static float girox = 0, giroy = 0;
static float giroxTB = 0, giroyTB = 0;
static float giroxTA = 0, giroyTA = 0;
static GLubyte objeto[1];

static int xanterior, yanterior;

void onMenu(int opcion)
{
	// Callback de atencion al menu de pop-up
	if (opcion == 0) modo = ALAMBRICO;
	else if (opcion == 1) modo = SOLIDO;
	else if (opcion == 2) modo = DOBLE;

	glutPostRedisplay();
}
void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

	// Menu de contexto
	glutCreateMenu(onMenu);
	glutAddMenuEntry("ALAMBRICO", 0);
	glutAddMenuEntry("SOLIDO", 1);
	glutAddMenuEntry("DOBLE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

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
	gluLookAt(2, 2, 3, 0, 0, 0, 0, 1, 0);

	ejes();


	// Tetera Blanca
	glPushMatrix();
	glTranslatef(-0.5, 0, 0);
	glRotatef(giroxTB, 1, 0, 0);
	glRotatef(giroyTB, 0, 1, 0);
	glColor3f(1, 1, 1);
	ejes();
	if (modo == ALAMBRICO) glutWireTeapot(0.5);
	else if (modo == SOLIDO) glutSolidTeapot(0.5);
	else {
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(1, 0, 0);
		glutWireTeapot(0.51);
		glPopAttrib();
		glutSolidTeapot(0.5);
	}
	glPopMatrix();

	// Tetera Amarilla
	glPushMatrix();
	glTranslatef(0.5, 0, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	glColor3f(1, 1, 0);
	ejes();
	if (modo == ALAMBRICO) glutWireTeapot(0.5);
	else if (modo == SOLIDO) glutSolidTeapot(0.5);
	else {
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(1, 0, 0);
		glutWireTeapot(0.51);
		glPopAttrib();
		glutSolidTeapot(0.5);
	}
	glPopMatrix();

	glutSwapBuffers();
}

void select()
{
	// Dibuja sobre el backbuffer en modo solido
	// sin hacer el swapBuffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Seleccion de la matriz modelo-vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situamos y orientamos la camara
	gluLookAt(2, 2, 3, 0, 0, 0, 0, 1, 0);

	// Tetera Blanca
	glPushMatrix();
	glTranslatef(-0.5, 0, 0);
	glRotatef(giroxTB, 1, 0, 0);
	glRotatef(giroyTB, 0, 1, 0);
	glColor3f(1, 1, 1);
	glutSolidTeapot(0.5);
	glPopMatrix();

	// Tetera Amarilla
	glPushMatrix();
	glTranslatef(0.5, 0, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	glColor3f(1, 1, 0);
	glutSolidTeapot(0.5);
	glPopMatrix();
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

void onKey(unsigned char tecla, int x, int y)
{
	// Callback de atencion al pulsado de letras y numeros
	switch (tecla) {
	case 'a':modo = ALAMBRICO;
		break;
	case 's': modo = SOLIDO;
		break;
	case 'd': modo = DOBLE;
		break;
	case 27:exit(0);
	}

	// Encolar el evento de redibujo
	glutPostRedisplay();
}

void onClick(int boton, int estado, int x, int y)
{
	// Callback de atencion al pulsado de un boton del raton

	// Almacenar la posicion cuando click
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
	}

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); // Devuelve el viewport
	GLint yv = viewport[3] - y;
	select(); // Hace el render en el backbuffer
	glReadPixels(x, yv, 1, 1, GL_BLUE, GL_UNSIGNED_BYTE, objeto);

}

void onDrag(int x, int y)
{
	// Calbback de arrastre del raton pulsado

	const float pixel2grados = 1;
	// Al mover el raton hacia la derecha la x aumenta
	// y el giro es alrededor del eje y positivo 
	//giroy += (x - xanterior)*pixel2grados;
	// Al mover el raton hacia abajo la y aumenta 
	// y el giro es alrededor del eje x positivo
	//girox += (y - yanterior)*pixel2grados;

	if (objeto[0] == 0x00) { // Pincho sobre amarilla
		giroyTA += (x - xanterior) * pixel2grados;
		giroxTA += (y - yanterior) * pixel2grados;
	}
	else {
		giroyTB += (x - xanterior) * pixel2grados;
		giroxTB += (y - yanterior) * pixel2grados;
	}

	xanterior = x;
	yanterior = y;
	// Encolar un evento de redibujo
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
	glutKeyboardFunc(onKey);
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);

	// Bucle de atencion a eventos
	glutMainLoop();
}
