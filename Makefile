INC_DIR = include
LIBS    = -fopenmp
FILES 	= src/*.c src/**/*.c
OUTPUT 	= build.o

all: src/main.c
	gcc-11 -o build.o $(FILES) -I$(INC_DIR) $(LIBS)