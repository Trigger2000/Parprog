g++ -fopenmp main.cpp
OMP_NUM_THREADS=4 ./a.out $1
rm a.out