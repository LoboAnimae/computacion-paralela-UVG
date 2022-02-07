separator="\n=====================================================\n"
input=hello_omp.c
output=hello.out
args=10
gcc-11 -o $output $input -fopenmp
echo "$separator[INFO] Generated $output from $input"
echo "[INFO] Running.$separator"
./$output $args

echo "$separator[INFO] Finished.$separator"