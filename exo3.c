#include "stdlib.h"
#include "stdio.h"

#define NB_RESTAURANT_MAX 21

typedef struct restaurant {
    char nom[40];
    char adresse[40];
    double position[2]; // position[0]=x,position[1]=y
    char specialite[40];
} Restaurant;

Restaurant nouveau_restaurant(char* ligne) {

    Restaurant res;
    char* pos = malloc(40);

    int i=0,j=0;
    while (ligne[i]!=';'){
        res.nom[j++]=ligne[i++];
    }
    res.nom[j]=0;
    j=0;
    i++;
    while (ligne[i]!=';'){
        res.adresse[j++]=ligne[i++];
    }
    res.adresse[j]=0;
    j=0;
    i++;
    while (ligne[i]!=';'){
        pos[j++]=ligne[i++];
    }
    pos[j]=0;
    j=0;
    i+=2;
    while (ligne[i]!=';'){
        if (ligne[i]!='{' && ligne[i]!='}')
            res.specialite[j++]=ligne[i++];
        else
            i++;
    }
    res.specialite[j]=0;
    j=0;
    i=3;

    char x_pos[20],y_pos[20];
    while(pos[i]!=','){
        if (pos[i]!='=')
            x_pos[j++]=pos[i++];
        else
            i++;
    }
    x_pos[j]=0;
    j=0;
    i+=4;
    while (pos[i]!=')'){
        y_pos[j++]=pos[i++];
    }
    free(pos);
    y_pos[j]=0;

    res.position[0]=atof(x_pos);
    res.position[1]=atof(y_pos);
    return res;
}

int lire_restaurant(const char* chemin,Restaurant (*restaurant)[NB_RESTAURANT_MAX]){
    int nb=0;
    FILE* fichier = fopen(chemin,"r");
    if (fichier == NULL) {
        printf("Impossiblde d'ouvrir le fichier %s\n",chemin);
        exit(1);
    }

    char ligne[124];
    fgets(ligne,124,fichier);

    while (fgets(ligne,124,fichier)!=NULL){
        if (ligne[0]!='\n' && ligne[0]!=' '){
            (*restaurant)[nb++] = nouveau_restaurant(ligne);
        }
    }

    fclose(fichier);
    return nb;
}

void inserer_restaurant(Restaurant restaurant){
    FILE* fichier = fopen("restau.txt","a");
    fprintf(fichier,"%s%c%s%c%s%.3f%s%.3f%s%s%s%s\n",restaurant.nom,';',restaurant.adresse,';',"(x=",restaurant.position[0],", y=",restaurant.position[1],");"," {",restaurant.specialite,"};");
    fclose(fichier);
}

int main() {
    Restaurant restaurant[NB_RESTAURANT_MAX];
    const char* chemin = "restau.txt";
    int nb_restaurant = lire_restaurant(chemin,&restaurant);
    printf("Le nombre de restaurant est %d\n",nb_restaurant);
    for (int i=0;i<nb_restaurant;i++){
        printf("Le nom du restaurant est %s, l'adresse est %s, la postion est (x=%f,y=%f) et la specialite %s\n",restaurant[i].nom,restaurant[i].adresse,restaurant[i].position[0],restaurant[i].position[1],restaurant[i].specialite);
    }
    inserer_restaurant(restaurant[0]);
    return 0;
}
