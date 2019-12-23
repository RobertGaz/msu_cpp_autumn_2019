#include <iostream>
#include <fstream>
#include <cstdint>
#include <cassert>

using namespace std;

const size_t typesize = sizeof(uint64_t);
const string orig_file_name("sorted.bin");    
    
int main()
{
    ifstream file(orig_file_name, ifstream::binary | ifstream::in);
    
    uint64_t prev_val, val;
    
    file.read(reinterpret_cast<char*>(&prev_val), typesize);
    
    while(true) {
        file.read(reinterpret_cast<char*>(&val), typesize);
        if (file.gcount() ==0) 
            break;
        assert(prev_val <= val);
        prev_val = val;
    }
    
    return 0;
}
