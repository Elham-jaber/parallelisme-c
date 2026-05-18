/*
 * Mini-Projet Système : Crible d'Ératosthène
 * Phase 3 : parallélisme avec la file de travaux fournie 
 *
 * Travail en binôme : ELCHAMAA Mohamad et JABER Elham
 * L3 Informatique - IFA2
 * Enseignant : Stéphane Rubini
 * UBO - Université de Bretagne Occidentale, année 2025-2026
 *
 * On utilise ici exactement les fonctions fournies :
 *   - file_travaux_creer()
 *   - file_travaux_ajouter()
 *   - file_travaux_lire()
 *   - file_travaux_retirer()
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "travail.h"

/* 
 * Données de l'application partagées entre tous les threads
 * On les passe via le champ dt->donnees de la file
 * 
 */
struct donnees_application {
    long long N;   /* borne du crible */
    char *tab;     /* tableau du crible */
};

/*
 * Chaque worker connait son numéro 
 * et un pointeur vers la file de travaux
 */
struct arg_worker {
    int numero;                   /* numéro du worker = index de sa case */
    struct file_travaux *dt;      /* pointeur vers la file de travaux    */
};

/* 
 * Fonction exécutée par chaque worker.
 * Le worker appelle file_travaux_lire() qui est BLOQUANTE :
 * il dort tant qu'aucun travail n'est disponible dans sa case.
 * Quand une valeur p est déposée, il se réveille, barre tous
 * les multiples de p dans le tableau, puis appelle
 * file_travaux_retirer() pour libérer sa case.
 * Il recommence ensuite indéfiniment jusqu'à recevoir la valeur 0
 * (signal d'arrêt).
 */
void *worker_func(void *arg)
{
    struct arg_worker *a = (struct arg_worker *)arg;
    int numero = a->numero;
    struct file_travaux *dt = a->dt;
    struct donnees_application *donnees = (struct donnees_application *)dt->donnees;

    while (1)
    {
        /* Attendre qu'un travail soit disponible dans notre case.
         * file_travaux_lire() bloque sur le sémaphore sem_wait(&pret)
         * jusqu'à ce que le thread principal fasse file_travaux_ajouter() */
        int p = file_travaux_lire(dt, numero);

        /* La valeur 0 est le signal d'arrêt */
        if (p == 0)
            break;

        /* Barrer tous les multiples de p dans le tableau du crible */
        long long N = donnees->N;
        char *tab   = donnees->tab;

        for (long long j = (long long)p * p; j < N; j += p)
            tab[j] = 0;

        /* Signaler que le travail est terminé et libérer la case */
        file_travaux_retirer(dt, numero);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage : %s <N> <T>\n", argv[0]);
        return 1;
    }

    long long N = atoll(argv[1]);
    int T       = atoi(argv[2]);

    if (T < 1) T = 1;

    /*  Allocation et initialisation du tableau du crible */
    char *tab = malloc(N * sizeof(char));
    if (tab == NULL)
    {
        fprintf(stderr, "Erreur : allocation mémoire\n");
        return 1;
    }

    for (long long i = 0; i < N; i++)
        tab[i] = 1;
    tab[0] = tab[1] = 0;

    /* Données de l'application (partagées avec tous les workers) */
    struct donnees_application donnees;
    donnees.N   = N;
    donnees.tab = tab;

    /* 
     * On passe &donnees pour que chaque worker puisse accéder
     * au tableau et à N via dt->donnees */
    struct file_travaux *dt = file_travaux_creer(T, &donnees);
    if (dt == NULL)
    {
        fprintf(stderr, "Erreur : création de la file\n");
        free(tab);
        return 1;
    }

    /* Création des T workers 
     * Chaque worker reçoit son numéro et un pointeur vers la file */
    pthread_t *threads       = malloc(T * sizeof(pthread_t));
    struct arg_worker *args  = malloc(T * sizeof(struct arg_worker));

    for (int i = 0; i < T; i++)
    {
        args[i].numero = i;
        args[i].dt     = dt;
        pthread_create(&threads[i], NULL, worker_func, &args[i]);
    }

    /* Début des chronomètres  */
    clock_t debut_cpu = clock();
    struct timeval debut_reel, fin_reel;
    gettimeofday(&debut_reel, NULL);

    /* 
     * Cœur de l'algorithme :
     * On cherche les nombres premiers p (p*p < N) et on les
     * distribue dans la file avec file_travaux_ajouter().
     * file_travaux_ajouter() est BLOQUANTE si la file est pleine :
     * le thread principal attend qu'un worker libère une case.
     **/
    long long p = 2;
    int case_courante = 0;

    while (p * p < N)
    {
        /* Déposer p dans la case numéro case_courante.
         * file_travaux_ajouter() utilise le sémaphore sem_post(&pret)
         * pour réveiller le worker associé à cette case. */
        file_travaux_ajouter(dt, (int)p);

        /* Passer à la case suivante en round-robin */
        case_courante = (case_courante + 1) % T;

        /* Trouver le prochain nombre premier */
        long long suivant = p + 1;
        while (suivant * suivant < N && tab[suivant] == 0)
            suivant++;
        p = suivant;
    }

    /*
     * On surveille la file : tant qu'il reste des travaux occupés,
     * on attend. On utilise file_travaux_lire_dernier() pour
     * savoir si le plus ancien travail est encore en cours. */
    while (file_travaux_lire_dernier(dt) != 0)
    {
        /* petite attente passive pour ne pas saturer le CPU */
        struct timespec ts = {0, 1000000}; /* 1 ms */
        nanosleep(&ts, NULL);
    }

    /* le worker voit 0
     * et sort de sa boucle. */
    for (int i = 0; i < T; i++)
        file_travaux_ajouter(dt, 0);

    /*Attendre la fin de tous les threads */
    for (int i = 0; i < T; i++)
        pthread_join(threads[i], NULL);

    /*Arrêt de temps*/
    clock_t fin_cpu = clock();
    gettimeofday(&fin_reel, NULL);

    double temps_cpu  = (double)(fin_cpu - debut_cpu) / CLOCKS_PER_SEC;
    double temps_reel = (fin_reel.tv_sec  - debut_reel.tv_sec) +
                        (fin_reel.tv_usec - debut_reel.tv_usec) / 1000000.0;

    /* Sortie CSV : N, T, CPU_TIME, WALL_TIME */
    printf("%lld,%d,%f,%f\n", N, T, temps_cpu, temps_reel);

    /*Nettoyage */
    free(threads);
    free(args);
    free(tab);
    /* Note : on ne libère pas dt car aucune fonction free n'est fournie */

    return 0;
}