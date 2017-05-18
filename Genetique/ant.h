#define PROFONDEUR_MAX 1

#define IFSENSOR 0
#define PROG2 1
#define PROG3 2
#define PROG4 3
#define ADVANCE 4
#define TURNLEFT 5
#define TURNRIGHT 6

int nb_child[] = {2, 2, 3, 4, 0, 0, 0};

#define GRID_SIZE 10

#define NORTH 10
#define EAST 15
#define SOUTH 20
#define WEST 25

#define MAX_STEP 200
#define NB_CHOCOBON 15.0


int start[2] = {0, 9};
int start_direction = SOUTH;
int grid_init[GRID_SIZE][GRID_SIZE] = {
    {1,1,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,0,0,1,0},
    {0,0,1,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};

double chocobon_get;
int pos[2];
int dir;
int grid[GRID_SIZE][GRID_SIZE];
int nb_actions;

typedef struct node {
    int type;
    struct node *child;
} node;

struct ant {
    node *prog;
    double score;
};
typedef struct ant ant;

void affiche(node ant) {
    printf("(");
    switch (ant.type) {
        case IFSENSOR:  printf("IFSENSOR "); break;
        case PROG2:     printf("PROG2 "); break;
        case PROG3:     printf("PROG3 "); break;
        case PROG4:     printf("PROG4 "); break;
        case ADVANCE:   printf("ADVANCE"); break;
        case TURNLEFT:  printf("TURNLEFT"); break;
        case TURNRIGHT: printf("TURNRIGHT"); break;
    }

    int i;
    for (i = 0; i < nb_child[ant.type]; i++) {
        affiche(ant.child[i]);
    }

    printf(")");
}

void execute(node prog){
    switch (prog.type) {
        case IFSENSOR:
            if(dir == NORTH)
                if(grid[(pos[0] + GRID_SIZE - 1) % GRID_SIZE][pos[1]] == 1) execute(prog.child[0]);
                else execute(prog.child[1]);
            else if(dir == EAST)
                if(grid[pos[0]][(pos[1] + 1) % GRID_SIZE] == 1) execute(prog.child[0]);
                else execute(prog.child[1]);
            else if(dir == SOUTH)
                if(grid[(pos[0] + 1) % GRID_SIZE][pos[1]] == 1) execute(prog.child[0]);
                else execute(prog.child[1]);
            else if(dir == WEST)
                if(grid[pos[0]][(pos[1] + GRID_SIZE - 1) % GRID_SIZE] == 1) execute(prog.child[0]);
                else execute(prog.child[1]);
            break;
        case PROG2:
            execute(prog.child[0]);
            execute(prog.child[1]);
            break;
        case PROG3:
            execute(prog.child[0]);
            execute(prog.child[1]);
            execute(prog.child[2]);
            break;
        case PROG4:
            execute(prog.child[0]);
            execute(prog.child[1]);
            execute(prog.child[2]);
            execute(prog.child[3]);
            break;
        case ADVANCE:
            if(dir == NORTH)
                pos[0] = (pos[0] + GRID_SIZE - 1) % GRID_SIZE;
            else if(dir == EAST)
                pos[1] = (pos[1] + 1) % GRID_SIZE;
            else if(dir == SOUTH)
                pos[0] = (pos[0] + 1) % GRID_SIZE;
            else if(dir == WEST)
                pos[1] = (pos[1] + GRID_SIZE - 1) % GRID_SIZE;

            nb_actions += 1;
            if(grid[pos[0]][pos[1]] == 1) { // CHOCOBON!
                grid[pos[0]][pos[1]] = 0;
                chocobon_get ++;
            }
            break;
        case TURNLEFT:
            if(dir == NORTH)
                dir = WEST;
            else if(dir == EAST)
                dir = NORTH;
            else if(dir == SOUTH)
                dir = EAST;
            else if(dir == WEST)
                dir = SOUTH;
            nb_actions += 1;
            break;
        case TURNRIGHT:
            if(dir == NORTH)
                dir = EAST;
            else if(dir == EAST)
                dir = SOUTH;
            else if(dir == SOUTH)
                dir = WEST;
            else if(dir == WEST)
                dir = NORTH;
            nb_actions += 1;
            break;
    }
}

double valuation(ant a) {
    int i,j;
    pos[0] = start[0]; pos[1] = start[1];
    dir = start_direction;
    nb_actions = 0;
    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = grid_init[i][j];
        }
    }
    chocobon_get = 0;

    for (i = 0; i < MAX_STEP && chocobon_get < NB_CHOCOBON; i++) {
        execute(*a.prog);
    }

    /*affiche(*a.prog);
    printf("\n");
    printf("chocobons : %f\n", chocobon_get);
    printf("actions : %d\n", nb_actions);
    printf("%f\n", nb_actions/(chocobon_get+1));
    printf("\n");*/
    return nb_actions/(chocobon_get+1);
}

node generateNode(int deep) {
    node current;
    if(deep == 1) { // Profondeur max atteinte, on ajoute un terminal
        current.type = rand() % 3 +4;
    } else {
        int i;
        current.type = rand() % 7;

        if(nb_child[current.type] > 0) {
            current.child = malloc(nb_child[current.type] * sizeof(node));
            for (i = 0; i < nb_child[current.type]; i++) {
                current.child[i] = generateNode(deep-1);
            }
        }
        else
            current.child = NULL;
    }

    return current;
}

ant generateAnt() {
    int i;

    ant superant;
    node *root= malloc(sizeof(node));

    root->type = rand() % 4;
    root->child = malloc(nb_child[root->type] * sizeof(node));

    // recursivité
    for (i = 0; i < nb_child[root->type]; i++) {
        root->child[i] = generateNode(PROFONDEUR_MAX);
    }

    superant.prog = root;
    superant.score = valuation(superant);
    return superant;
}

node copyNode(node n) {
    node copy;
    int i;
    copy.type = n.type;

    if(nb_child[copy.type] > 0) {
        copy.child = malloc(nb_child[copy.type] * sizeof(node));
        for (i = 0; i < nb_child[copy.type]; i++) {
            copy.child[i] = copyNode(n.child[i]);
        }
    } else {
        copy.child = NULL;
    }

    return copy;
}

ant copyAnt(ant a) {
    ant copy;
    int i;
    node *root= malloc(sizeof(node));

    root->type = a.prog->type;
    root->child = malloc(nb_child[root->type] * sizeof(node));

    // recursivité
    for (i = 0; i < nb_child[root->type]; i++) {
        root->child[i] = copyNode(a.prog->child[i]);
    }

    copy.prog = root;
    //copy.score = valuation(superant);
    copy.score = rand() % 500;
    return copy;
}


/*(PROG3 (PROG3 (PROG4 (ADVANCE)(ADVANCE)(ADVANCE)(TURNLEFT))(IFSENSOR (ADVANCE)(TURNLEFT))(PROG3 (TURNLEFT)(TURNLEFT)(TURNLEFT)))(TURNLEFT)(PROG3 (ADVANCE)(ADVANCE)(PROG4 (TURNLEFT)(ADVANCE)(TURNLEFT)(ADVANCE))))
chocobons : 15.000000
actoins : 108
108.000000*/
