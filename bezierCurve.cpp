/*
author 	  : Aadith
reference : https://www.youtube.com/watch?v=dXECQRlmIaE
compile   :	g++ -std=c++14 filename.cpp -lGL -lGLU -lglut
*/

#include <bits/stdc++.h>
#include <GL/glut.h>
#include <unistd.h>

using namespace std;

int stp = 10;
double X0, Y0, X1, Y1, X2, Y2, itr = 0, incVal = 0.2;

// clears screen with black color
void clrscr() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void init() {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
	clrscr();
	glMatrixMode(GL_MODELVIEW);
}

// divides the line joining points (_X1, _Y1) and (_X2, _Y2) into stp (global varialbe) parts
// and returns the starting point of dLen'th part
pair<double, double> fP(double _X1, double _Y1, double _X2, double _Y2, int dLen) {
	double m = (_Y2 - _Y1) / (_X2 - _X1);
	double c = _Y1 - m * _X1;
	double seg = abs(_X2 - _X1) / stp;
	double xres = min(_X1, _X2) + seg * dLen;
	double yres = m * xres + c;
	return {xres, yres};
}

void solve() {
	pair<double, double> pcur, pprev;
	vector<array<double, 2>> pts1(stp + 1), pts2(stp + 1);
	for (int i = 0; i <= stp; i++) {
		pcur = fP(X0, Y0, X1, Y1, i);
		pts1[i][0] = pcur.first;
		pts1[i][1] = pcur.second;

		pcur = fP(X1, Y1, X2, Y2, i);
		pts2[i][0] = pcur.first;
		pts2[i][1] = pcur.second;
	}

	for (int i = 0; i <= stp; i++) {
		pcur = fP(pts1[i][0], pts1[i][1], pts2[i][0], pts2[i][1], i);
		if (i != 0) {
			for (int thick = 0; thick < 210; thick++) {
				if (thick < 70)	glColor3f(1, 0.5, 0);		// orange
				else if (thick < 140)	glColor3f(1, 1, 1);	// white
				else	glColor3f(0, 1, 0);					// green
				glBegin(GL_LINES);
					glVertex2d(pprev.first, pprev.second - thick);
					glVertex2d(pcur.first, pcur.second - thick);
				glEnd();
			}
			glFlush();
		}
		pprev = pcur;
	}
}

// draws brown color stick
void drawStick() {
	glColor3f(0.8, 0.4, 0.2);
	glBegin(GL_POLYGON);
		glVertex2i(150, 345);
		glVertex2i(160, 345);
		glVertex2i(160, 60);
		glVertex2i(150, 60);
	glEnd();
	glFlush();
}

// Animate function
void work() {
	clrscr();
	drawStick();
	X0 = 160, Y0 = 340, X1 = 210 + (itr * 3.0) / 4, Y1 = 390 - (itr * 2), X2 = 260 + (itr * 3.0) / 2, Y2 = 340;
	solve();
	X0 = 260 + (itr * 3.0) / 2, Y0 = 340, X1 = 310 + 3 * (itr * 3.0) / 4.0, Y1 = 290 + (itr * 2), X2 = 360 + (itr * 3.0), Y2 = 340;
	solve();
	usleep(20000);
	itr += incVal;
	if (itr > 15)		incVal = -0.1;
	else if (itr < 0)	incVal = 0.1;
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("bezier");
	init();
	glutDisplayFunc(work);
	glutMainLoop();
}
