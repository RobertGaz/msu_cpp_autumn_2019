#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

const size_t typesize = sizeof(uint64_t);
const string orig_file_name("orig.bin");    
    
int main()
{
    {
        ofstream file(orig_file_name, ifstream::binary | ifstream::out);
    
        for (uint64_t i = 0; i < 1000000; i++) {
            uint64_t val = rand()%5000 ;
            file.write(reinterpret_cast<char*>(&val), typesize);
        }
    }
    
    return 0;
}



