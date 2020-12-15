/*!
	\file		Efectos.cpp
	\brief		Mezcla y niebla en OpenGL

	Muestra unos objetos vistos a traves de una ventana
	-textura con canal alfa- afectados por una niebla
	blanca. El toro y el cilindro son translucidos.

	\author		Roberto Vivo' <rvivo@upv.es>
	\version	1.0
	\date		2016 - 2020
	\copyright(c) upv.es LGPL
*/

#define PROYECTO "ISGI::S9E01::Efectos"

#include <iostream>			
#include <Utilidades.h>

using namespace std;

static float girox = 0, giroy = 0;
static int xanterior, yanterior;

static GLuint texR;
static GLuint texNubes;
static GLuint texVentana;


void init()
// Inicializaciones
{
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL version " << glGetString(GL_VERSION) << endl;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Luces (caracteristicas cromaticas)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, BLANCO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, BLANCO);

	// Texturas (carga de texturas)
	glGenTextures(1, &texR);
	glBindTexture(GL_TEXTURE_2D, texR);
	loadImageFile((char*)"R.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &texNubes);
	glBindTexture(GL_TEXTURE_2D, texNubes);
	loadImageFile((char*)"nubes.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &texVentana);
	glBindTexture(GL_TEXTURE_2D, texVentana);
	loadImageFile((char*)"ventana.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	// Niebla
	glFogfv(GL_FOG_COLOR, BLANCO);
	glFogf(GL_FOG_DENSITY, 0.5);

	// Configurar el motor de render
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_FOG);
}

void dibujarOpacos()
{
	// Material plano
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ORO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, AMARILLO);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glShadeModel(GL_SMOOTH);

	// Plano
	glBindTexture(GL_TEXTURE_2D, texR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glBegin(GL_POLYGON);

	glTexCoord2f(0, 0);
	glVertex3f(-2, -1, 2);

	glTexCoord2f(2, 0);
	glVertex3f(2, -1, 2);

	glTexCoord2f(2, 2);
	glVertex3f(2, -1, -2);

	glTexCoord2f(0, 2);
	glVertex3f(-2, -1, -2);

	glEnd();
}

void dibujarTranslucidos()
{
	// Texturar con coordenadas de textura auto
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Dehabilitar la escritura en z-buffer
	glDepthMask(GL_FALSE);

	// Donut translucido
	glBindTexture(GL_TEXTURE_2D, texNubes);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	// Textura automatica esferica (mapa de entorno)
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	// Material donut
	GLfloat colordonut[] = { 1,1,1, 0.7 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colordonut);
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLANCO);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glShadeModel(GL_SMOOTH);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.4, 1.0, 20, 20);
	glPopMatrix();

	// Cilindro
	GLfloat colorcilindro[] = { 0,1,0, 0.7 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colorcilindro);
	glMaterialfv(GL_FRONT, GL_SPECULAR, VERDE);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.5, 1, 20, 20);
	glPopMatrix();

	// Volver a habilitar el zbuffer
	glDepthMask(GL_TRUE);

	glPopAttrib();
}

void dibujarHUD()
{
	// Dibuja elementos solidarios a la camara
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Cambio de camara a una ortografica
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	// Cambio la situacion de la camara 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Dibujar un poligono que ocupe todo el viewport
	glBindTexture(GL_TEXTURE_2D, texVentana);
	glDisable(GL_LIGHTING);
	GLfloat v0[] = { -1,-1,0 };
	GLfloat v1[] = { 1,-1,0 };
	GLfloat v2[] = { 1,1,0 };
	GLfloat v3[] = { -1,1,0 };
	quadtex(v0, v1, v2, v3, 0, 1, 0, 1);

	// Volver a la camara original
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();

}
void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, texNubes);
	//texturarFondo();

	// Seleccion de la matriz modelo-vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Si situo las luces aqui son solidarias a la camara
	// (luz de minero)



	// Situamos y orientamos la camara
	gluLookAt(2, 2, 3, 0, 0, 0, 0, 1, 0);

	// Si situo las luces aqui son fijas en la escena
	static GLfloat P0[] = { 5,5,5,0 }; // direccional
	glLightfv(GL_LIGHT0, GL_POSITION, P0);
	static GLfloat P1[] = { -5,5,-5,1 }; // puntual
	glLightfv(GL_LIGHT1, GL_POSITION, P1);

	// Geometria
	ejes();

	glRotatef(girox, 1, 0, 0);
	glRotatef(giroy, 0, 1, 0);

	// Primero dibujar opacos
	dibujarOpacos();

	// Despues dibujar translucidos
	dibujarTranslucidos();

	// Dibujar HUD
	dibujarHUD();

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
	giroy += (x - xanterior)*pixel2grados;
	// Al mover el raton hacia abajo la y aumenta 
	// y el giro es alrededor del eje x positivo
	girox += (y - yanterior)*pixel2grados;

	xanterior = x;
	yanterior = y;
	// Encolar un evento de redibujo
	glutPostRedisplay();
}

void main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	FreeImage_Initialise();
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

	// Bucle de atencion a eventos
	glutMainLoop();

	FreeImage_DeInitialise();

}
