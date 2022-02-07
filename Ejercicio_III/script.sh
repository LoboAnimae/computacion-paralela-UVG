input=riemann2.c
output=riemann2.out
separator="\n=====================================================\n"
args="1 40 10"
gcc-11 -o $output $input -fopenmp
echo "$separator This exercise had a question to be answered. The answer is as follows:"
cat ./PREG.md
echo "$separator[INFO] Generated $output from $input"
echo "[INFO] Running.$separator"
./$output $args

echo "$separator[INFO] Finished.$separator"