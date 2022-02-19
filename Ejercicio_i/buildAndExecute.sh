output=piSeriesSeq.o
rm $output
gcc-11 -o $output piSeriesNaive.c -fopenmp
./$output 8 1000000000