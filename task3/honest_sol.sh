g++ -fopenmp honest.cpp -o honest.out
OMP_NUM_THREADS=27 ./honest.out
rm honest.out