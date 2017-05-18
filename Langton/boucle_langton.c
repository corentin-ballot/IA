#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>

// Images par secondes.
#define FPS 20

// Nombre de cellules en X et Y.
#define SIZE 80

// Taille de la fen�tre.
int width = 900;
int height = 900;

// Le tableau o� simuler le jeu.
int c[SIZE][SIZE];
int t[SIZE][SIZE];
int (*cellules)[SIZE] = c;
int (*cellules_next)[SIZE] = t;

// Table de règles
int rules[219][6] = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 2}, {0, 0, 0, 0, 2, 0}, {0, 0, 0, 0, 3, 0}, {0, 0, 0, 0, 5, 0}, {0, 0, 0, 0, 6, 3}, {0, 0, 0, 0, 7, 1}, {0, 0, 0, 1, 1, 2}, {0, 0, 0, 1, 2, 2}, {0, 0, 0, 1, 3, 2}, {0, 0, 0, 2, 1, 2}, {0, 0, 0, 2, 2, 0}, {0, 0, 0, 2, 3, 0}, {0, 0, 0, 2, 6, 2}, {0, 0, 0, 2, 7, 2}, {0, 0, 0, 3, 2, 0}, {0, 0, 0, 5, 2, 5}, {0, 0, 0, 6, 2, 2}, {0, 0, 0, 7, 2, 2}, {0, 0, 1, 0, 2, 2}, {0, 0, 1, 1, 2, 0}, {0, 0, 2, 0, 2, 0}, {0, 0, 2, 0, 3, 0}, {0, 0, 2, 0, 5, 0}, {0, 0, 2, 1, 2, 5}, {0, 0, 2, 2, 2, 0}, {0, 0, 2, 3, 2, 2}, {0, 0, 5, 2, 2, 2}, {0, 1, 2, 3, 2, 1}, {0, 1, 2, 4, 2, 1}, {0, 1, 2, 5, 2, 5}, {0, 1, 2, 6, 2, 1}, {0, 1, 2, 7, 2, 1}, {0, 1, 2, 7, 5, 1}, {0, 1, 4, 2, 2, 1}, {0, 1, 4, 3, 2, 1}, {0, 1, 4, 4, 2, 1}, {0, 1, 4, 7, 2, 1}, {0, 1, 6, 2, 5, 1}, {0, 1, 7, 2, 2, 1}, {0, 1, 7, 2, 5, 5}, {0, 1, 7, 5, 2, 1}, {0, 1, 7, 6, 2, 1}, {0, 1, 7, 7, 2, 1}, {0, 2, 5, 2, 7, 1}, {1, 0, 0, 0, 1, 1}, {1, 0, 0, 0, 6, 1}, {1, 0, 0, 0, 7, 7}, {1, 0, 0, 1, 1, 1}, {1, 0, 0, 1, 2, 1}, {1, 0, 0, 2, 1, 1}, {1, 0, 0, 2, 4, 4}, {1, 0, 0, 2, 7, 7}, {1, 0, 0, 5, 1, 1}, {1, 0, 1, 0, 1, 1}, {1, 0, 1, 1, 1, 1}, {1, 0, 1, 2, 4, 4}, {1, 0, 1, 2, 7, 7}, {1, 0, 2, 0, 2, 6}, {1, 0, 2, 1, 2, 1}, {1, 0, 2, 2, 1, 1}, {1, 0, 2, 2, 4, 4}, {1, 0, 2, 2, 6, 3}, {1, 0, 2, 2, 7, 7}, {1, 0, 2, 3, 2, 7}, {1, 0, 2, 4, 2, 4}, {1, 0, 2, 6, 2, 6}, {1, 0, 2, 6, 4, 4}, {1, 0, 2, 6, 7, 7}, {1, 0, 2, 7, 1, 0}, {1, 0, 2, 7, 2, 7}, {1, 0, 5, 4, 2, 7}, {1, 1, 1, 1, 2, 1}, {1, 1, 1, 2, 2, 1}, {1, 1, 1, 2, 4, 4}, {1, 1, 1, 2, 5, 1}, {1, 1, 1, 2, 6, 1}, {1, 1, 1, 2, 7, 7}, {1, 1, 1, 5, 2, 2}, {1, 1, 2, 1, 2, 1}, {1, 1, 2, 2, 2, 1}, {1, 1, 2, 2, 4, 4}, {1, 1, 2, 2, 5, 1}, {1, 1, 2, 2, 7, 7}, {1, 1, 2, 3, 2, 1}, {1, 1, 2, 4, 2, 4}, {1, 1, 2, 6, 2, 1}, {1, 1, 2, 7, 2, 7}, {1, 1, 3, 2, 2, 1}, {1, 2, 2, 2, 4, 4}, {1, 2, 2, 2, 7, 7}, {1, 2, 2, 4, 3, 4}, {1, 2, 2, 5, 4, 7}, {1, 2, 3, 2, 4, 4}, {1, 2, 3, 2, 7, 7}, {1, 2, 4, 2, 5, 5}, {1, 2, 4, 2, 6, 7}, {1, 2, 5, 2, 7, 5}, {2, 0, 0, 0, 1, 2}, {2, 0, 0, 0, 2, 2}, {2, 0, 0, 0, 4, 2}, {2, 0, 0, 0, 7, 1}, {2, 0, 0, 1, 2, 2}, {2, 0, 0, 1, 5, 2}, {2, 0, 0, 2, 1, 2}, {2, 0, 0, 2, 2, 2}, {2, 0, 0, 2, 3, 2}, {2, 0, 0, 2, 4, 2}, {2, 0, 0, 2, 5, 0}, {2, 0, 0, 2, 6, 2}, {2, 0, 0, 2, 7, 2}, {2, 0, 0, 3, 2, 6}, {2, 0, 0, 4, 2, 3}, {2, 0, 0, 5, 1, 7}, {2, 0, 0, 5, 2, 2}, {2, 0, 0, 5, 7, 5}, {2, 0, 0, 7, 2, 2}, {2, 0, 1, 0, 2, 2}, {2, 0, 1, 1, 2, 2}, {2, 0, 1, 2, 2, 2}, {2, 0, 1, 4, 2, 2}, {2, 0, 1, 7, 2, 2}, {2, 0, 2, 0, 2, 2}, {2, 0, 2, 0, 3, 2}, {2, 0, 2, 0, 5, 2}, {2, 0, 2, 0, 7, 3}, {2, 0, 2, 1, 2, 2}, {2, 0, 2, 1, 5, 2}, {2, 0, 2, 2, 1, 2}, {2, 0, 2, 2, 2, 2}, {2, 0, 2, 2, 7, 2}, {2, 0, 2, 3, 2, 1}, {2, 0, 2, 4, 2, 2}, {2, 0, 2, 4, 5, 2}, {2, 0, 2, 5, 2, 0}, {2, 0, 2, 5, 5, 2}, {2, 0, 2, 6, 2, 2}, {2, 0, 2, 7, 2, 2}, {2, 0, 3, 1, 2, 2}, {2, 0, 3, 2, 1, 6}, {2, 0, 3, 2, 2, 6}, {2, 0, 3, 4, 2, 2}, {2, 0, 4, 2, 2, 2}, {2, 0, 5, 1, 2, 2}, {2, 0, 5, 2, 1, 2}, {2, 0, 5, 2, 2, 2}, {2, 0, 5, 5, 2, 1}, {2, 0, 5, 7, 2, 5}, {2, 0, 6, 2, 2, 2}, {2, 0, 6, 7, 2, 2}, {2, 0, 7, 1, 2, 2}, {2, 0, 7, 2, 2, 2}, {2, 0, 7, 4, 2, 2}, {2, 0, 7, 7, 2, 2}, {2, 1, 1, 2, 2, 2}, {2, 1, 1, 2, 6, 1}, {2, 1, 2, 2, 2, 2}, {2, 1, 2, 2, 4, 2}, {2, 1, 2, 2, 6, 2}, {2, 1, 2, 2, 7, 2}, {2, 1, 4, 2, 2, 2}, {2, 1, 5, 2, 2, 2}, {2, 1, 6, 2, 2, 2}, {2, 1, 7, 2, 2, 2}, {2, 2, 2, 2, 7, 2}, {2, 2, 2, 4, 4, 2}, {2, 2, 2, 4, 6, 2}, {2, 2, 2, 7, 6, 2}, {2, 2, 2, 7, 7, 2}, {3, 0, 0, 0, 1, 3}, {3, 0, 0, 0, 2, 2}, {3, 0, 0, 0, 4, 1}, {3, 0, 0, 0, 7, 6}, {3, 0, 0, 1, 2, 3}, {3, 0, 0, 4, 2, 1}, {3, 0, 0, 6, 2, 2}, {3, 0, 1, 0, 2, 1}, {3, 0, 1, 2, 2, 0}, {3, 0, 2, 5, 1, 1}, {4, 0, 1, 1, 2, 0}, {4, 0, 1, 2, 2, 0}, {4, 0, 1, 2, 5, 0}, {4, 0, 2, 1, 2, 0}, {4, 0, 2, 2, 2, 1}, {4, 0, 2, 3, 2, 6}, {4, 0, 2, 5, 2, 0}, {4, 0, 3, 2, 2, 1}, {5, 0, 0, 0, 2, 2}, {5, 0, 0, 2, 1, 5}, {5, 0, 0, 2, 2, 5}, {5, 0, 0, 2, 3, 2}, {5, 0, 0, 2, 7, 2}, {5, 0, 0, 5, 2, 0}, {5, 0, 2, 0, 2, 2}, {5, 0, 2, 1, 2, 2}, {5, 0, 2, 1, 5, 2}, {5, 0, 2, 2, 2, 0}, {5, 0, 2, 2, 4, 4}, {5, 0, 2, 7, 2, 2}, {5, 1, 2, 1, 2, 2}, {5, 1, 2, 2, 2, 0}, {5, 1, 2, 4, 2, 2}, {5, 1, 2, 7, 2, 2}, {6, 0, 0, 0, 1, 1}, {6, 0, 0, 0, 2, 1}, {6, 0, 2, 1, 2, 0}, {6, 1, 2, 1, 2, 5}, {6, 1, 2, 1, 3, 1}, {6, 1, 2, 2, 2, 5}, {7, 0, 0, 0, 7, 7}, {7, 0, 1, 1, 2, 0}, {7, 0, 1, 2, 2, 0}, {7, 0, 1, 2, 5, 0}, {7, 0, 2, 1, 2, 0}, {7, 0, 2, 2, 2, 1}, {7, 0, 2, 2, 5, 1}, {7, 0, 2, 3, 2, 1}, {7, 0, 2, 5, 2, 5}, {7, 0, 2, 7, 2, 0}};

