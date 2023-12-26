#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <stdio.h> 
#include <windows.h> 

const int HEIGHT = 600;
const int WIDTH = 600;
const double PI = 3.14;
const int GSZ = 100;

double ground[GSZ][GSZ] = { 0 };

double offset = 0;
double eyex = 1, eyey = 26, eyez = 20;
double dy = 0.1;
// locomotion
double speed = 0, angular_speed = 0;
double dirX, dirZ;
double sight_angle = PI;
// aircraft data
double air_speed = 0, air_angular_speed = 0;
double air_dirX, air_dirY, air_dirZ;
double yaw = PI, pitch = 0;
double airX = 0, airY = 20, airZ = 10;
double fill = 0, pitchEye = 0;

bool slider_is_moving = false;
double oldY;

bool stopBuilding = false;

char text[100] = "AFEKA";

// materials 
// plastic magenta
float mat1amb[4] = { 0.3,0.3,0.3,0.8 };
float mat1diff[4] = { 0.5,0.5,1,0.8 };
float mat1spec[4] = { 1,1,1,1 };

// silver
float mat2amb[4] = { 0.3,0.3,0.3,0 };
float mat2diff[4] = { 0.6,0.6,0.6,0 };
float mat2spec[4] = { 0.5,0.5,0.5,0 };

// gold
float mat3amb[4] = { 0.3,0.3,0.2,1 };
float mat3diff[4] = { 0.75,0.6,0.2,1 };
float mat3spec[4] = { 0.8,0.8,0.5,0 };

// lighting
float lt1amb[4] = { 0.3,0.3,0.3,0 };
float lt1diff[4] = { 0.7,0.7,0.7,0 };
float lt1spec[4] = { 0.8,0.8,0.8,0 };
float lt1pos[4] = { 1.5,3,0.8,0 }; // if the last component is 0 the light is DIRECTIONAL
// if the last component is 1 the light is positional

float lt2amb[4] = { 0.1,0.1,0.1,0 };
float lt2diff[4] = { 0.4,0.1,0.0,0 };
float lt2spec[4] = { 0.5,0.5,0.5,0 };
float lt2pos[4] = { -1.5,3,0.8,0 }; // if the last component is 0 the light is DIRECTIONAL


unsigned int letters;
void DrawAfeka();


void SetText()
{
	HDC hdc = wglGetCurrentDC();
	HFONT hf;
	GLYPHMETRICSFLOAT gm[128];
	LOGFONT lf;

	lf.lfHeight = 20;
	lf.lfWidth = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = true;
	lf.lfUnderline = false;
	lf.lfOrientation = 0;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = FF_MODERN;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfEscapement = 0;

	strcpy((char*)lf.lfFaceName, "Arial");

	hf = CreateFontIndirect(&lf);

	SelectObject(hdc, hf);

	letters = glGenLists(128);

	wglUseFontOutlines(hdc, 0, 128, letters,
		0, 0.3, WGL_FONT_POLYGONS, gm);
	DeleteObject(hf);
}

void DrawText(char* text)
{
	glListBase(letters);
	glCallLists(1, GL_UNSIGNED_BYTE, text);
}

void init()
{
	int i, j;

	srand(time(0));


	//  R   G    B
	glClearColor(0.7, 0.8, 1, 0);// color of window background

	glEnable(GL_DEPTH_TEST);
	SetText();

}







void DrawAxes()
{
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3d(1, 0, 0);
	glVertex3d(0, 0.1, 0);
	glVertex3d(15, 0.1, 0);

	glColor3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 8, 0);

	glColor3d(0, 0, 1);
	glVertex3d(0, 0.1, 0);
	glVertex3d(0, 0.1, 15);
	glEnd();
	glLineWidth(1);

}


void DrawCylinder(int num_pts, double r, double g, double b, double visible)
{
	double alpha, teta = 2 * PI / num_pts;

	for (alpha = 0; alpha < 2 * PI; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glColor4d(r, g, b, visible);
		glVertex3d(sin(alpha), 1, cos(alpha)); // 1
		glVertex3d(sin(alpha + teta), 1, cos(alpha + teta)); // 2
		glVertex3d(sin(alpha + teta), 0, cos(alpha + teta)); // 3
		glVertex3d(sin(alpha), 0, cos(alpha)); // 4
		glEnd();
	}
}


