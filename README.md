Crible d'Ératosthène - Parallélisation (Phases 1, 2, 3)
Projet de L3 Informatique - JABER Elham & ELCHAMAA Mohamad - IFA2 2025-2026

Description
Implémentation du crible d'Ératosthène en trois versions progressives : séquentielle, puis parallèle avec deux stratégies de threads différentes. Testé sur N = 1 000 000 000.

Les 3 phases
Phase 1 - Séquentielle

Un seul thread, exécution classique étape par étape
Temps très faibles pour les petits N (< 5 ms jusqu'à N=10⁶)
Sert de référence de base, ne profite pas des multi-cœurs

Phase 2 - Threads parallèles (stratégie 1)

Un thread créé et détruit par nombre premier traité (pthread_create / pthread_join)
Pour N=10⁹, T=1 : ~19.21s
Gain visible à partir de T=2 (~13.79s), se stabilise autour de 11.5s pour T≥64
Problème : overhead important dû à la recréation de threads à chaque fois

Phase 3 - Workers avec file de travaux (stratégie 2)

T workers créés une seule fois au démarrage, actifs jusqu'à la fin
File de travaux circulaire (tableau de T cases) gérée par fenêtre glissante
Synchronisation par sémaphores (sem_wait, sem_post)
Signal d'arrêt : valeur 0 envoyée à chaque worker
Pour N=10⁹, T=1 : ~13.48s — déjà meilleur que la phase 2 à T=1
Meilleur résultat : T=512 → 9.48s (gain ~30% vs séquentiel)


Résultats phase 3 (N = 1 000 000 000)
TCPU_TIMEWALL_TIME113.47913.47929.6809.66649.9809.98089.6639.662169.6199.620329.6539.652649.8819.8811289.6669.6672569.5649.5645129.4789.47910249.6269.632

Pourquoi la phase 3 est meilleure

Pas de recréation de threads à chaque itération → overhead quasi nul
La file avec fenêtre glissante libère les cases efficacement
La synchronisation par sémaphores est plus légère que pthread_join en boucle


Limite observée
Ajouter plus de threads au-delà d'un certain seuil n'améliore plus les temps — la loi d'Amdahl s'applique : la partie séquentielle (recherche du prochain premier) devient le goulot d'étranglement.
