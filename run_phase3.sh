#!/bin/bash
# Script de la Phase 3 : Crible d'Eratosthene (Workers)
# Binome : ELCHAMAA Mohamad et JABER Elham - L3 IFA2
# Enseignant : Stephane Rubini - UBO 2025-2026
# Compile workers.c avec travail.c et lance
# T = 2^i pour 0 <= i <= 10, N fixe a 10^9


N=1000000000
OUTPUT="resultats_phase3.csv"

echo "Compilation..."
gcc -O2 -o workers.exe workers.c travail.c -lpthread

if [ $? -ne 0 ]; then
    echo "Erreur de compilation !"
    exit 1
fi
echo "Compilation OK"
echo ""

echo "N,T,CPU_TIME,WALL_TIME" > "$OUTPUT"

for T in 1 2 4 8 16 32 64 128 256 512 1024; do
    echo "Execution avec N=$N et T=$T ..."
    ./workers.exe $N $T >> "$OUTPUT"
done

echo ""
echo "Termine ! Resultats dans : $OUTPUT"
