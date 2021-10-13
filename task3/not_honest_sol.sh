g++ -fopenmp not_honest.cpp -o not_honest.out
OMP_NUM_THREADS=27 ./not_honest.out
rm not_honest.out