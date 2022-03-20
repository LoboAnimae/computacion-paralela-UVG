#ifndef PARALLEL_PROJ_H
#define PARALLEL_PROJ_H

#include "Structures.h"

template <class T>
class ParallelBar
{
public:
    ParallelBar(long long, double, double, double, int, int, int);
    long long iteration;
    double left, right, constant;
    int space, time;
    Matrix<T> solution_matrix;
    ~ParallelBar();
    long long next();
    long long previous();
    int thread_count;
    T calculate_next(Locality<T> *);
    T get_time_init(int, T, T, int);
    void run();
    bool stop();
};

#endif