// Si != 0 la simulation s'ex�cute.
int running = 0;

// Initialise le tableau de cellules.
void initCellules() {
	for(int y=0; y<SIZE; y++) {
		for(int x=0; x<SIZE; x++) {
			cellules[x][y] = 0;
		}
	}

	int x = 20, y = 20;

	// Structure initiale
	for (int i = 1; i <= 8; i++) {
		cellules[x][y+i] = 2; // bordure exterieur haute
		cellules[x+9][y+i] = 2; // bordure exterieur basse
		cellules[x+i][y] = 2; // bordure exterieur gauche
		cellules[x+i][y+9] = 2; // bordure exterieur droite
	}

	for (int i = 0; i < 6; i++) {
		cellules[x+2][y+2+i] = 2; // bordure interieur haute
		cellules[x+7][y+2+i] = 2; // bordure interieur basse
		cellules[x+2+i][y+2] = 2; // bordure interieur gauche
		cellules[x+2+i][y+7] = 2; // bordure interieur droite
	}

	// Queue partie haute
	for (int i = 0; i < 4; i++) {
		cellules[x+10+i][y+2] = 2; // bordure interieur haute
	}

	// Queue partie basse
	for (int i = 0; i < 6; i++) {
		cellules[x+8+i][y] = 2; // bordure extereur basse
	}

	// Interieur bas
	cellules[x+1][y+1] = 0;
	cellules[x+2][y+1] = 7;
	cellules[x+3][y+1] = 1;
	cellules[x+4][y+1] = 0;
	cellules[x+5][y+1] = 7;
	cellules[x+6][y+1] = 1;
	cellules[x+7][y+1] = 0;
	cellules[x+8][y+1] = 7;
	cellules[x+9][y+1] = 1;
	cellules[x+10][y+1] = 1;
	cellules[x+11][y+1] = 1;
	cellules[x+12][y+1] = 1;
	cellules[x+13][y+1] = 1;
	// Interieur gauche
	cellules[x+1][y+2] = 1;
	cellules[x+1][y+3] = 7;
	cellules[x+1][y+4] = 0;
	cellules[x+1][y+5] = 1;
	cellules[x+1][y+6] = 7;
	cellules[x+1][y+7] = 0;
	cellules[x+1][y+8] = 1;
	// Interieur haut
	cellules[x+2][y+8] = 7;
	cellules[x+3][y+8] = 0;
	cellules[x+4][y+8] = 1;
	cellules[x+5][y+8] = 4;
	cellules[x+6][y+8] = 0;
	cellules[x+7][y+8] = 1;
	cellules[x+8][y+8] = 4;
	// Interieur droit
	cellules[x+8][y+2] = 1;
	cellules[x+8][y+3] = 1;
	cellules[x+8][y+4] = 1;
	cellules[x+8][y+5] = 1;
	cellules[x+8][y+6] = 1;
	cellules[x+8][y+7] = 0;
	cellules[x+8][y+8] = 4;

	//cellules[0][0] = 2;
}

