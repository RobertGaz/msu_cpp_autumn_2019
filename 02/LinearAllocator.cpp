#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(size_t maxSize) : max_size(maxSize) {
    data = reinterpret_cast<char*>(malloc(max_size));
    if (!data)
        throw std::bad_alloc();
        
    cur_size = 0;
}
    
LinearAllocator::~LinearAllocator() {
    free((char*) data);
}
    
char* LinearAllocator::alloc(size_t size) {
    if (cur_size + size <= max_size) {
        cur_size += size;
        return (char*)data + cur_size;
    } else {
        return nullptr;
    }
}
        
void LinearAllocator::reset() {
    cur_size = 0;
}
