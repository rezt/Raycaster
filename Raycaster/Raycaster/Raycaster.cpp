#include <GL/glut.h>
#include <math.h>
#define PI 3.14159265358979323846
#define P2 PI/2
#define P3 3*PI/2

float px, py, pdx, pdy, pa; // Player position

void drawPlayer()
{
	glColor3f(0.8, 0.2, 0.2);
	glBegin(GL_QUADS);
	glVertex2f(px - 10, py - 10);
	glVertex2f(px + 10, py - 10);
	glVertex2f(px + 10, py + 10);
	glVertex2f(px - 10, py + 10);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(px, py);
	glVertex2f(px + pdx * 5, py + pdy * 5);
	glEnd();
}

int mapX = 8, mapY = 8, mapS = 64;
int map[] = 
{
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

void drawMap()
{
	int x, y, xo, yo;
	for (int y = 0; y < mapY; y++)
	{
		for (int x = 0; x < mapX; x++)
		{
			if (map[y * mapX + x] == 1)
			{
				glColor3f(1, 1, 1);
			}
			else
			{
				glColor3f(0.2, 0.2, 0.2);
			}
			glBegin(GL_QUADS);
			xo = x * mapS; yo = y * mapS;
			glVertex2f(xo+1, yo+1);
			glVertex2f(xo+1, yo+mapS-1);
			glVertex2f(xo+mapS-1, yo+mapS-1);
			glVertex2f(xo+mapS-1, yo+1);
			glEnd();
		}
	}
}

void drawRays3D() {
	int r, mx, mp, dof; float rx, ry, ra, xo, yo;
	ra = pa;
	for (r = 0; r < 1; r++) {
		// Horizonal Ray Casting
		dof = 0;
		float aTan = -1 / tan(ra);
		if (ra > PI) { ry = ((int)py / mapS) * mapS - 0.0001; rx = (py - ry) * aTan + px; yo = -mapS; xo = -yo * aTan; }
		else if (ra < PI) { ry = ((int)py / mapS) * mapS + mapS; rx = (py - ry) * aTan + px; yo = mapS; xo = -yo * aTan; }
		else { rx = px; ry = py; dof = 8; }
		while(dof < 8) {
			mx = (int)(rx / mapS);
			mp = (int)(ry / mapS);
			if (mx < 0 || mx >= mapX || mp < 0 || mp >= mapY) { dof = 8; }
			else if (map[mp * mapX + mx] == 1) { dof = 8; }
			else { rx += xo; ry += yo; dof++; }
		}
		glColor3f(0, 1, 0); glLineWidth(1); glBegin(GL_LINES); glVertex2i(px, py); glVertex2i(rx, ry); glEnd();
		// Vertical Ray casting
		/*dof = 0;
		float nTan = -tan(ra);
		if (ra > P2 && ra < P3) { ry = ((int)py / mapS) * mapS - 0.0001; rx = (py - ry) * aTan + px; yo = -mapS; xo = -yo * nTan; }
		else if (ra < P2 || ra > P3) { ry = ((int)py / mapS) * mapS + mapS; rx = (py - ry) * aTan + px; yo = mapS; xo = -yo * nTan; }
		else { rx = px; ry = py; dof = 8; }
		while (dof < 8) {
			mx = (int)(rx / mapS);
			mp = (int)(ry / mapS);
			if (mx < 0 || mx >= mapX || mp < 0 || mp >= mapY) { dof = 8; }
			else if (map[mp * mapX + mx] == 1) { dof = 8; }
			else { rx += xo; ry += yo; dof++; }
		}
		glColor3f(0, 1, 0); glLineWidth(1); glBegin(GL_LINES); glVertex2i(px, py); glVertex2i(rx, ry); glEnd();*/
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap();
	drawPlayer();
	drawRays3D();
	glutSwapBuffers();
}

void buttons(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': px += pdx; py+=pdy; break;
	case 's': px -= pdx; py -= pdy; break;
	case 'a': pa -= 0.1; if (pa < 0) { pa += 2*PI; } pdx = cos(pa) * 5; pdy = sin(pa) * 5; break;
	case 'd': pa += 0.1; if (pa > 2*PI) { pa -= 2*PI; } pdx = cos(pa) * 5; pdy = sin(pa) * 5; break;
	}
	glutPostRedisplay();
}

void init()
{
	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0, 1024, 512, 0);
	px = 300; py = 300; pdx = cos(pa) * 5; pdy = sin(pa) * 5;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 512);
	glutCreateWindow("Raycaster");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);
	glutMainLoop();
	return 0;
}