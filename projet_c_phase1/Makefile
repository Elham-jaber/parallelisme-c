CC = gcc
CFLAGS = -Wall -O2
TARGET = programme.exe
SRC = main.c crible.c

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run:
	./$(TARGET) 100

csv:
	echo N,CPU_TIME,WALL_TIME > resultats.csv
	./$(TARGET) 10 >> resultats.csv
	./$(TARGET) 100 >> resultats.csv
	./$(TARGET) 1000 >> resultats.csv
	./$(TARGET) 10000 >> resultats.csv
	./$(TARGET) 100000 >> resultats.csv
	./$(TARGET) 1000000 >> resultats.csv
	./$(TARGET) 10000000 >> resultats.csv

clean:
	del /Q *.exe *.o resultats.csv
