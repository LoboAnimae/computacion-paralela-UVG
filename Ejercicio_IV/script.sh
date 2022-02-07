input=riemann3.c
output=riemann3.out
args="1 40 10"
gcc-11 -o $output $input -fopenmp
echo -e "Generated $output from $input"
echo -e "Running.\n"
./$output $args

echo -e "\nFinished."