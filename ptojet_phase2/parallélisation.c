
/*travail en binome : elchammaa mohamad et jaber elham L3 informatique IFA2*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>


/* Variables globales partagées */
long long N;
char *tab;

/* Fonction exécutée par chaque thread */
void* marquer(void *arg)
{
    long long p = *((long long*)arg);

    for(long long j = p*p; j < N; j += p)
        tab[j] = 0;

    return NULL;
}

/* Trouver le prochain nombre premier */
long long prochain_premier(long long current)
{
    long long next = current + 1;

    while(next*next < N && tab[next] == 0)
        next++;

    return next;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s <N> <T>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    int T = atoi(argv[2]);

    tab = malloc(N * sizeof(char));
    if(tab == NULL)
    {
        printf("Erreur allocation\n");
        return 1;
    }

    /* Initialisation */
    for(long long i = 0; i < N; i++)
        tab[i] = 1;

    tab[0] = tab[1] = 0;

    pthread_t threads[T];
    long long p_values[T];

    clock_t cpu_start = clock();
    struct timeval wall_start, wall_end;
    gettimeofday(&wall_start, NULL);

    long long p = 2;

    while(p*p < N)
    {
        int nb_threads = 0;

        for(int i = 0; i < T && p*p < N; i++)
        {
            p_values[i] = p;

            pthread_create(&threads[i], NULL,
                           marquer, &p_values[i]);

            nb_threads++;

            p = prochain_premier(p);
        }

        /* attendre la fin des threads */
        for(int i = 0; i < nb_threads; i++)
            pthread_join(threads[i], NULL);
    }

    clock_t cpu_end = clock();
    gettimeofday(&wall_end, NULL);

    double cpu_time =
        (double)(cpu_end - cpu_start) / CLOCKS_PER_SEC;

    double wall_time =
        (wall_end.tv_sec - wall_start.tv_sec) +
        (wall_end.tv_usec - wall_start.tv_usec)/1000000.0;

    printf("%lld,%d,%f,%f\n", N, T, cpu_time, wall_time);

    free(tab);
    return 0;
}