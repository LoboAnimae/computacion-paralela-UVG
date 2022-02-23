output=piSeriesSeq.o
rm $output
gcc-11 -o $output piSeriesNaive.c -fopenmp
./$output 16 10000