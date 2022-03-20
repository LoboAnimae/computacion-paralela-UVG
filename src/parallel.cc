#include "parallel.h"
#include "Structures.h"
#include <omp.h>

ParallelBar::ParallelBar(long long iteration, double left, double right, double constant, int space, int time, int threads)
{
    this->iteration = iteration;
    this->left = left;
    this->right = right;
    this->constant = constant;
    this->space = space;
    this->time = time;
    this->thread_count = threads;
    this->solution_matrix = new Matrix(space, time);
}

double ParallelBar::get_time_init(int iteration, double left, double right, int max)
{
    return (left - right) / (iteration * max);
}

void ParallelBar::run()
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
        Locality *locality = new Locality(
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
                    this->run();
                }
            }
        }
    }
}

long long ParallelBar::next()
{
    return ++this->iteration;
}

long long ParallelBar::previous()
{
    return --this->iteration;
}

double ParallelBar::calculate_next(Locality *locality)
{
    return locality->current() + (locality->previous() - 2 * (locality->current()) - locality->next()) * this->constant;
}

bool ParallelBar::stop()
{
    return this->iteration < 1000000;
}