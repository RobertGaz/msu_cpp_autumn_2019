#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <cassert>

class Row
{
    int* row;
    std::size_t n_columns;
    
public:
    Row(int *arr, std::size_t len);
    
    int& operator[](std::size_t col);
};

class Matrix
{
    std::size_t n_rows;
    std::size_t n_columns;
    int **M;
    
public:
    Matrix(std::size_t rows, std::size_t cols);
    
    ~Matrix();
    
    size_t getRows() const;
    
    size_t getColumns() const;
            
    
    Row operator[](std::size_t row);
    
    Matrix& operator*=(int a);
    
    bool operator==(Matrix& b) const;
    
    bool operator!=(Matrix& b) const;
    
    void print() const; 
    
};
