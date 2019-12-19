#include <cstdlib>

class LinearAllocator
{
public:
    LinearAllocator(size_t maxSize);
    
    ~LinearAllocator();
    
    char* alloc(size_t size);
        
    void reset();

private:
    const size_t max_size;
    size_t cur_size;
    char const * data;
};

