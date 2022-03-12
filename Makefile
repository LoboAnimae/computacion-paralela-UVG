OBJS	= main.o
SOURCE_SEQ	= sequential/src/*.c sequential/src/**/*.c
HEADER_SEQ	= sequential/include
OUT	= main.o
CC_SEQ	 = gcc-11
LFLAGS_SEQ	 = -fopenmp

OBJS	= main.o
SOURCE_PAR	= parallel/src/*.c parallel/src/**/*.c
HEADER_PAR	= parallel/include
OUT	= main.o
CC_PAR	 = gcc-11
LFLAGS_PAR	 = -fopenmp

all: clean
	clear
	echo "\n\n\n\n------------>Use \"make parallel\" or \"make seq\"<------------\n\n\n\n"

parallel: clean
	$(CC_PAR) -o PAR_$(OUT) $(SOURCE_PAR) $(LFLAGS_PAR) -I$(HEADER_PAR)

seq: clean
	$(CC_SEQ) -o SEQ_$(OUT) $(SOURCE_SEQ) $(LFLAGS_SEQ) -I$(HEADER_SEQ)


clean:
	rm -f PAR_$(OBJS) $(OUT)
	rm -f SEQ_$(OBJS) $(OUT)
