input=hello_omp.c
output=hello.out
args=10
gcc-11 -o $output $input -fopenmp
echo -e "Generated $output from $input"
echo -e "Running.\n"
./$output $args

echo -e "\nFinished."