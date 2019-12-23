#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <string>
#include <future>

using namespace std;

const size_t typesize = sizeof(uint64_t);
const string orig_file_name("orig.bin");
const size_t buf_size = 100000;

mutex m;

int comp(const void* a_, const void* b_) 
{
    const uint64_t *a = static_cast<const uint64_t *>(a_);
    const uint64_t *b = static_cast<const uint64_t *>(b_);
    
    if (*a < *b) 
        return -1;
    else if (*a > *b)
        return 1;
    else 
        return 0;
} 

size_t getFileSizeInBytes(string file_name)
{
    ifstream file(file_name, ifstream::binary | ifstream::in);
    file.seekg(0, ios::end);
    size_t res = file.tellg();
    
    return res;
}

//merge sort of two files to the third one
void merge(string file1_name, string file2_name, string out_name)
{
    ifstream file1(file1_name, ifstream::binary | ifstream::in);
    ifstream file2(file2_name, ifstream::binary | ifstream::in);
    
    ofstream out_file(out_name, fstream::binary | fstream::out); 
    
    uint64_t a, b;
    
    file1.read(reinterpret_cast<char*>(&a), typesize); 
    file2.read(reinterpret_cast<char*>(&b), typesize);
               
    while(file1.gcount() && file2.gcount()) {

        if (a < b) {
            out_file.write(reinterpret_cast<char*>(&a), typesize);
            file1.read(reinterpret_cast<char*>(&a), typesize);
        } else {
            out_file.write(reinterpret_cast<char*>(&b), typesize);
            file2.read(reinterpret_cast<char*>(&b), typesize);
        }
    }
    
    while (file1.gcount()) {
        out_file.write(reinterpret_cast<char*>(&a), typesize);
        file1.read(reinterpret_cast<char*>(&a), typesize);
    }   
    
    while (file2.gcount()) {
        out_file.write(reinterpret_cast<char*>(&b), typesize);
        file2.read(reinterpret_cast<char*>(&b), typesize);
    } 
    
}
            

string process(string filename)
{
    ifstream file(filename, ifstream::binary | ifstream::in);
    uint64_t buf[buf_size];
    
    string thr_num = filename.substr(0, 1);
    
    size_t numbers_left = getFileSizeInBytes(filename)/typesize;
    size_t read_size;
    
    string file1_name = "thr" + thr_num + "_main_0.bin";
    string file2_name = "thr" + thr_num + "_new.bin";
    string new_file1_name;
    
    //Debug info print
    /*
    {
        unique_lock<mutex> lock(m);
        cout<<"\nfilename: "<<filename<<endl;
        cout<<"size: "<<getFileSizeInBytes(filename)/typesize<<endl;
        cout<<"thr "<<thr_num<<", numbers_left: "<<numbers_left<<", buf_size: "<<buf_size<<endl;
    }  
    */
    
    //file1 - the main file, collects sorted sequence 
    read_size = min(numbers_left, buf_size);
    numbers_left -= read_size;
     
    file.read(reinterpret_cast<char*>(buf), read_size * typesize);
    qsort(buf, read_size, typesize, comp);
    
    ofstream file1(file1_name, fstream::binary | fstream::out); 
    file1.write(reinterpret_cast<char*>(buf), read_size * typesize);
    file1.close();
    
    
    size_t cnt = 0;
    //file2 - sorted sequence of buf_size, which is merging with file1
    while (numbers_left) {
        read_size = min(numbers_left, buf_size);
        numbers_left -= read_size;
        
        file.read(reinterpret_cast<char*>(buf), read_size * typesize);
        qsort(buf, read_size, typesize, comp);
      
        ofstream file2(file2_name, fstream::binary | fstream::out); 
        file2.write(reinterpret_cast<char*>(buf), read_size * typesize);
        file2.close();
        
        
        //merging
        new_file1_name =  "thr" + thr_num + "_main_" + to_string(cnt%2)+".bin";
        merge(file1_name, file2_name, new_file1_name);
        
        file1_name = new_file1_name;
        
        cnt++;
    }
    
    return file1_name;
}        
    
   
int main()
{
    ifstream orig_file(orig_file_name, ifstream::binary | ifstream::in);
    
    size_t fileSize = getFileSizeInBytes(orig_file_name);
    if (fileSize % typesize) {
        cout<<"ERROR: bad file"<<endl;
        return 0;
    }
    fileSize /= typesize;
    
    //cout<<"fileSize: "<<fileSize<<endl;
    
    
    uint64_t val;
    
    //divide file into two files, which will be processed separately
    ofstream file1("1.bin", fstream::binary | fstream::out);
    for (size_t i = 0; i < fileSize/2; ++i) {
        orig_file.read(reinterpret_cast<char*>(&val), typesize);
        file1.write(reinterpret_cast<char*>(&val), typesize);
    }
    
    ofstream file2("2.bin", fstream::binary | fstream::out); 
    for (size_t i = fileSize/2; i < fileSize; ++i) {
        orig_file.read(reinterpret_cast<char*>(&val), typesize);
        file2.write(reinterpret_cast<char*>(&val), typesize);
    }
    
    file1.close();
    file2.close();
    
    future<string> f1 = async(launch::async, process, "1.bin");
    future<string> f2 = async(launch::async, process, "2.bin");
    
    string filename1 = f1.get();
    string filename2 = f2.get();
    
    merge(filename1, filename2, "sorted.bin");
    
    return 0;
}
    
    
