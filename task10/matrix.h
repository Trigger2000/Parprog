#pragma once

struct matrix
{
    matrix()
    {
        n_ = 0;
        a_ = nullptr;
    }

    matrix(int n)
    {
        n_ = n;
        a_ = new double*[n_];
        for (int i = 0; i < n_; ++i)
        {
            a_[i] = new double[n_];
        }
    }

    ~matrix()
    {
        for (int i = 0; i < n_; ++i)
        {
            delete[] a_[i];
        }
        delete[] a_;
    }

    double **a_ = nullptr;
    int n_ = 0;
};