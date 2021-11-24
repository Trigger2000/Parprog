#include "solvers.h"

void diffur_solver::solve()
{
    // 2 точки - это граничные условия, их не учитываем
    int size = N_ - 2;
    // начальное приближение равно нулю
    vector y{size};

    // метод Ньютона
    // в файле Math.jpg представлен вывод формул, используемых ниже
    for (int k = 0; k < 100; ++k)
    {
        // трехдиагональная матрица Якоби
        matrix A{size};
        for (int i = 0; i < size - 1; ++i)
        {
            A.a_[i][i] = -2 + 5.0 * STEP * STEP / 6.0 * std::exp(-y[i]);
            A.a_[i + 1][i] = 1 + STEP * STEP / 12.0 * std::exp(-y[i]);
            A.a_[i][i + 1] = 1 + STEP * STEP / 12.0 * std::exp(-y[i + 1]);
        }
        A.a_[size - 1][size - 1] = -2 + 5.0 * STEP * STEP / 6.0 * std::exp(-y[size - 1]);

        // правая часть
        vector f{size};
        // инициализируем первый и последний эл-ты с учетом граничных условий
        f[0] = y[1] - 2 * y[0] + 1 - STEP * STEP * (5.0 / 6.0 * std::exp(-y[0]) + 1.0 / 12.0 * 
                  std::exp(-y[1]) + 1.0 / 12.0 * std::exp(-1.0));
        f[size - 1] = b_ - 2 * y[size - 1] + y[size - 2] - STEP * STEP * (5.0 / 6.0 * std::exp(-y[size - 1]) + 1.0 / 12.0 * 
                  std::exp(-b_) + 1.0 / 12.0 * std::exp(-y[size - 2]));
        
        for (int i = 1; i < size - 1; ++i)
        {
            f[i] = y[i + 1] - 2 * y[i] + y[i - 1] - STEP * STEP * (5.0 / 6.0 * std::exp(-y[i]) + 1.0 / 12.0 * 
                  std::exp(-y[i + 1]) + 1.0 / 12.0 * std::exp(-y[i - 1]));
        }

        sole_solver solver(A, f);
        const vector& correction = solver.progonka_solve();
        y = y - correction;
    }

    solution_[0] = 1;
    solution_[N_ - 1] = b_;
    for (int i = 0; i < size; ++i)
    {
        solution_[i + 1] = y[i];
    }
}

void diffur_solver::print_to_file(const std::string& filename)
{
    std::ofstream file;
    file.open(filename, std::ios::trunc);
    for (int i = 0; i < POINTS; ++i)
    {
        file << STEP * i << "\t" << solution_[i] << "\n"; 
    }
    file.close();
}

// трехдиагональная прогонка
// алгоритм и большинство обозначений взяты отсюда
// https://pro-prof.com/forums/topic/sweep-method-for-solving-systems-of-linear-algebraic-equations
// трехдиагональную прогонку из-за зависимости по данным сложно распараллелить
vector sole_solver::progonka_solve()
{
    int size = y_.size();
    vector result{size};

    double *y_ar = new double[size];
    double *alpha_ar = new double[size];
    double *betta_ar = new double[size];

    // прямой ход прогонки
    y_ar[0] = A_.a_[0][0];
    alpha_ar[0] = -A_.a_[0][1] / y_ar[0];
    betta_ar[0] = y_[0] / y_ar[0];

    for (int i = 1; i < size - 1; ++i)
    {
        y_ar[i] = A_.a_[i][i] + A_.a_[i][i - 1] * alpha_ar[i - 1];
        alpha_ar[i] = -A_.a_[i][i + 1] / y_ar[i];
        betta_ar[i] = (y_[i] - A_.a_[i][i - 1] * betta_ar[i - 1]) / y_ar[i];
    }

    y_ar[size - 1] = A_.a_[size - 1][size - 1] + A_.a_[size - 1][size - 2] * alpha_ar[size - 2];
    betta_ar[size - 1] = (y_[size - 1] - A_.a_[size - 1][size - 2] * betta_ar[size - 2]) / y_ar[size - 1]; 
    // прямой ход прогонки

    // обратный ход прогонки
    result[size - 1] = betta_ar[size - 1];

    for (int i = size - 2; i >= 0; --i)
    {
        result[i] = alpha_ar[i] * result[i + 1] + betta_ar[i];
    }
    // обратныц ход прогонки

    delete[] y_ar;
    delete[] alpha_ar;
    delete[] betta_ar;
    return result;
}

// алгоритм редукции, его удобно распараллеливать
// взят отсюда https://intuit.ru/studies/courses/4447/983/lecture/14931?page=4 
// см. секцию "последовательный алгоритм"
// Долго реализовывать, может быть как-нибудь потом вернусь
/*
vector sole_solver::reduction_solve()
{
    int iterations = 10; // log2(POINTS - 1)-1 - число итераций в редукции
    matrix *m_array = new matrix[iterations];
    for (int i = 1; i <= 10; ++i)
    {
        int matrix_size = 2048 / std::pow(2, i);
        new (&(m_array[i - 1])) matrix(matrix_size);
    }

    // прямой ход редукции
    for (int i = 1; 1 <= 10; ++i)
    {
        int matrix_size = 2048 / std::pow(2, i);
        matrix current = m_array[i - 1];
        if (i == 1)
        {
            for (int j = 0; j < matrix_size; ++j)
            {
                int index = j * 2 + 1;
                double alpha = -A_[index][]
                current.a_[]
            }
        }
        else
        {

        }
    }
} */