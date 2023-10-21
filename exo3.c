#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define NB_RESTAURANT_MAX 30

typedef struct restaurant {
    char nom[40];
    char adresse[40];
    double position[2]; // position[0]=x, position[1]=y
    char specialite[40];
} Restaurant;

// Déclarations de fonctions
void seek_to_next_line(void);
Restaurant nouveau_restaurant(char *ligne);
int lire_restaurant(const char *chemin, Restaurant restaurant[]);
void inserer_restaurant(Restaurant restaurant);
void cherche_restaurant(double x, double y, double rayon_recherche, Restaurant results[]);
void cherche_par_specialite(double x, double y, char *specialite[], Restaurant results[]);
void tri_tableau(Restaurant results[], double x, double y, int n);
void afficher_restaurant(Restaurant restaurant[]);

int main() {
    Restaurant restaurant[NB_RESTAURANT_MAX];
    printf("Bonjour utilisateur, veuillez renseigner votre location via des coordonnées x, y\n");
    double x, y;
    printf("X=");
    scanf("%lf", &x);
    printf("Y=");
    scanf("%lf", &y);

    int jeu = 1, reponse;
    while (jeu) {
        printf("---\nVeuillez sélectionner l'une des options suivantes :\n");
        printf("1. Rechercher des restaurants par spécialité\n");
        printf("2. Rechercher des restaurants dans un rayon proche de moi\n");
        printf("3. Insérer un restaurant dans la base restau.txt\n");
        printf("4. Quitter ce menu et annuler votre recherche.\n---\n");

        scanf("%d", &reponse);
        switch (reponse) {
            case 1: {
                printf("Veuillez indiquer les spécialités recherchées (AFIN d'affirmer que vous avez fini votre sélection, écrivez NULL)\n");
                char specialite[11][40];
                char *specialite_ptr[11];
                int i = 0, j = 0;
                seek_to_next_line();
                while (i < 11) {
                    fgets(specialite[i], sizeof(specialite[i]), stdin);
                    while (specialite[i][j + 1]) {
                        j++;
                    }
                    specialite[i][j] = 0;
                    if (j == 4) {
                        if (specialite[i][0] == 'N' && specialite[i][1] == 'U' && specialite[i][2] == 'L' && specialite[i][3] == 'L') {
                            break;
                        }
                    }
                    specialite_ptr[i] = specialite[i];
                    j = 0;
                    i++;
                }
                specialite_ptr[i] = NULL;
                Restaurant results[NB_RESTAURANT_MAX];
                cherche_par_specialite(x, y, specialite_ptr, results);
                jeu = 0;
                break;
            }
            case 2: {
                printf("Veuillez indiquer le rayon de recherche souhaité\n");
                double rayon;
                scanf(" %lf", &rayon);
                Restaurant results[NB_RESTAURANT_MAX];
                cherche_restaurant(x, y, rayon, results);
                jeu = 0;
                break;
            }
            case 3: {
                Restaurant res;
                int i = 0;
                printf("Veuillez indiquer, le nom, l'adresse et les spécialités du restaurant ainsi que sa position X,Y\n");
                seek_to_next_line();
                fgets(res.nom, sizeof(res.nom), stdin);
                while (res.nom[i + 1]) {
                    i++;
                }
                res.nom[i] = 0;
                i = 0;
                fgets(res.adresse, sizeof(res.adresse), stdin);
                while (res.adresse[i + 1]) {
                    i++;
                }
                res.adresse[i] = 0;
                i = 0;
                fgets(res.specialite, sizeof(res.specialite), stdin);
                while (res.specialite[i + 1]) {
                    i++;
                }
                res.specialite[i] = 0;
                scanf("%lf", &res.position[0]);
                scanf("%lf", &res.position[1]);
                inserer_restaurant(res);
                break;
            }
            case 4: {
                jeu = 0;
                break;
            }
            default:
                printf("Mauvais choix de menu, veuillez réessayer\n");
                break;
        }
    }
    afficher_restaurant(restaurant);
    return 0;
}

void seek_to_next_line(void) {
    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n');
}