void DrawCylinder1(int num_pts, double topr, double bottomr, double r, double g, double b, double visible)
{
	double alpha, teta = 2 * PI / num_pts;

	for (alpha = 0; alpha < 2 * PI; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glColor4d(r , g , b,visible);
		glVertex3d(topr * sin(alpha), 1, topr * cos(alpha)); // 1
		glVertex3d(topr * sin(alpha + teta), 1, topr * cos(alpha + teta)); // 2
		glColor4d(r, g, b, visible); //0.75, 0.8, 0.9
		glVertex3d(bottomr * sin(alpha + teta), 0, bottomr * cos(alpha + teta)); // 3
		glVertex3d(bottomr * sin(alpha), 0, bottomr * cos(alpha)); // 4
		glEnd();
	}
}

void DrawCylinder2(int num_pts)
{
	double alpha, teta = 2 * PI / num_pts;

	for (alpha = 0; alpha < 2 * PI; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glColor3d(1.0, 1.0, 1.0);
		glVertex3d(sin(alpha), 1, cos(alpha)); // 1
		glVertex3d(sin(alpha + teta), 1, cos(alpha + teta)); // 2
		glVertex3d(sin(alpha + teta), 0, cos(alpha + teta)); // 3
		glVertex3d(sin(alpha), 0, cos(alpha)); // 4
		glEnd();
	}
}

void DrawCylinder3(int num_pts, double topr, double bottomr, double r, double g, double b, double visible)
{
	double alpha, teta = 2 * PI / num_pts;

	for (alpha = 0; alpha <  PI; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glColor4d(r, g, b, visible);
		glVertex3d(topr * sin(alpha), 1, topr * cos(alpha)); // 1
		glVertex3d(topr * sin(alpha + teta), 1, topr * cos(alpha + teta)); // 2
		glColor4d(r, g, b, visible); //0.75, 0.8, 0.9
		glVertex3d(bottomr * sin(alpha + teta), 0, bottomr * cos(alpha + teta)); // 3
		glVertex3d(bottomr * sin(alpha), 0, bottomr * cos(alpha)); // 4
		glEnd();
	}
}

void DrawCylinder4(int num_pts, double topr, double bottomr, double r, double g, double b, double visible)
{
	double alpha, teta = 2 * PI / num_pts;

	for (alpha = 0; alpha < 2 * PI; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glColor4d(r, g, b, visible);
		glVertex3d(topr * sin(alpha), 2, topr * cos(alpha)); // 1
		glVertex3d(topr * sin(alpha + teta), 2, topr * cos(alpha + teta)); // 2
		glColor4d(r + 0.05, g + 0.05, b + 0.03, visible);
		glVertex3d(bottomr * sin(alpha + teta), 0, bottomr * cos(alpha + teta)); // 3
		glVertex3d(bottomr * sin(alpha), 0, bottomr * cos(alpha)); // 4
		glEnd();
	}
}


void DrawCircle(double radius, int num_pts, double r, double g, double b, double visible) {
	glBegin(GL_POLYGON);
	glColor4d(r, g, b, visible); // Blue color

	for (int i = 0; i < num_pts; ++i) {
		float theta = 2 * PI * i / num_pts;
		float x = radius * cos(theta);
		float y = radius * sin(theta);
		float z = 0.0; // Keep the z-coordinate constant
		glVertex3d(x, y, z);
	}

	glEnd();
}

