#include "matrix.h"

int main() 
{
    const std::size_t rows = 5;
    const std::size_t cols = 3;
    
    Matrix a(rows, cols);
    Matrix b(rows, cols);

    assert(a.getRows() == 5);
    assert(a.getColumns() == 3);
    
    a[1][2] = 5;
    b[1][2] = 5;
    
    assert(a == b);
    assert(!(a != b));
    
    a *= 7;
    
    assert(a != b);
    assert(!(a == b));
    
    a.print();
    b.print();
    return 0;
}
