#ifndef STRUCTURES_PROJ_H
#define STRUCTURES_PROJ_H

class Matrix
{
public:
    Matrix(int, int);
    ~Matrix();
    double *data;
    int rows;
    int cols;
    double get(int, int);
    void set(int, int, double);
};

class Locality
{
public:
    Locality(int, int, int);
    ~Locality();
    Locality();
    double data[3];
    double previous();
    double current();
    double next();
    void push(double);
};
#endif