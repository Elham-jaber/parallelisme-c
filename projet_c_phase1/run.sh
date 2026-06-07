#!/bin/bash

echo "N,CPU_TIME,WALL_TIME" > resultats.csv

./programme.exe 10 >> resultats.csv
./programme.exe 100 >> resultats.csv
./programme.exe 1000 >> resultats.csv
./programme.exe 10000 >> resultats.csv
./programme.exe 100000 >> resultats.csv
./programme.exe 1000000 >> resultats.csv
./programme.exe 10000000 >> resultats.csv
./programme.exe 100000000 >> resultats.csv
./programme.exe 1000000000 >> resultats.csv
