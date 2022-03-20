CC      = g++
INC_DIR = include
LIBS    = -fopenmp
FILES 	= src/*.cc 
OUTPUT 	= build.o

all: src/main.cc
	$(CC) -o build.o $(FILES) -I$(INC_DIR) $(LIBS)

debug: src/main.cc
	$(CC) -g $(FILES) -o build.o -I$(INC_DIR) $(LIBS)