Restaurant nouveau_restaurant(char *ligne) {
    Restaurant res;
    char *pos = malloc(40);

    int i = 0, j = 0;
    while (ligne[i] != ';') {
        res.nom[j++] = ligne[i++];
    }
    res.nom[j] = 0;
    j = 0;
    i++;
    while (ligne[i] != ';') {
        res.adresse[j++] = ligne[i++];
    }
    res.adresse[j] = 0;
    j = 0;
    i++;
    while (ligne[i] != ';') {
        pos[j++] = ligne[i++];
    }
    pos[j] = 0;
    j = 0;
    i += 2;
    while (ligne[i] != ';') {
        if (ligne[i] != '{' && ligne[i] != '}')
            res.specialite[j++] = ligne[i++];
        else
            i++;
    }
    res.specialite[j] = 0;
    j = 0;
    i = 3;

    char x_pos[20], y_pos[20];
    while (pos[i] != ',') {
        if (pos[i] != '=')
            x_pos[j++] = pos[i++];
        else
            i++;
    }
    x_pos[j] = 0;
    j = 0;
    i += 4;
    while (pos[i] != ')') {
        y_pos[j++] = pos[i++];
    }
    free(pos);
    y_pos[j] = 0;

    res.position[0] = atof(x_pos);
    res.position[1] = atof(y_pos);
    return res;
}

int lire_restaurant(const char *chemin, Restaurant restaurant[]) {
    int nb = 0;
    FILE *fichier = fopen(chemin, "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", chemin);
        exit(1);
    }

    char ligne[124];
    fgets(ligne, 124, fichier);

    while (fgets(ligne, 124, fichier) != NULL) {
                if (ligne[0] != '\n' && ligne[0] != ' ') {
            restaurant[nb++] = nouveau_restaurant(ligne);
        }
    }
    fclose(fichier);
    return nb;
}

void inserer_restaurant(Restaurant restaurant) {
    FILE *fichier = fopen("restau.txt", "a");
    fprintf(fichier, "\n%s%c%s%c%s%.3f%s%.3f%s%s%s%s\n", restaurant.nom, ';', restaurant.adresse, ';', "(x=", restaurant.position[0], ", y=", restaurant.position[1], ");", " {", restaurant.specialite, "};");
    fclose(fichier);
}

void cherche_restaurant(double x, double y, double rayon_recherche, Restaurant results[]) {
    Restaurant liste_restaurant[NB_RESTAURANT_MAX];
    int nb_restaurants = lire_restaurant("restau.txt", liste_restaurant);
    int indice = 0;
    for (int i = 0; i < nb_restaurants; i++) {
        if (sqrt(pow(x - liste_restaurant[i].position[0], 2) + pow(y - liste_restaurant[i].position[1], 2)) <= rayon_recherche) {
            results[indice++] = liste_restaurant[i];
        }
    }
    results[indice].position[0] = INT_MIN;
    results[indice].position[1] = INT_MIN;
}

int specialite_dans_restaurant(Restaurant restaurant, char *specialite[]) {
    int Present = 0;
    for (int k = 0; specialite[k] != NULL; k++) {
        for (int i = 0; restaurant.specialite[i] != '\0'; i++) {
            Present = 0;
            for (int j = 0; specialite[k][j] != '\0'; j++) {
                if (restaurant.specialite[i + j] != specialite[k][j]) {
                    Present = 0;
                    break;
                }
                Present = 1;
            }
            if (Present) {
                break;
            }
        }
        if (Present) {
            break;
        }
    }
    if (Present) {
        return 1;
    }
    return 0;
}

void tri_tableau(Restaurant results[], double x, double y, int n) {
    Restaurant c;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (sqrt(pow(x - results[j].position[0], 2) + pow(y - results[j].position[1], 2)) > sqrt(pow(x - results[j + 1].position[0], 2) + pow(y - results[j + 1].position[1], 2))) {
                c = results[j];
                results[j] = results[j + 1];
                results[j + 1] = c;
            }
        }
    }
}

void cherche_par_specialite(double x, double y, char *specialite[], Restaurant results[]) {
    Restaurant liste_restaurant[NB_RESTAURANT_MAX];
    int nb_restaurants = lire_restaurant("restau.txt", liste_restaurant);
    int indice = 0;
    for (int i = 0; i < nb_restaurants; i++) {
        if (specialite_dans_restaurant(liste_restaurant[i], specialite)) {
            results[indice++] = liste_restaurant[i];
        }
    }
    tri_tableau(results, x, y, indice);
    results[indice].position[0] = INT_MIN;
    results[indice].position[1] = INT_MIN;
}

void afficher_restaurant(Restaurant restaurant[]) {
    int i = 0;
    while (restaurant[i].position[0] != INT_MIN && restaurant[i].position[1] != INT_MIN) {
        printf("%d. Le nom du restaurant est %s, l'adresse est %s, la position est (x=%f,y=%f) et la spécialité est %s\n", i + 1, restaurant[i].nom, restaurant[i].adresse, restaurant[i].position[0], restaurant[i].position[1], restaurant[i].specialite);
        i++;
    }
}

