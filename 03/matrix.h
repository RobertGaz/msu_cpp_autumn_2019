#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <cassert>

class Row
{
    int* row;
    size_t n_columns;
    
public:
    Row(int *arr, size_t len);
    
    int& operator[](size_t col);
    const int& operator[](size_t col) const;
};

class Matrix
{
    size_t n_rows;
    size_t n_columns;
    int **M;
    
public:
    Matrix(size_t rows, size_t cols);
    
    ~Matrix();
    
    size_t getRows() const;
    
    size_t getColumns() const;
            
    
    Row operator[](size_t row);
    const Row operator[](size_t row) const;
    
    Matrix& operator*=(int a);
    
    bool operator==(Matrix& b) const;
    
    bool operator!=(Matrix& b) const;
    
    void print() const; 
    
};
