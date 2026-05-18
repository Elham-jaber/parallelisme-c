//travail en binome IFA2: ELHAM JABER & MOHAMAD ELCHAMAA
//L3 INFORMATIQUE

#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include "crible.h"
// Fonction qui calcule le crible
void crible(int N, bool L[]) {

    for(int i = 0; i <= N; i++)
        L[i] = true;

    L[0] = false;
    L[1] = false;

    for(int i = 2; i*i <= N; i++) {
        if(L[i]) {
            for(int j = i*i; j <= N; j += i) {
                L[j] = false;
            }
        }
    }
}

// Fonction qui écrit dans un fichier
void ecrire_fichier(int N, bool L[], const char *nom_fichier) {

    FILE *f = fopen(nom_fichier, "w");

    if(f == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }

    for(int i = 2; i <= N; i++) {
        if(L[i])
            fprintf(f, "%d\n", i);
    }

    fclose(f);
}

