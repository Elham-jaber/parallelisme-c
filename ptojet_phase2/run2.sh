#!/bin/bash

# N fixé à 10^9
N=1000000000

# Création du fichier CSV
echo "N,T,CPU_TIME,WALL_TIME" > resultats_phase2.csv

# Boucle sur T = 2^i
for i in {0..10}
do
    T=$((2**i))
    ./programme $N $T >> resultats_phase2.csv
done