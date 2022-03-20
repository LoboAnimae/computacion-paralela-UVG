INC_DIR = include
LIBS    = -fopenmp
FILES 	= src/*.cc 
OUTPUT 	= build.o

all: src/main.cc
	g++-11 -o build.o $(FILES) -I$(INC_DIR) $(LIBS)

debug: src/main.cc
	g++-11 -g $(FILES) -o build.o -I$(INC_DIR) $(LIBS)