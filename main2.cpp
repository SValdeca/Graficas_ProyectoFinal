#include <stdlib.h>
#include "GL/glut.h"
#include "math.h"
#include <iostream>
#define WINDOW_TITLE "Project"

char title[] = "Proyecto final";
static const double PI = 3.141592654;
/* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
double color = 0;
bool cambio = true;
GLfloat w, h, phi = 0, theta = 0, radio = 7, movX, movY, movZ;
GLfloat phi2=0, theta2=0, r=3;

void pintaFigura() {
	
	glColor3f(1.0f, 0, 0);
	double radio = 0;
	double altura = 0;
	double incH = .1;
	double incR = .08;
	double s = .05;
	GLUquadric* quad = gluNewQuadric();

	for (altura = 0; altura <= 1; altura = altura + incH) {
		gluCylinder(quad, radio, radio + (incR / 2), (incH / 2), 100, 100);
		glTranslatef(0, 0, incH);
		radio = radio + incR;
	}

	glColor3f(0, 1.0f, 0);
	for (altura = 1; altura >= 0; altura = altura - incH) {
		//anillo(altura + (incH / 2), altura, radio, radio + (incR / 2), s);
		gluCylinder(quad, radio + (incR / 2), radio, (incH / 2), 100, 100);
		glTranslatef(0, 0, -incH);
		radio = radio + incR;
	}
	glColor3f(0, 0, 1.0f);
	for (altura = 0; altura <= 1; altura = altura + incH) {
		//anillo(altura + (incH / 2), altura, radio + (incR / 2), radio, s);
		gluCylinder(quad, radio, radio + (incR / 2), (incH / 2), 100, 100);
		glTranslatef(0, 0, incH);
		radio = radio + incR;
	}

	glColor4f(color, 0, 1 - color, 0.3);
	glutSolidSphere(radio, 100, 100);
	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

	if (cambio) {
		color += .005;
		cambio = (color < 1);
	}
	else {
		color -= .005;
		cambio = (color <= 0);
	}
}

void luces() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 0.15 };
	GLfloat mat_diffuse[] = { 0.5, 0.95, 0.8, 0.2 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 0.3 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 32);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glEnable(GL_COLOR_MATERIAL);
}

void display() {
	luces();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	GLfloat posX = r * cos(phi2) * sin(theta2), posY = r * sin(phi2) * sin(theta2), posZ = r * cos(theta2);
	GLfloat movX = radio * cos(phi) * sin(theta);
	GLfloat movY = radio * sin(phi) * sin(theta);
	GLfloat movZ = radio * cos(theta);

	glViewport(0, 0, w/2, h);
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(0.0, 0.0f, -7.0f);  // Move right and into the screen
	gluLookAt(posX, posY, posZ, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
	pintaFigura();

	glViewport(w/3, 0, w, h);
	glLoadIdentity();
	gluLookAt(movX, movY, movZ, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);

	pintaFigura();

	theta += 0.005;
	phi += 0.005;
}

void inputTeclas(unsigned char key, int x, int y) {
	switch (key) {
		case  'a'://rotar en x
			theta2 -= .01;
		break;
		case 's'://rotar en y
			phi2 -= .01;
		break;
		case  'w'://rotar en x
			phi2 += .01;
		break;
		case 'd'://rotar en y
			theta2 += .01;
		break;
		case  'e'://zoom in
			r += 0.1;
		break;
		case 'q'://zoom out
			r -= 0.1;
		break;
		case 'r': //activar la luz
			glEnable(GL_LIGHT2);
			
			GLfloat posL[] = { x, y, 0, 0.5f };
			glLightfv(GL_LIGHT2,GL_SPECULAR,posL);
		break;
	}

	glutPostRedisplay();
}

void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(1000/60, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	w = (GLfloat)width;
	h = (GLfloat)height;

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);            // Initialize GLUT

	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	glEnable(GL_BLEND);
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(1280, 900);   // Set the window's initial width & height

	glutInitWindowPosition(30, 30);
	glutCreateWindow(title);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(inputTeclas);

	initGL();                       // Our own OpenGL initialization
	glutTimerFunc(1000, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