void DrawBottle() {

	//upper part water
	if (pitch <= -1.227889) {
		glPushMatrix();
		glTranslated(0, 3.7, 0);
		glScaled(2.7, 4.4, 2.7);
		glRotated(-30, 0, 1, 0);
		DrawCylinder3(30, 0.55, 1.25, 0.25, 0.6, 0.98, 1);
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslated(0, 3.7, 0);
		glScaled(2.7, 3.2, 2.7);
		DrawCylinder1(30, 0.7, 1.25, 0.25, 0.6, 0.98, 1);
		glPopMatrix();
	}
	
	
	//upper part 
	glPushMatrix();
	glTranslated(0, 4, 0);
	glScaled(3.2, 4.5, 3.2);
	DrawCylinder1(30, 0.5, 1.25, 0.85, 0.97, 1, 0.7);
	glPopMatrix();

	//label part
	glPushMatrix();
	glScaled(4, 4, 4);
	DrawCylinder2(30);
	glPopMatrix();
	

	// lower part water
	glPushMatrix();
	glTranslated(0, -6, 0);
	glScaled(3.5, 5.5, 3.5);
	DrawCylinder(30, 0.25, 0.6, 0.98, 1);
	glPopMatrix();

	// lower part
	glPushMatrix();
	glTranslated(0, -6, 0);
	glScaled(4, 6, 4);
	DrawCylinder(30, 0.85, 0.97, 1, 0.5);
	glPopMatrix();


	// head
	glPushMatrix();
	glTranslated(0, 8, 0);
	glScaled(1.5, 1.5, 1.5);
	DrawCylinder(30, 0.85, 0.97, 1, 0.9);
	glPopMatrix();

	// bottom
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(0, 0, 6);
	glScaled(1.5, 1.5, 1.5);
	DrawCircle(2.75, 30, 0.85, 0.97, 1, 0.7);
	glPopMatrix();

	//DrawAfeka();

}

void DrawAfeka() {
	// set text
	glPushMatrix();
	glTranslated(-0.55, 0, 0);
	//glRotated(pitch , 0, 0, 1);

	glColor4d(0.40, 0.89, 0.61, 1);


	for (int i = 0; i < strlen(text); i++)
	{
		glPushMatrix();
		glScaled(0.6, 0.6, 0.6);
		glTranslated(i - 1.0, 35, 19.9 + sin(i * 180 / PI));
		glRotated(pitch * 180 / PI, 0, 0, 1);
		DrawText(text + i);
		glPopMatrix();
	}
	glPopMatrix();
}

void DrawGlass() {

	//cilinder
	glPushMatrix();
	glTranslated(6, 14, 10);
	glScaled(1.2, 2.5, 1.2);
	DrawCylinder4(30, 2, 1.6, 0.85, 0.97, 1, 0.5);
	glPopMatrix();

	//bottom
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(6, 10, -14.3);
	glScaled(1.5, 1.5, 1.5);
	DrawCircle(1.25, 30, 0.75, 0.9, 0.95,0.5);
	glPopMatrix();


}

void DrawWater() {

	//water cilinder
	glPushMatrix();
	glTranslated(6, 14, 10);
	glScaled(1.0, 1.0 + fill, 1.0);
	DrawCylinder4(30, 2, 1.6, 0.25, 0.6, 0.98, 1);
	glPopMatrix();

	// water upper part
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(6, 10, -15.7 - fill * 2.05);
	glScaled(1.3 + fill * 0.055, 1.3 + fill * 0.055, 1.3 + fill * 0.055);
	DrawCircle(1.45, 30, 0.25, 0.6, 0.98,1);
	glPopMatrix();

}


void DrawWaterfall(int num_pts, float radius, double r, double g, double b) {

	glPushMatrix();
	glTranslated(4.5, 18.5 + pitch * 2.7, 10);
	glScaled(1.3, 3, 2);
	

	// Draw the quater circle

	glLineWidth(8);
	glBegin(GL_LINE_STRIP);
	glColor4d(r, g, b, 0.8);
	for (int i = 0; i <= num_pts; ++i) {
		float angle = static_cast<float>(i) / num_pts * PI; // Angle in radians
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		if (x >= 0) {
			glVertex3d(x, y, 0.0);
		}

	}
	glEnd();

	glPopMatrix();
}



