#include "Structures.h"

Matrix::Matrix(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    data = new double[rows * cols];
    for (int i = 0; i < rows * cols; i++)
    {
        data[i] = 0;
    }
}

Matrix::~Matrix()
{
    delete[] data;
}

double Matrix::get(int row, int col)
{
    return data[row * cols + col];
}

void Matrix::set(int row, int col, double value)
{
    data[row * cols + col] = value;
}

double Locality::previous()
{
    return data[0];
}

double Locality::current()
{
    return data[1];
}

double Locality::next()
{
    return data[2];
}

Locality::Locality(int previous, int current, int next)
{
    data[0] = previous;
    data[1] = current;
    data[2] = next;
}

Locality::Locality()
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
}

void Locality::push(double new_value)
{
    data[0] = data[1];
    data[1] = data[2];
    data[2] = new_value;
}