﻿#include <GL/glut.h>
#include "glext.h"
#ifndef WIN32
#define GLX_GLXEXT_LEGACY
#include <GL/glx.h>
#define wglGetProcAddress glXGetProcAddressARB
#endif


#include <stdlib.h>
#include <stdio.h>
#include "colors.h"
#include "targa.h"
#include <GLFW/glfw3.h>




PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL;






enum
{
	VENUS_TEX,      // Wenus
	EARTH_TEX,      // Ziemia
	MARS_TEX,       // Mars
	FULL_WINDOW,    // aspekt obrazu - całe okno
	ASPECT_1_1,     // aspekt obrazu 1:1
	EXIT            // wyjście
};



int aspect = FULL_WINDOW;




#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif



const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;



GLfloat rotatex = 270.0;
GLfloat rotatez = 0.0;



int button_state = GLUT_UP;




int button_x, button_y;





GLfloat scale = 1.5;

// identyfikatory tekstur

GLuint EARTH;

// identyfikator bieżącej tekstury

GLuint texture;

// filtr pomniejszający

GLint min_filter = GL_LINEAR_MIPMAP_LINEAR;

// funkcja rysująca napis w wybranym miejscu
// (wersja korzystająca z funkcji glWindowPos2i)

void DrawString(GLint x, GLint y, char *string)
{
	// położenie napisu
	glWindowPos2i(x, y);

	// wyświetlenie napisu
	int len = strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
}

// funkcja generująca scenę 3D

void DisplayScene()
{
	// kolor tła - zawartość bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru i bufora głębokości
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesunięcie układu współrzędnych obiektów do środka bryły odcinania
	glTranslatef(0.0, 0.0, -(near + far) / 2);

	// obroty obiektu
	glRotatef(rotatex, 1.0, 0.0, 0.0);
	glRotatef(rotatez, 0.0, 0.0, 1.0);

	// skalowanie obiektu - klawisze "+" i "-"
	glScalef(scale, scale, scale);

	// włączenie testu bufora głębokości
	glEnable(GL_DEPTH_TEST);

	// włączenie teksturowania dwuwymiarowego
	glEnable(GL_TEXTURE_2D);

	// filtr powiększający
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// filtr pomniejszający
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

	// dowiązanie wybranej tekstury
	glBindTexture(GL_TEXTURE_2D, texture);

	// ustawienie parametów środowiska tekstur
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// utworzenie kwadryki
	GLUquadricObj *quadobj = gluNewQuadric();

	// styl (wygląd) generowanej kwadryki
	gluQuadricDrawStyle(quadobj, GLU_FILL);

	// sposób generacji wektorów normalnych
	gluQuadricNormals(quadobj, GLU_SMOOTH);

	// nałożenie tekstury na kwadrykę
	gluQuadricTexture(quadobj, GL_TRUE);

	// narysowanie kuli
	gluSphere(quadobj, 1.0, 30, 30);

	// usunięcie kwadryki
	gluDeleteQuadric(quadobj);

	// wyłączenie teksturowania dwuwymiarowego
	glDisable(GL_TEXTURE_2D);

	// informacje o wybranych parametrach bieżącej tekstury
	char string[200];
	GLfloat var;
	glColor3fv(Black);

	// wartość priorytetu tekstury
	glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, &var);
	sprintf(string, "GL_TEXTURE_PRIORITY = %f", var);
	DrawString(2, 2, string);

	// czy tekstura jest rezydentna
	glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_RESIDENT, &var);
	if (var == GL_FALSE)
		strcpy(string, "GL_TEXTURE_RESIDENT = GL_FALSE");
	else
		strcpy(string, "GL_TEXTURE_RESIDENT = GL_TRUE");
	DrawString(2, 16, string);

	// szerokość tekstury (poziom 0)
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &var);
	sprintf(string, "GL_TEXTURE_WIDTH = %f", var);
	DrawString(2, 30, string);

	// wysokość tekstury (poziom 0)
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &var);
	sprintf(string, "GL_TEXTURE_HEIGHT = %f", var);
	DrawString(2, 46, string);

	// skierowanie poleceñ do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
}

// zmiana wielkości okna

void Reshape(int width, int height)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokość okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom*height / width, top*height / width, near, far);
		else

			// szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left*width / height, right*width / height, bottom, top, near, far);
	}
	else
		glFrustum(left, right, bottom, top, near, far);

	// generowanie sceny 3D
	DisplayScene();
}

// obsługa klawiatury

void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		scale += 0.05;
	else

		// klawisz -
		if (key == '-' && scale > 0.05)
			scale -= 0.05;

	// narysowanie sceny
	DisplayScene();
}

