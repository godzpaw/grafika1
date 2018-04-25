//Bartosz Kwiczala
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


using namespace std;



POINT mousePoint;
POINT centerCor;


int width = 800;
int heigth = 600;


GLdouble X, Y, Z;


vector<POINT> points;


bool worldState = false;


void Center(bool state) {
	worldState = state;
}
void  GetWorldCor()
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	double winX = (double)mousePoint.x;
	double winY = viewport[3] - (double)mousePoint.y;
	gluUnProject(mousePoint.x, mousePoint.y, 0, modelview, projection, viewport, &X, &Y, &Z);
	Y += heigth;
	if (!worldState)
	{
		POINT result;
		result.x = X;
		result.y = Y;
		points.push_back(result);
		cout << points.size() << " Edges" << endl;
	}
	else
	{
		centerCor.x = X;
		centerCor.y = Y;
		points.clear();
		Center(false);
	}
}
void mouse(int button, int state, int xIn, int yIn)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mousePoint.x = xIn;
		mousePoint.y = -yIn;
		GetWorldCor();
		glutPostRedisplay();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		cout << " deleting polygon ! " << endl;
		mousePoint.x = xIn;
		mousePoint.y = -yIn;
		Center(true);
		GetWorldCor();
		glutPostRedisplay();
	}
}
void mouseFunction(int xIn, int yIn) {
	mousePoint.x = xIn;
	mousePoint.y = -yIn;
	GetWorldCor();
	glutPostRedisplay();
}
void drawPolygon() {
	glPointSize(4.0f);
	glBegin(GL_POLYGON);
	if (worldState)
	{
		cout << "drawing others" << endl;
		glVertex2f(centerCor.x, centerCor.y);

	}
	else
	{
		cout << "drawing normals" << endl;
		for (int i = 0; i < points.size(); i++)
			glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}
void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawPolygon();
	glFlush();
}
void SetupRC(void) {
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}
void ChangeSize(int w, int h) {
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
	{
		heigth = 250.0f*h / w;
		width = 250.0f;
	}
	else
	{
		width = 250.0f*w / h;
		heigth = 250.0f;
	}
	glOrtho(0.0f, width, 0.0f, heigth, 1.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void TimerFunction(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, heigth);
	glutCreateWindow("Mój pierwszy program w GLUT");
	glutMouseFunc(mouse);
	glutMotionFunc(mouseFunction);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(10, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}