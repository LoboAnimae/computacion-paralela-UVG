separator="\n=====================================================\n"
input=hello_omp_mod.c
output=hello_omp_mod.out
args=23
gcc-11 -o $output $input -fopenmp
echo "$separator[INFO] Generated $output from $input"
echo "[INFO] Running.$separator"
./$output $args

echo "$separator[INFO] Finished.$separator"