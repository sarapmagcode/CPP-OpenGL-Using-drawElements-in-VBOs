#define GLEW_STATIC // <----- define GLEW_STATIC
#include <iostream>
#include <GL/glew.h> // <---- additional header files
#include <GL/glut.h>

using namespace std;

// Function Prototypes
void display();
void triangle();
void init();
void modifyTriangle(int value);
void monitorKeyboard(unsigned char key, int x, int y);
// Global Variables
bool triangleUpdate = false; // flag to check if triangle is for update
GLfloat *mappedVertices = NULL;
GLuint VBOid;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	glutKeyboardFunc(monitorKeyboard);
	GLenum err = glewInit(); 
	if (err == GLEW_OK) {
		glutMainLoop();
	}
	return 0;
}

void monitorKeyboard(unsigned char key, int x, int y){
	if (key == 27) { // ESC
		// Disable client side capability for normal operation
		glDisableClientState(GL_VERTEX_ARRAY);
		// bind with 0, so, switch back to normal pointer operation
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VBOid);
		exit(0);
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(.16f,.72f,.08f,1.0f);
	triangle();
	glFlush();
	glutTimerFunc(1000, modifyTriangle, 0);
}

void modifyTriangle(int value) {
	triangleUpdate = true;
	// Map the array from the server memory (video card) to the local variable
	mappedVertices = (GLfloat *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	mappedVertices[0] += .01f;
	mappedVertices[3] += .01f;
	mappedVertices[6] += .01f;
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glutPostRedisplay();
}

void triangle() {
	if (!triangleUpdate) {
		// 1. Initialize the triangle using an array
		GLfloat trianglevertices[] = {
			0.0f, .75f, 0.0f,
			-0.75f, 0.0f, 0.0f,
			0.75f, 0.0f, 0.0f
		};
		
		// 2. Generate a Vertex Object ID for your array of vertices and bind it
		glGenBuffers(1, &VBOid);
		glBindBuffer(GL_ARRAY_BUFFER, VBOid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, trianglevertices, GL_DYNAMIC_DRAW);
		
		// 3. Activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		
		// 4. Draw the primitive object
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	} else {
		// When for update just draw the primitive object
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}