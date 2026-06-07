# Crible d'Ératosthène — Parallélisation multi-threads

Projet de synthèse L3 Informatique 2025-2026
Elham Jaber & Mohamad Elchamaa — IFA2

---

## C'est quoi ce projet ?

On a implémenté le crible d'Ératosthène pour trouver tous les nombres premiers jusqu'à 1 milliard (N = 10⁹), en C avec pthreads. L'idée était de comparer trois façons de faire : sans threads, avec threads classiques, et avec un pool de workers.

| Phase | Approche | En gros |
|-------|----------|---------|
| Phase 1 | Séquentielle | Un seul thread, pas de parallélisme — sert de référence |
| Phase 2 | Threads parallèles | On crée et détruit des threads à chaque nombre premier |
| Phase 3 | Workers + file de travaux | Un pool fixe de T workers qui tournent en permanence |

---

## Structure du projet

```
├── 2DGraphicCore/              # Module graphique 2D
├── Robi/                       # Application principale (Maven)
│   └── pom.xml
├── SParser/                    # Module parseur
├── Prototype_basse_fidelite.pdf
├── .gitlab-ci.yml              # Pipeline CI/CD
└── lisezMoi.md
```

---

## Résultats (N = 10⁹)

### Phase 3 — la meilleure des trois

| Threads (T) | CPU Time (s) | Wall Time (s) |
|:-----------:|:------------:|:-------------:|
| 1 | 13.479 | 13.479 |
| 2 | 9.680 | 9.666 |
| 8 | 9.663 | 9.662 |
| 64 | 9.881 | 9.881 |
| 256 | 9.564 | 9.564 |
| **512** | **9.478** | **9.479** |
| 1024 | 9.626 | 9.632 |

Le meilleur temps est à T=512 avec 9.48s, soit environ 30% plus rapide qu'avec un seul thread.

---

## Pourquoi la phase 3 gagne ?

- En phase 2, on recréait des threads à chaque nombre premier traité, ce qui coûte cher en temps (overhead).
- En phase 3, les threads sont créés une seule fois au démarrage et restent actifs. La synchronisation passe par des sémaphores, ce qui est beaucoup plus léger.
- Résultat : même à T=1, la phase 3 est déjà plus rapide que la phase 2 à T=1.

Au-delà de T=512, le gain plafonne — c'est la loi d'Amdahl : la partie séquentielle (chercher le prochain nombre premier) finit par limiter le gain.

---

## Stack technique

- Langage : C avec pthreads et sémaphores POSIX
- Build : Maven 3.9 + Java 21
- Tests : JUnit 5 (127 tests)
- Couverture : JaCoCo
- Analyse statique : Checkstyle
- CI/CD : GitLab CI — 5 étapes (build, test, coverage, analyze, report)

---

## Lancer le projet

```bash
git clone https://github.com/TON_USERNAME/nom-du-repo.git
cd nom-du-repo

# Compiler
mvn compile -f Robi/pom.xml

# Lancer les tests
mvn test -f Robi/pom.xml
```

---

Projet académique — L3 Informatique 2025-2026
