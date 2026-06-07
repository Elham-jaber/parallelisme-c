#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "crible.h"

int main(int argc, char *argv[]) {

    int N = atoi(argv[1]);
    bool L[N+1];

    clock_t c1 = clock();
    struct timeval t1, t2;

    gettimeofday(&t1, NULL);

    crible(N, L);

    gettimeofday(&t2, NULL);//on utilise pour calculer le temps réel (wall)
    clock_t c2 = clock();//clock pour le temps requis de CPU- phase calcul seulement

    double cpu_time = (double)(c2 - c1) / CLOCKS_PER_SEC;

    double wall_time =
        (t2.tv_sec - t1.tv_sec) +
        (t2.tv_usec - t1.tv_usec) / 1000000.0;

    ecrire_fichier(N, L, "resultat.txt");

    printf("%d,%f,%f\n", N, cpu_time, wall_time);

    return 0;
}
