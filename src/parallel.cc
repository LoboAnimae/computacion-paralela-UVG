#include "parallel.h"
#include "Structures.h"
#include <omp.h>

template <class T>
ParallelBar<T>::ParallelBar(long long iteration, double left, double right, double constant, int space, int time, int threads)
{
    this->iteration = iteration;
    this->left = left;
    this->right = right;
    this->constant = constant;
    this->space = space;
    this->time = time;
    this->thread_count = threads;
    this->solution_matrix = Matrix<T>(space, time);
}

template <class T>
T ParallelBar<T>::get_time_init(int iteration, T left, T right, int max)
{
    return (left - right) / (iteration * max);
}

template <class T>
void ParallelBar<T>::run()
{
    // TODO: Implement this without heap?
#pragma omp single nowait
    {
#pragma omp task
        {
            // TODO: Figure out left and right vals
            this->solution_matrix->set(0, this->iteration, this->left);
            this->solution_matrix->set(this->space - 1, this->iteration, this->right);
        }
        Locality<T> *locality = new Locality<T>(
            this->solution_matrix->get(0, this->iteration),
            this->solution_matrix->get(1, this->iteration),
            this->solution_matrix->get(2, this->iteration));
        for (int i = 1; i < this->space; ++i)
        {
            // TODO: Make it so that the solution matrix is independent
#pragma omp task
            {
                for (int k = -1; k < 2; ++k)
                {
                    locality->push(this->get_time_init(k + i, this->left, this->right, this->space));
                }
                this->solution_matrix->set(i, this->iteration, this->get_time_init(this->iteration, this->left, this->right, this->space));
                this->solution_matrix->set(i, this->next(), this->calculate_next(locality));
                if (this->stop())
                {
                    this->iteration();
                }
            }
        }
    }
}

template <class T>
long long ParallelBar<T>::next()
{
    return ++this->iteration;
}

template <class T>
long long ParallelBar<T>::previous()
{
    return --this->iteration;
}

template <class T>
T ParallelBar<T>::calculate_next(Locality<T> *locality)
{
    return locality->current() + (locality->previous() - 2 * (locality->current()) - locality->next()) * this->constant;
}

template <class T>
bool ParallelBar<T>::stop()
{
    return this->iteration < 1000000;
}