void DrawSlider()
{
	glColor3d(1, 1, 0);
	// background
	glBegin(GL_POLYGON);
	glVertex2d(0, 0);
	glVertex2d(0, 100);
	glVertex2d(100, 100);
	glVertex2d(100, 0);
	glEnd();

	glLineWidth(3);
	glColor3d(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(50, 0);
	glVertex2d(50, 100);
	glEnd();
	glLineWidth(1);

	glMatrixMode(GL_MODELVIEW); // matrix of world
	glLoadIdentity(); // start transformations here

	glPushMatrix();
	glTranslated(0, pitch * 180 / PI, 0); // move in dgrees


	// slider button
	glColor3d(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex2d(40, 80);
	glVertex2d(40, 100);
	glVertex2d(60, 100);
	glVertex2d(60, 80);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(40, 90);
	glVertex2d(60, 90);
	glEnd();

	glPopMatrix();
}

// put all drawings hereh
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clean frame buffer and Z buffer
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION); // vision matrix
	glLoadIdentity(); // start transformations here

	glFrustum(-1, 1, -1, 1, 1, 3000);
	gluLookAt(eyex, eyey-3 , eyez+2,
		eyex + dirX, eyey - 3+ pitchEye, eyez + dirZ,
		0, 0.5, -0.5);

	//glMatrixMode(GL_MODELVIEW); // matrix of world
	//glLoadIdentity(); // start transformations here
	//DrawAxes();

	glPushMatrix();

	glTranslated(airX, airY, airZ);
	//glRotated(-yaw * 180 / PI, 0, 1, 0);

	glRotated(pitch * 180 / PI, 0, 0, 1);
	glScaled(0.5, 0.5, 0.5);
	DrawBottle();
	glPopMatrix();

	DrawWater();
	if (pitch <= -1.227889) {
		DrawWaterfall(10, 2, 0.25, 0.6, 0.98);
	}
	DrawGlass();
	
	glPushMatrix();
	//glTranslated(0, 0,0 );
	//glRotated(pitch * 180 / PI, 0, 0, 1);
	//DrawAfeka();
	glPopMatrix();
	


	// controls sub-window
	glViewport(0, 0, 100, 100);
	// Now we switch to Orthogonal projection, so we must change the projections matrix
	glMatrixMode(GL_PROJECTION); // vision matrix
	glLoadIdentity(); // start transformations here
	glOrtho(0, 100, 0, 100, -1, 1);
	glDisable(GL_DEPTH_TEST); // return to principles of graphics 2D

	DrawSlider();

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	glutSwapBuffers(); // show all
}





void idle()
{
	int i, j;
	double dist;
	offset += 0.01;

	//	if(!stopBuilding)
	//		UpdateGround3();

		// aircraft motion
	yaw += air_angular_speed;
	air_dirX = sin(yaw);
	air_dirZ = cos(yaw);
	air_dirY = sin(pitch);

	airX += air_speed * air_dirX;
	airY += air_speed * air_dirY;
	airZ += air_speed * air_dirZ;
	// player motion
	sight_angle += angular_speed;

	dirX = sin(sight_angle);
	dirZ = cos(sight_angle);

	eyex += speed * dirX;
	eyez += speed * dirZ;

	if (pitch <= -1.227889) fill += 0.00005;
	if (fill >= 1.02) fill = 0.0;
	glutPostRedisplay(); // calls indirectly display
}

void specialFun(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		speed += 0.001;
		break;
	case GLUT_KEY_DOWN:
		speed -= 0.001;
		break;
	case GLUT_KEY_LEFT:
		angular_speed += 0.0001;
		break;
	case GLUT_KEY_RIGHT:
		angular_speed -= 0.0001;
		break;
	case GLUT_KEY_PAGE_UP:
		eyey += dy;
		break;
	case GLUT_KEY_PAGE_DOWN:
		eyey -= dy;
		break;

	}
}

void mouse(int button, int state, int x, int y)
{
	double angle_in_degrees = pitch * 180 / PI + 90;
	bool clickInSlider = x > 40 && x<60 && HEIGHT - y >angle_in_degrees - 10 && HEIGHT - y < angle_in_degrees + 10;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && clickInSlider) // drag slider
	{
		slider_is_moving = true;
		oldY = HEIGHT - y;
	}
	else	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		stopBuilding = !stopBuilding;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		slider_is_moving = false;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		air_angular_speed += 0.001;
		break;
	case 'w':
		air_speed += 0.01;
		break;
	case 's':
		air_speed -= 0.01;
		break;
	case 'd':
		air_angular_speed -= 0.001;
		break;
	case 'q':
		pitchEye += 0.1;
		break;
	case 'z':
		pitchEye -= 0.1;
		break;
	}
}

void menu(int choice)
{
	switch (choice)
	{
	case 1:
		glutDisplayFunc(display);
		break;
	}
}

void mouseDrag(int x, int y)
{
	double dy = (HEIGHT - y) - oldY;

	if (slider_is_moving && HEIGHT - y < 99 && HEIGHT - y>0)
	{
		pitch += dy * PI / 180; // transform dy to radian
		oldY = (HEIGHT - y);
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 50);
	glutCreateWindow("Aircraft");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutSpecialFunc(specialFun);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseDrag);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Regular View", 1);
	


	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}