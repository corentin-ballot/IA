// gcc perceptron.c -lglut -lGL -lGLU -o perceptron
#include <stdio.h>


#define SEUIL 0
#define PAS_APPRENTISSAGE 1
#define NB_NERONNE 8


double d[10] = {0,1,0,1,0,1,0,1,0,1};
double poids[NB_NERONNE] = {0,0,0,0,0,0,0,0};
double X[10][NB_NERONNE] = {{1,1,1,1,1,1,0,-1},	// 0
			{0,1,1,0,0,0,0,-1},	// 1
			{1,1,0,1,1,0,1,-1},	// 2
			{1,1,1,1,0,0,1,-1},	// 3
			{0,1,1,0,0,1,1,-1},	// 4
			{1,0,1,1,0,1,1,-1},	// 5
			{1,0,1,1,1,1,1,-1},	// 6
			{1,1,1,0,0,0,0,-1},	// 7
			{1,1,1,1,1,1,1,-1},	// 8
			{1,1,1,1,0,1,1,-1}};	// 9

double AUnit(int id, double *afficheur7segments) {
	return afficheur7segments[id];
}

double fonctionActivation(double somme) {
	return (somme >= SEUIL) ? 1 : 0;
}

double RUnit(double *afficheur7segments) {
	double somme = 0;
	int i = 0;

	for (i = 0; i < NB_NERONNE; i++) {
		somme += AUnit(i, afficheur7segments) * poids[i];
	}

	return fonctionActivation(somme);
}

void apprentissage() {
	int i, j, k=0, modification;
	do {
		modification = 0;
		for (i = 0; i < 10; i++) {
			double dj, sj, si;
			dj = d[i];
			sj = RUnit(X[i]);
			if(d[i] != sj){
				for (j = 0; j < NB_NERONNE; j++) {
					si = AUnit(j, X[i]);

					poids[j] += PAS_APPRENTISSAGE * (dj - sj) * si;
					printf("%f, ", poids[j]);
				}
				printf("\n");
				modification++;
			}
		}
	}while(modification != 0);
}

int poidsCorrect() {
	int i, result = 1;
	for (i = 0; i < 10; i++) {
		if(RUnit(X[i]) != d[i])
			result = 0;
	}
	return result;
}

double estPair(double *afficheur7segments) {
	return RUnit(afficheur7segments);
}

int main(int argc, char *argv[]) {
	apprentissage();
	int i;
	for (i = 0; i < 10; i++) {
		printf("%d est pas pair : %f\n", i, estPair(X[i]));
	}

	return 0;
}
