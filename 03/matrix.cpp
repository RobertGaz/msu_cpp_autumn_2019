#include "matrix.h"

Row::Row(int *arr, size_t len) : row(arr), n_columns(len) {}

int& Row::operator[](size_t col) {
    if (col >= n_columns)
        throw std::out_of_range("");
        
    return row[col];
} 

const int& Row::operator[](size_t col) const {
    if (col >= n_columns)
        throw std::out_of_range("");
        
    return row[col];
} 
    

Matrix::Matrix(size_t rows, size_t cols) : n_rows(rows), n_columns(cols) {
    M = new int*[n_rows];
    for (int i = 0; i < n_rows; ++i) 
        M[i] = new int[n_columns];
  
}
    
Matrix::~Matrix() {
    for (int i = 0; i < n_rows; ++i) 
        delete [] M[i];
    
    delete [] M;
}
    
size_t Matrix::getRows() const {
    return n_rows;
}
    
size_t Matrix::getColumns() const {
    return n_columns;
}
            
    
Row Matrix::operator[](size_t row) {
    if (row >= n_rows)
        throw std::out_of_range("");
        
    return Row(M[row], n_columns);
} 

const Row Matrix::operator[](size_t row) const {
    if (row >= n_rows)
        throw std::out_of_range("");
        
    return Row(M[row], n_columns);
} 

Matrix& Matrix::operator*=(int a) {
    for (int i = 0; i < n_rows; ++i) 
        for (int j = 0; j < n_columns; ++j)
            M[i][j] = M[i][j] * a;
    return *this;
}      
    
bool Matrix::operator==(Matrix& b) const {
    if (this == &b)
        return true;    
        
    if (n_rows != b.n_rows or n_columns != b.n_columns) 
        return false;
        
    for (int i = 0; i < n_rows; ++i)
       for (int j = 0; j < n_columns; ++j)
            if (M[i][j] != b.M[i][j])
               return false;
        
    return true;
}
    
bool Matrix::operator!=(Matrix& b) const {
    return !(*this == b);
}
    
void Matrix::print() const {
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_columns; ++j)
            std::cout<<M[i][j]<<" ";  
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}     
