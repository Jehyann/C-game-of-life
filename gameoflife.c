#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constantes
#define WIDTH 200
#define HEIGHT 30

// Fonctions
void init_grid(int grid[WIDTH][HEIGHT]);
void print_grid(int grid[WIDTH][HEIGHT]);
void update_grid(int grid[WIDTH][HEIGHT]);
int count_neighbors(int grid[WIDTH][HEIGHT], int x, int y);
void manual_mode(int grid[WIDTH][HEIGHT]);
void random_mode(int grid[WIDTH][HEIGHT]);
void file_mode(int grid[WIDTH][HEIGHT]);
void save_grid(int grid[WIDTH][HEIGHT], char filename[]);

int main() {
    int grid[WIDTH][HEIGHT];
    int mode, generations;
    char choice = 'y';
    char save = 'y';
    char filename[100] = "save.txt";

    // Pour le random
    srand(time(NULL));

    while (choice == 'y') {
        // Initialise la grille
        init_grid(grid);

        // Affiche le menu et permet la saisie utilisateur
        printf("Select a mode:\n");
        printf("1. Manual\n");
        printf("2. Random\n");
        printf("3. Load file\n");
        printf("Enter the mode number: ");
        scanf("%d", &mode);

        if (mode !=3) {
            // Demande à l'utilisateur le nombre de génération qu'il veut
            printf("Enter the number of generations: ");
            scanf("%d", &generations);
        }

        // Choisi le mode
        switch (mode) {
            case 1:
                manual_mode(grid);
                break;
            case 2:
                random_mode(grid);
                break;
            case 3:
                file_mode(grid);
                break;
            default:
                printf("Invalid mode\n");
                return 1;
                break;
        }

        // Répète le jeu jusqu'à x fois de generations
        for (int i = 0; i < generations; i++) {
            update_grid(grid);
            printf("\n\nGeneration %d:\n", i + 1);
            print_grid(grid);
        }

        // Demande à l'utilisateur s'il veut continuer à jouer
        printf("\nDo you want to play again? (y/n) ");
        scanf(" %c", &choice);
    }

    if (choice == 'n') {
        printf("\nDo you want to save? (y/n) ");
        scanf(" %c", &save);
        if (save == 'y') {
            save_grid(grid, filename);
        }
    }

    return 0;
}

// Initialise la grille avec des valeurs aléatoires
void init_grid(int grid[WIDTH][HEIGHT]) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            grid[x][y] = rand() % 2;
        }
    }
}


// Affiche la grille
void print_grid(int grid[WIDTH][HEIGHT]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", grid[x][y] ? '*' : ' ');
        }
        printf("\n");
    }
}

// Actualise la grille
void update_grid(int grid[WIDTH][HEIGHT]) {
    int next_grid[WIDTH][HEIGHT];

    // Calcul l'état de chaque prochaine cellule
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            int neighbors = count_neighbors(grid, x, y);
            if (grid[x][y]) {
                // Cellule vivante
                next_grid[x][y] = (neighbors == 2 || neighbors == 3);
            } else {
                // Cellule morte
            next_grid[x][y] = (neighbors == 3);
            }
        }
    }

    // Copie le prochain état sur l'état actuel
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            grid[x][y] = next_grid[x][y];
        }
    }
}


// Compte le nb de voisin morts ou vivants
int count_neighbors(int grid[WIDTH][HEIGHT], int x, int y) {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nx = x + i;
            int ny = y + j;

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && (i != 0 || j != 0)) {
                count += grid[nx][ny];
            }
        }
    }
    return count;
}


// Manual mode : permet à l'utilisateur de rentrer coordonnées X, Y exactes
void manual_mode(int grid[WIDTH][HEIGHT]) {
    int x, y;
    char input[10];

    printf("Enter coordinates (x,y) of cell to toggle (or q to quit): ");
    
    while (scanf("%s", input) == 1 && input[0] != 'q') {
        sscanf(input, "%d,%d", &x, &y);
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            grid[x][y] = !grid[x][y];
        }
        printf("Enter coordinates (x,y) of cell to toggle (or q to quit): ");
    }
}


// Random mode : tant que le count (parmis ttes cellules) n'est pas à 0 il rend des cellules x, y vivantes
void random_mode(int grid[WIDTH][HEIGHT]) {
    int count = rand() % 2;
        while (count > 0) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        if (!grid[x][y]) {
            grid[x][y] = 1;
            count--;
        }
    }
}


// File mode : lis le stade initiale d'un fichier afin de l'ouvrir
void file_mode(int grid[WIDTH][HEIGHT]) {
    char filename[100];
    FILE* file;
    int x, y, value;
    printf("Enter the name of the file: ");
    scanf("%s", filename);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: file not found\n");
    return;
}

    while (fscanf(file, "%d,%d,%d", &x, &y, &value) == 3) {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            grid[x][y] = value;
    }
}

    fclose(file);
}


// Save grid : permet de sauvegarder la grille
void save_grid(int grid[WIDTH][HEIGHT], char* filename) {
    FILE* file = fopen(filename, "w");
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            fprintf(file, "%d ", grid[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Grid saved here : %s\n", filename);
}