int next_state(int cur_state[]) {
	int i;
	for (i = 0; i < 218; i++) {
		if((cur_state[0] == rules[i][0] && cur_state[1] == rules[i][1] && cur_state[2] == rules[i][2] && cur_state[3] == rules[i][3] && cur_state[4] == rules[i][4])
			|| (cur_state[0] == rules[i][0] && cur_state[2] == rules[i][1] && cur_state[3] == rules[i][2] && cur_state[4] == rules[i][3] && cur_state[1] == rules[i][4])
			|| (cur_state[0] == rules[i][0] && cur_state[3] == rules[i][1] && cur_state[4] == rules[i][2] && cur_state[1] == rules[i][3] && cur_state[2] == rules[i][4])
			|| (cur_state[0] == rules[i][0] && cur_state[4] == rules[i][1] && cur_state[1] == rules[i][2] && cur_state[2] == rules[i][3] && cur_state[3] == rules[i][4]))
			return rules[i][5];
	}
	return 0;
}

void calcul() {
	if(running != 0) {
		//printf("\ncalcul\n");
		int i, j;
		//CTRBL -> C
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				int rule[5];

				rule[0] = cellules[i][j];
				rule[1] = cellules[i][(j + SIZE + 1) % SIZE]; // Nord
				rule[2] = cellules[(i + SIZE + 1) % SIZE][j]; // Est
				rule[3] = cellules[i][(j + SIZE - 1) % SIZE]; // Sud
				rule[4] = cellules[(i + SIZE - 1) % SIZE][j]; // Ouest

				//printf("{ %d, %d, %d, %d, %d } -> %d\n", rule[0], rule[1], rule[2], rule[3], rule[4], next_state(rule));

				cellules_next[i][j] = next_state(rule);
			}
			//printf("\n");
		}

		int (*temp)[SIZE] = cellules;
		cellules = cellules_next;
		cellules_next = temp;
	}
}

