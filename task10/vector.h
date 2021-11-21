#pragma once

class vector //geometrical vector
{
public:
    vector() = delete;

    vector(int n)
    {
        n_ = n;
        v_ = new double[n_];
    }

    ~vector()
    {
        delete[] v_;
    }

    const double& operator[](size_t i) const
    {
        return v_[i];
    }

    double& operator[](size_t i)
    {
        return v_[i];
    }

    vector operator-(const vector& rhs)
    {
        assert(rhs.n_ == n_);
        vector result{n_};
        for (int i = 0; i < n_; ++i)
        {
            result[i] = v_[i] - rhs[i]; 
        }
        return result;
    }

    /* bool operator==(const vector& rhs)
    {
        assert(rhs.n_ == n_);
        for (int i = 0; i < n_; ++i)
        {
            if (rhs[i] != v_[i])
            {
                return false;
            }
        }

        return true;
    } */

    vector& operator=(const vector& rhs)
    {
        if (&rhs == this)
        {
            return *this;
        }

        n_ = rhs.n_;
        delete[] v_;
        v_ = new double[n_];
        for (int i = 0; i < n_; ++i)
        {
            v_[i] = rhs[i];
        }

        return *this;
    }

    int size() const
    {
        return n_;
    }

private:
    double *v_;
    int n_;
};