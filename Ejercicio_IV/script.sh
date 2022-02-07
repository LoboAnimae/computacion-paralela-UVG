separator="\n=====================================================\n"
input=riemann3.c
output=riemann3.out
args="1 40 10"
gcc-11 -o $output $input -fopenmp
echo "$separator[INFO] Generated $output from $input"
echo "[INFO] Running.$separator"
./$output $args

echo "$separator[INFO] Finished.$separator"