// obsługa przycisków myszki

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapamiętanie stanu lewego przycisku myszki
		button_state = state;

		// zapamiętanie położenia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}

// obsługa ruchu kursora myszki

void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		rotatez += 30 * (right - left) / glutGet(GLUT_WINDOW_WIDTH) * (x - button_x);
		button_x = x;
		rotatex -= 30 * (top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) * (button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}

// obsługa menu podręcznego

void Menu(int value)
{
	switch (value)
	{
	
	case EARTH_TEX:
		texture = EARTH;
		DisplayScene();
		break;



		// filtr pomniejszający
	case GL_NEAREST_MIPMAP_NEAREST:
	case GL_NEAREST_MIPMAP_LINEAR:
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
		min_filter = value;
		DisplayScene();
		break;

		// obszar renderingu - całe okno
	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// wyjście
	case EXIT:
		exit(0);
	}
}

// utworzenie tekstur

void GenerateTextures()
{
	// zmienne użyte przy obsłudze plików TARGA
	GLsizei width, height;
	GLenum format, type;
	GLvoid *pixels;

	// tryb upakowania bajtów danych tekstury
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);



	
	
	/*GLboolean error = load_targa("merk.tga", width, height, format, type, pixels);*/
	GLboolean error = load_targa("mar0kuu2.tga", width, height, format, type, pixels);


	// błąd odczytu pliku
	if (error == GL_FALSE)
	{
		printf("Niepoprawny odczyt pliku ura0fss1.tga");
		system("pause");
	}

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &EARTH);

	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, EARTH);

	// utworzenie tekstury wraz z mipmapami
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, format, type, pixels);

	// porządki
	delete[](unsigned char*)pixels;

	

	// identyfikator bieżącej tekstury
	texture = EARTH;
}

// sprawdzenie i przygotowanie obsługi wybranych rozszerzeñ

void ExtensionSetup()
{
	// pobranie numeru wersji biblioteki OpenGL
	const char *version = (char*)glGetString(GL_VERSION);

	// odczyt wersji OpenGL
	int major = 0, minor = 0;
	if (sscanf(version, "%d.%d", &major, &minor) != 2)
	{
#ifdef WIN32
		printf("Błędny format wersji OpenGL\n");
#else

		printf("Bledny format wersji OpenGL\n");
#endif

		exit(0);
	}

	// sprawdzenie czy jest co najmniej wersja 1.4
	if (major > 1 || minor >= 4)
	{
		// pobranie wskaŸnika wybranej funkcji OpenGL 1.4
		glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i");
	}
	else
		// sprawdzenie czy jest obsługiwane rozszerzenie ARB_window_pos
		if (glutExtensionSupported("GL_ARB_window_pos"))
		{
			// pobranie wskaŸnika wybranej funkcji rozszerzenia ARB_window_pos
			glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress
			("glWindowPos2iARB");
		}
		else
		{
			printf("Brak rozszerzenia ARB_window_pos!\n");
			exit(0);
		}
}

int main(int argc, char *argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// rozmiary głównego okna programu
	glutInitWindowSize(500, 500);

	// utworzenie głównego okna programu
	glutCreateWindow("Tekstura");

	// dołączenie funkcji generującej scenę 3D
	glutDisplayFunc(DisplayScene);

	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// dołączenie funkcji obsługi klawiatury
	glutKeyboardFunc(Keyboard);

	// obsługa przycisków myszki
	glutMouseFunc(MouseButton);

	// obsługa ruchu kursora myszki
	glutMotionFunc(MouseMotion);


	// utworzenie podmenu - Filtr pomniejszający
	int MenuMinFilter = glutCreateMenu(Menu);
	glutAddMenuEntry("GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST);
	glutAddMenuEntry("GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR);
	glutAddMenuEntry("GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST);
	glutAddMenuEntry("GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR);

	// utworzenie podmenu - Aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - całe okno", FULL_WINDOW);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif

	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);

	// menu główne
	glutCreateMenu(Menu);

#ifdef WIN32

	glutAddSubMenu("Filtr pomniejszający", MenuMinFilter);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddMenuEntry("Wyjście", EXIT);
#else

	glutAddSubMenu("Filtr pomniejszajacy", MenuMinFilter);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// określenie przycisku myszki obsługującego menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// utworzenie tekstur
	GenerateTextures();

	// sprawdzenie i przygotowanie obsługi wybranych rozszerzeñ
	ExtensionSetup();

	// wprowadzenie programu do obsługi pętli komunikatów
	glutMainLoop();
	return 0;
}
