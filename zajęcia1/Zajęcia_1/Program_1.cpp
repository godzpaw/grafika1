//Bartosz Kwiczala
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int width = 800;
int heigth = 600;
GLfloat windowWidth;
GLfloat windowHeight;
void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSwapBuffers();
}
void SetupRC() {
	glClearColor(0.6f, 0.4f, 0.12f, 1.0f);
}
void ChangeSize(GLsizei w, GLsizei h) {
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
	{
		windowHeight = 250.0f*h / w;
		windowWidth = 250.0f;
	}
	else
	{
		windowWidth = 250.0f*w / h;
		windowHeight = 250.0f;
	}
	glOrtho(0.0f, windowWidth, 0.0f, windowHeight, 1.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void Program1(int argc, char*argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, heigth);
	glutCreateWindow("Mój pierwszy program w GLUT");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();
	glutMainLoop();
}
void main(int argc, char* argv[]) {
	Program1(argc, argv);
}