// ----------------------------------------------------------------------
// Code graphique
// ----------------------------------------------------------------------

int grille;
int carre;

// V�rifie l'�tat de la machine OpenGL.
void checkError() {
	int err = glGetError();
	if(err != 0) printf("Error %s\n", gluErrorString(err));
}

// Cr�e une grille de lignes.
int creeGrille() {
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	float incr = 2.0 / SIZE;
	float x = -1;
	glBegin(GL_LINES);
	//glColor3f(0.3, 0.3, 0.3);
	for(int i=0; i<SIZE-1; i++) {
		x += incr;
		glVertex2f(x, -1);
		glVertex2f(x,  1);
		glVertex2f(-1, x);
		glVertex2f( 1, x);
	}
	glEnd();
	glEndList();
	return list;
}

// Cr�e une cellule.
int creeCarre() {
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	float w = 1.0 / SIZE;
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(-w, -w);
		glVertex2f( w, -w);
		glVertex2f( w,  w);
		glVertex2f(-w,  w);
	glEnd();
	glEndList();
	return list;
}

// Affiche l'ensemble des cellules.
void afficheCarres() {
	float delta = 2.0 / SIZE;
	glPushMatrix();
	glTranslatef(-1 + delta/2, -1 + delta/2, 0);
	glColor3f(1, 1, 1);
	for(int y=0; y<SIZE; y++) {
		for(int x=0; x<SIZE; x++) {
			switch (cellules[x][y]) {
				case 1 :
					glColor3f(0, 0, 1);
					break;
				case 2 :
					glColor3f(1, 0, 0);
					break;
				case 3 :
					glColor3f(0, 1, 0);
					break;
				case 4 :
					glColor3f(1, 1, 0);
					break;
				case 5 :
					glColor3f(1, 0, 1);
					break;
				case 6 :
					glColor3f(1, 1, 1);
					break;
				case 7 :
					glColor3f(0, 1, 1);
					break;
				default:
					glColor3f(0, 0, 0);
					break;
			}

			if(cellules[x][y] != 0) {
				glPushMatrix();
				glTranslatef(x*delta, y*delta, 0);
				glCallList(carre);
				glPopMatrix();
			}
		}
	}
	glPopMatrix();
}

