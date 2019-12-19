#include <cassert>
#include "LinearAllocator.h"

int main(void) {
    LinearAllocator A(10);
    char* mem;
    
    mem = A.alloc(6);
    assert (mem != nullptr);
    
    mem = A.alloc(3);
    assert (mem != nullptr);
    
    mem = A.alloc(2);
    assert (mem == nullptr);
    
    A.reset();
    
    mem = A.alloc(2);
    assert (mem != nullptr);
    
    return 0;
}
    

