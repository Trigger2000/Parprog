/*
    Решен 2ой вариант задачи
    На графике Figure_1 приведены решения для b = 0..1 
    Из графика Figure_2 видно, что в окрестности точки b=1.5 решение краевой задачи становится прямая y=x

    Реализовал решение краевой задачи без замены прогонки на алгоритм редукции
    Возможно, вернусь как-нибудь потом и доделаю редукцию (см. метод sole_solver::reduction_solve() )
*/

#include "solvers.h"

int main()
{
    // считаем для b = 0..1
    #pragma omp parallel for
    for (int i = 0; i <= 10; ++i)
    {
        diffur_solver solver(0.1 * i, POINTS);
        solver.solve();

        std::string filename = "file" + std::to_string(i) + ".dat";
        solver.print_to_file(filename);
    }

    return 0;
}