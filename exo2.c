#include <stdio.h>
#include <stdlib.h>

typedef struct POINTS {
    int note;
    struct POINTS* suivant;
} POINTS;

void note_max(POINTS* Points) {
    POINTS* parcours = Points;
    int max = 0;
    while (parcours != NULL) {
        if (parcours->note > max) {
            max = parcours->note;
        }
        parcours = parcours->suivant;
    }
    printf("\n La note maximale de cet examen est : %d", max);
}

void note_min(POINTS* Points) {
    POINTS* parcours = Points;
    int min = parcours->note;
    while (parcours != NULL) {
        if (parcours->note < min) {
            min = parcours->note;
        }
        parcours = parcours->suivant;
    }
    printf("\n La note minimale de cet examen est : %d", min);
}

void note_moy(POINTS* Points) {
    POINTS* parcours = Points;
    float n = 0;
    float summ = 0;
    float moy;
    while (parcours != NULL) {
        summ = summ + parcours->note;
        n++;
        parcours = parcours->suivant;
    }
    if (n != 0) {
        moy = summ / n;
        printf("\n La moyenne des notes de cet examen est : %.2f", moy);
    } else {
        printf("\n Aucune note n'a été entrée.");
    }
}

int main() {
    POINTS* Points = NULL;
    POINTS* courant = NULL;
    int NOTES[7] = {0,0,0,0,0,0,0};
    int max_notes = 0;
    int init = 0;
    char choix = '0';

    while (choix != '7') {
        printf("\n======================================");
        printf("\n       Que voulez-vous faire ?");
        printf("\n======================================");
        printf("\n1. Entrer des notes");
        printf("\n2. Rechercher la note maximale");
        printf("\n3. Rechercher la note minimale");
        printf("\n4. Calculer la moyenne");
        printf("\n5. Afficher le graphique en nuage de points");
        printf("\n6. Afficher le graphique en bâtons");
        printf("\n7. Quitter");
        printf("\n======================================");
        printf("\n   Votre choix ? ");
        choix = getchar();
        fflush(stdin); // pour vider le buffer d'entrée standard

        switch (choix) {
            case '1':
                int saisie = 0;
                while (saisie != -1) {
                    printf("\n Entrez une note (-1 pour arreter) : ");
                    scanf("%d", &saisie);
                    if ((saisie >= 0) && (saisie <= 60)) {
                        POINTS* nouveau_point = malloc(sizeof(POINTS));
                        nouveau_point->note = saisie;
                        nouveau_point->suivant = NULL;
                        if (Points == NULL) {
                            Points = nouveau_point;
                            courant = Points;
                        }
                        else {
                            courant->suivant = nouveau_point;
                            courant = courant->suivant;
                        }
                    }
                    else{
                        printf("\n La note n'est pas comprise entre 0 et 60, recomencez la saisie");
                    }
                }
                init = 0;
                memset(NOTES, 0, sizeof(NOTES)); // Réinitialiser le tableau NOTES à zéro
                break;

            case '2':
                note_max(Points);
                break;

            case '3':
                note_min(Points);
                break;

            case '4':
                note_moy(Points);
                break;

            case '5':
                POINTS* parcours = Points;
                if(init == 0){
                    while(parcours != NULL){
                        if(parcours->note <= 9){
                            NOTES[0]++;
                        }
                        else if(parcours->note <= 19){
                            NOTES[1]++;
                        }
                        else if(parcours->note <= 29){
                            NOTES[2]++;
                        }
                        else if(parcours->note <= 39){
                            NOTES[3]++;
                        }
                        else if(parcours->note <= 49){
                            NOTES[4]++;
                        }
                        else if(parcours->note <= 59){
                            NOTES[5]++;
                        }
                        else if(parcours->note <= 60){
                            NOTES[6]++;
                        }
                        parcours = parcours->suivant;
                    }
                    for(int i=0;i<7;i++){
                        if(NOTES[i] > max_notes){
                            max_notes = NOTES[i];
                        }
                    }
                    init = 1;
                }

                for (int i = max_notes; i >= 0; i--) {
                    parcours = Points;
                    printf("%2d >", i);

                    for (int j=0;j<7;j++) {
                        if (NOTES[j] == i) {
                            printf("   o  ");
                        } else {
                            printf("      "); // Espaces pour le même espacement
                        }
                    }
                    printf("\n");
                }
                printf("    +-----+-----+-----+-----+-----+-----+-----+\n");
                printf("    | 0-9 |10-19|20-29|30-39|40-49|50-59|  60 |\n");
                break;

            case '6':
                POINTS* parcours_2 = Points;
                if(init == 0){
                    while(parcours_2 != NULL){
                        if(parcours_2->note <= 9){
                            NOTES[0]++;
                        }
                        else if(parcours_2->note <= 19){
                            NOTES[1]++;
                        }
                        else if(parcours_2->note <= 29){
                            NOTES[2]++;
                        }
                        else if(parcours_2->note <= 39){
                            NOTES[3]++;
                        }
                        else if(parcours_2->note <= 49){
                            NOTES[4]++;
                        }
                        else if(parcours_2->note <= 59){
                            NOTES[5]++;
                        }
                        else if(parcours_2->note == 60){
                            NOTES[6]++;
                        }
                        parcours_2 = parcours_2->suivant;
                    }
                    max_notes = 0;
                    for(int i=0;i<7;i++){
                        if(NOTES[i] > max_notes){
                            max_notes = NOTES[i];
                        }
                    }
                    init = 1;
                }

                for (int i = max_notes; i >= 0; i--) {
                    parcours = Points;
                    printf("%2d >", i);

                    for (int j=0;j<7;j++) {
                        if (NOTES[j] >= i) {
                            printf(" #####");
                        }
                        else{
                            printf("      ");
                        }
                    }
                    printf("\n");
                }
                printf("    +-----+-----+-----+-----+-----+-----+-----+\n");
                printf("    | 0-9 |10-19|20-29|30-39|40-49|50-59|  60 |\n");
                break;

            case '7':
                printf("\n======== PROGRAMME TERMINE ========\n");
                break;

            default:
                printf("\n\nERREUR : votre choix n'est pas valide ! ");
        }
        printf("\n\n\n");
    }

    // Libérer la mémoire allouée pour les éléments de la liste
    courant = Points;
    while (courant != NULL) {
        POINTS* temp = courant;
        courant = courant->suivant;
        free(temp);
    }

    return 0;
}
