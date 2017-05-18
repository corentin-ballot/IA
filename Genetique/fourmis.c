#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ant.h"

#define TAUX_REPRODUCTION 0.8

// Produit deux fourmis a_n et b_n à partir des fourmis a et b.
void recombinaison(ant* a, ant* b, ant* a_n, ant* b_n) {
    *a_n = copyAnt(*a);
    *b_n = copyAnt(*b);

    int stop;
    node *n_a;
    n_a = a_n->prog;
    node *n_b;
    n_b = b_n->prog;
    node n_tmp;

    // COUPE DE a
    stop = 0;
    while(!stop) {
        // Random dans les n fils
        n_a = &n_a->child[rand() % (nb_child[n_a->type] + 1)];
        // couper si fils == feuille ou random == 1
        if(n_a->type == ADVANCE || n_a->type == TURNRIGHT || n_a->type == TURNLEFT || rand()%2)
            stop = 1;
    }

    // COUPE DE b
    stop = 0;
    while(!stop) {
        // Random dans les n fils
        n_b = &n_b->child[rand() % (nb_child[n_b->type] + 1)];
        // couper si fils == feuille ou random == 1
        if(n_b->type == ADVANCE || n_b->type == TURNRIGHT || n_b->type == TURNLEFT || rand()%2)
            stop = 1;
    }

    // Echange
    n_tmp = *n_a;
    *n_a = *n_b;
    *n_b = n_tmp;

    a_n->score = valuation(*a_n);
    b_n->score = valuation(*b_n);
}

void ordonnerTableau(ant tableau[], int tailleTableau) {
    int i, j;

    for ( i = 0; i < tailleTableau ; i++) {
         for (j = 1; j < (tailleTableau - i); j++) {
             if ( tableau[i].score>tableau[i+j].score) {
                 ant tmp;

                 tmp = tableau[i];
                 tableau[i] = tableau[i+j];
                 tableau[i+j] = tmp;
             }
         }
     }
}

int randomAnt(int tableau_scores[], int somme_scores,int tailleTableau) {
    int r = rand() % somme_scores, i;
    printf("%d\n", r);
    int ant = -1;

    for (i = 0; i < tailleTableau && ant == -1; i++) {
        if(r <= tableau_scores[i])
            ant = i;
    }

    return ant;
}

void nextGen(ant cur_gen[], int tailleTableau) {
    int i, j, somme_scores = 0;
    int tableau_scores[tailleTableau];
    ant *next_gen = malloc(tailleTableau * sizeof(ant));
    ordonnerTableau(cur_gen, tailleTableau);

    for (i = 0; i < tailleTableau; i++) {
        next_gen[i] = copyAnt(cur_gen[i]);
    }

    for (i = 0; i < tailleTableau; i++) {
        somme_scores += (int) cur_gen[i].score;
        tableau_scores[i] = somme_scores;
    }

    int nb_reproduction = tailleTableau * TAUX_REPRODUCTION, debut = (tailleTableau - nb_reproduction)/2;
    for (i = debut; i < nb_reproduction; i+=2) {
        int ant_1 = randomAnt(tableau_scores, somme_scores, tailleTableau);
        int ant_2 = randomAnt(tableau_scores, somme_scores, tailleTableau);
        while(ant_1 == ant_2) {
            ant_2 = randomAnt(tableau_scores, somme_scores, tailleTableau);
        }

        printf("%d, %d\n", ant_1, ant_2);

        recombinaison(&cur_gen[ant_1], &cur_gen[ant_2], &next_gen[i], &next_gen[i+1]);
    }

    *cur_gen = *next_gen;
}

int main(int argc, char const *argv[]) {
    srand(time(NULL));   //init random

    int nb_ant = 10, i;
    ant echantillon[nb_ant];

    for (i = 0; i < nb_ant; i++) {
        echantillon[i] = generateAnt();
    }

    ordonnerTableau(echantillon, nb_ant);
    for (i = 0; i < nb_ant; i++) {
        affiche(*echantillon[i].prog);
        printf(" => %f\n", echantillon[i].score);
    }

    printf("\n\n");
    nextGen(echantillon, nb_ant);

    for (i = 0; i < nb_ant; i++) {
        affiche(*echantillon[i].prog);
        printf(" => %f\n", echantillon[i].score);
    }

    return 0;
}
