#include <stdlib.h>
#include "GL/glut.h"
#include "math.h"
#include <iostream>
using std::cout;
using std::cin;
#pragma comment(lib, "glut32.lib")
#define WINDOW_TITLE "Project"

char title[] = "Proyecto final";
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 15;        // refresh interval in milliseconds [NEW]
static const double PI = 3.141592654;
/* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
void anillo(double hg, double hc, double rg, double rc, double s) {
	//hg=altura del circulo grande
	//hc=altura del cirgulo chico
	//rg=radio del criculo grande
	//rc=radio del circulo chico
	//s=saltos de angulo en cada iteracion
	glBegin(GL_QUADS);
	for (double j = 0; j < 2 * PI + s; j = j + s) {
		glVertex3f(rg*sin(j + s), rg*cos(j + s), hg);
		glVertex3f(rg*sin(j), rg*cos(j), hg);
		glVertex3f(rc*sin(j), rc*cos(j), hc);
		glVertex3f(rc*sin(j + s), rc*cos(j + s), hc);
	}
	glEnd();
}
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	// Render a color-cube consisting of 6 quads with different colors
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(0.0, 0.0f, -7.0f);  // Move right and into the screen
	glRotatef(angleCube, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW]

	                // Begin drawing the color cube with 6 quads
	   // Top face (y = 1.0f)
	   // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(1.0f, 0, 0);
	double radio = 0;
	double altura = 0;
	double incH = .1;
	double incR = .08;
	double s = .05;
	for (altura = 0; altura <= 1; altura = altura + incH) {
		anillo(altura + (incH / 2), altura, radio+(incR/2), radio, s);
		radio = radio + incR;
	}
	glColor3f(0, 1.0f, 0);
	for (altura = 1; altura >= 0; altura = altura - incH) {
		anillo(altura + (incH / 2), altura, radio, radio + (incR / 2), s);
		radio = radio + incR;
	}
	glColor3f(0, 0, 1.0f);
	for (altura = 0; altura <= 1; altura = altura + incH) {
		anillo(altura + (incH / 2), altura, radio + (incR / 2), radio, s);
		radio = radio + incR;
	}
	double incA=.07;
	glColor3f(0.5f, 0.5f, 0.5f);
	for (double ang = 0; ang < PI / 2; ang=ang + incA) {
		anillo(-radio*sin(ang)+1+incH, -radio*sin(ang+(incA/2))+1+incH, radio * cos(ang), radio * cos(ang + (incA / 2)), s);
		cout << ang<<"\n";
	}

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

	// Update the rotational angle after each refresh [NEW]
	anglePyramid += 0.2f;
	angleCube -= 0.15f;
}

/* Called back when timer expired [NEW] */
void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(640, 480);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	initGL();                       // Our own OpenGL initialization
	glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}