// Proc�dure g�n�rale d'affichage.
void affiche() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	afficheCarres();
	glCallList(grille);
	checkError();
	glutSwapBuffers();
}

// Animation.
void animate(int id) {
	calcul();
	glutPostRedisplay();
	glutTimerFunc(1000/FPS, &animate, 0);
}

void key(unsigned char key, int x, int y) {
	switch(key) {
		case ' ':
			running = (running + 1) % 2;
			break;
		default:
			break;
	}
}

void mouse(int button, int state, int x, int y) {
	if(state == GLUT_DOWN) {
		int xx = (int) (((x-1)/(float)width) * SIZE);
		int yy = (SIZE-1) - (int) (((y-1)/(float)height) * SIZE);
		cellules[xx][yy] = (cellules[xx][yy] + 1) % 8;
	}
}

void reshape(int w, int h) {
	width = w;
	height = h;
}

int main(int argc, char *argv[]) {

	//int test[] = {6, 0, 0, 0, 1};
	//printf("%d%d%d%d%d:%d\n", test[0], test[1], test[2], test[3], test[4], next_state(test));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Boucle de Langton");
	glutDisplayFunc(&affiche);
	initCellules();
	grille = creeGrille();
	carre = creeCarre();
	glutKeyboardFunc(&key);
	glutMouseFunc(&mouse);
	glutTimerFunc(1000/FPS, &animate, 0);
	glutReshapeFunc(&reshape);
	glutMainLoop();
	return 0;
}
