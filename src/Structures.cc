#include "Structures.h"

template <class T>
Matrix<T>::Matrix(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    data = new T[rows * cols];
    for (int i = 0; i < rows * cols; i++)
    {
        data[i] = 0;
    }
}

template <class T>
Matrix<T>::~Matrix()
{
    delete[] data;
}
template <class T>
T Matrix<T>::get(int row, int col)
{
    return data[row * cols + col];
}

template <class T>
T Matrix<T>::set(int row, int col, T value)
{
    data[row * cols + col] = value;
}

template <class T>
T Locality<T>::previous()
{
    return data[0];
}

template <class T>
T Locality<T>::current()
{
    return data[1];
}

template <class T>
T Locality<T>::next()
{
    return data[2];
}

template <class T>
Locality<T>::Locality(int previous, int current, int next)
{
    data[0] = previous;
    data[1] = current;
    data[2] = next;
}

template <class T>
Locality<T>::Locality()
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
}

template <class T>
void Locality<T>::push(T new_value)
{
    data[0] = data[1];
    data[1] = data[2];
    data[2] = new_value;
}