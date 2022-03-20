#ifndef PARALLEL_PROJ_H
#define PARALLEL_PROJ_H

#include "Structures.h"

class ParallelBar
{
public:
    ParallelBar(long long, double, double, double, int, int, int);
    long long iteration;
    double left, right, constant;
    int space, time;
    Matrix *solution_matrix;
    ~ParallelBar();
    long long next();
    long long previous();
    int thread_count;
    double calculate_next(Locality *);
    double get_time_init(int, double, double, int);
    void run();
    bool stop();
};

#endif