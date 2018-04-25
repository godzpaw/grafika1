//Bartosz Kwiczala
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>;
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


using namespace std;



GLfloat x = 50.0f;
GLfloat y = 150.0f;
GLsizei polygonRadiusSize = 50;
int numberOfSides = 5;

GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

int width = 800;
int heigth = 600;

GLfloat windowWidth;
GLfloat windowHeight;

float * X = new float[numberOfSides];
float * Y = new float[numberOfSides];

float min(float * inArray) {
	float min = inArray[0];
	for (int i = 1; i < numberOfSides; i++)
	{
		if (min > inArray[i])
		{
			min = inArray[i];
		}
	}
	return min;
}
float max(float * inArray) {
	float max = inArray[0];
	for (int i = 1; i < numberOfSides; i++)
	{
		if (max < inArray[i])
		{
			max = inArray[i];
		}
	}
	return max;
}

void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.1f, 0.12f);
	glBegin(GL_POLYGON);
	for (int i = 0; i < numberOfSides; i++)
	{
		float xx = x + polygonRadiusSize * sin(2.0*M_PI*i / numberOfSides);
		float yy = y + polygonRadiusSize * cos(2.0*M_PI*i / numberOfSides);
		X[i] = xx;
		Y[i] = yy;
		//cout << round(xx) << ","<< round(yy) <<"  " <<i<< endl;
		glVertex2f(xx, yy);
	}
	glEnd();
	glutSwapBuffers();
}

void TimerFunction(int value) {
	float minFromX = min(X); 
	float maxFromX = max(X); 
	float minFromY = min(Y); 
	float maxFromY = max(Y); 
	//ystep = 0;
	if (x > windowWidth - polygonRadiusSize || x < polygonRadiusSize)
		xstep = -xstep;
	if (y > windowHeight - polygonRadiusSize || y < polygonRadiusSize - minFromY)
		ystep = -ystep;
	/*if (y > windowHeight - polygonRadiusSize || (minFromY-polygonRadiusSize) <0)
		ystep = -ystep;*/
	x += xstep;
	y += ystep;
	glutReshapeWindow(width,heigth); 
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}
 
void TimerFunctionHomework(int value) {
	float minFromX = min(X); 
	float maxFromX = max(X); 
	float minFromY = min(Y); 
	float maxFromY = max(Y); 
	bool leftRigthCollision = x > windowWidth - polygonRadiusSize || x < polygonRadiusSize;
	bool topBottomCollision = y > windowHeight - polygonRadiusSize || y < polygonRadiusSize - minFromY;
	if (y > windowHeight - polygonRadiusSize )
	{
		ystep = 0;
	}
	if (x > windowWidth - polygonRadiusSize)
	{
		xstep = 0;
		ystep = -1;
	}
	if (y < polygonRadiusSize - minFromY && x > windowWidth - polygonRadiusSize)
	{
		ystep = 0;
		xstep = -1;
	}
	if (x < polygonRadiusSize &&y < polygonRadiusSize - minFromY)
	{
		xstep = 0;
		ystep = 1;
	}
	if (x < polygonRadiusSize &&y > windowHeight - polygonRadiusSize)
	{
		ystep = 0;
		xstep = 1;
	}
	x += xstep;
	y += ystep;
	glutReshapeWindow(width, heigth); 
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunctionHomework, 1);
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

void Program3(int argc, char*argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, heigth);
	glutCreateWindow("Mój pierwszy program w GLUT");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(10, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
	delete X;
	delete Y;
}
void main(int argc, char* argv[]) {
	Program3(argc, argv);
	

}