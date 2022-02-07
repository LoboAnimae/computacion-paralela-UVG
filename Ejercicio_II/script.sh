input=hello_omp_mod.c
output=hello_omp_mod.out
args=23
gcc-11 -o $output $input -fopenmp
echo -e "Generated $output from $input"
echo -e "Running.\n"
./$output $args

echo -e "\nFinished."