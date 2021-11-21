#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ISIZE 10000
#define JSIZE 10000

int main(int argc, char **argv)
{
    double **a = (double**)calloc(ISIZE, sizeof(double*));
    for (int k = 0; k < ISIZE; ++k)
    {
        a[k] = (double*)calloc(JSIZE, sizeof(double));
    }

    int i, j;
    FILE *ff;

    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i +j;
        }
    }

    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = sin(0.00001 * a[i][j]);
        }
    }

    ff = fopen("result.txt","w");
    for(i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);
}