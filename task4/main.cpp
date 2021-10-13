// задачу делал без доп условий

#include <iostream>

#include <omp.h>

int main()
{
    // создаем 2 матрицы, например, 1000 на 1000
    int N = 1000;
    int **m1 = new int*[N];
    int **m2 = new int*[N];
    int **res = new int*[N];
    for (int i = 0; i < N; ++i)
    {
        m1[i] = new int[N];
        m2[i] = new int[N];
        res[i] = new int[N];
        for (int j = 0; j < N; ++j)
        {
            m1[i][j] = i * N + j;
            m2[i][j] = i * N + j;
        }
    }

    // умножаем матрицы
    #pragma omp parallel for shared(m1, m2, res, N) schedule(dynamic, 100)
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            int sum = 0;
            for (int k = 0; k < N; ++k)
            {
                sum += m1[k][i] * m2[j][k];
            }
            res[i][j] = sum;
        }
    }

    for (int i = 0; i < N; ++i)
    {
        delete[] m1[i];
        delete[] m2[i];
    }
    delete[] m1;
    delete[] m2;

    return 0;
}