// gcc perceptron.c -lglut -lGL -lGLU -o perceptron
#include <stdio.h>
#include <stdlib.h>


#define SEUIL 0
#define PAS_APPRENTISSAGE 1
#define NB_NERONNE 8
#define NB_RUNITS 4


double d[NB_RUNITS][10] = {
	{0,1,0,1,0,1,0,1,0,1}, // Bit 1
	{0,0,1,1,0,0,1,1,0,0}, // Bit 2
	{0,0,0,0,1,1,1,1,0,0}, // Bit 3
	{0,0,0,0,0,0,0,0,1,1}  // Bit 4
};
double poids[NB_RUNITS][NB_NERONNE] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};
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

double RUnit(int id, double *afficheur7segments) {
	double somme = 0;
	int i = 0;

	for (i = 0; i < NB_NERONNE; i++) {
		somme += AUnit(i, afficheur7segments) * poids[id][i];
	}

	return fonctionActivation(somme);
}

void apprentissage() {
	int i, j, k, modification;
	do {
		modification = 0;
		for (i = 0; i < 10; i++) {
			double dj[NB_RUNITS], sj[NB_RUNITS], si;

			for (j = 0; j < NB_RUNITS; j++) {
				dj[j] = d[j][i];
				sj[j] = RUnit(j, X[i]);

				if(d[j][i] != sj[j]){
					for (k = 0; k < NB_NERONNE; k++) {
						si = AUnit(k, X[i]);

						poids[j][k] += PAS_APPRENTISSAGE * (dj[j] - sj[j]) * si;
						printf("%f, ", poids[j][k]);
					}
					printf("\n");
					modification++;
				}
			}
		}
	}while(modification != 0);
}

int poidsCorrect() {
	int i, j, result = 1;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < NB_RUNITS; j++) {
			if(RUnit(j, X[i]) != d[j][i])
				result = 0;
		}
	}
	return result;
}

char* estValeur(double *afficheur7segments) {
	char* result = malloc(sizeof(char) * NB_RUNITS);
	// L'implémentation à été faire de "gauche à droite", on décrémente donc au
	// lieu d'incrémenter pour récupérer une lecture du nombre de correct
	result[0] = '0'+RUnit(3, afficheur7segments);
	result[1] = '0'+RUnit(2, afficheur7segments);
	result[2] = '0'+RUnit(1, afficheur7segments);
	result[3] = '0'+RUnit(0, afficheur7segments);
	return result;
}

int main(int argc, char *argv[]) {
	apprentissage();
	int i;
	for (i = 0; i < 10; i++) {
		printf("%d à pour valeur binaire : %s\n", i, estValeur(X[i]));
	}

	return 0;
}
