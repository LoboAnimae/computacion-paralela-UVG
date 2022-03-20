#ifndef STRUCTURES_PROJ_H
#define STRUCTURES_PROJ_H

template <class T>
class Matrix
{
public:
    Matrix(int, int);
    ~Matrix();
    T *data;
    int rows;
    int cols;
    T get(int, int);
    T set(int, int, T);
};

template <class T>
class Locality
{
    Locality(int, int, int);
    ~Locality();
    Locality();
    T data[3];
    T previous();
    T current();
    T next();
    void push(T);
};
#endif