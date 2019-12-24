#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <string>
#include <future>
#include <stdexcept>
#include <cstdio>

using namespace std;

const size_t typesize = sizeof(uint64_t);
const string orig_file_name("orig.bin");
const string sorted_file_name("sorted.bin");
const size_t buf_size = 100000;

const string open_err_str = "Can't open the file ";
const string bad_data_err = "Incorrect data in the file ";

size_t getFileSizeInBytes(const string& file_name)
{
    ifstream file(file_name, ifstream::binary | ifstream::in);
    if (!file.is_open())
        throw runtime_error(open_err_str + file_name);
    file.seekg(0, ios::end);
    size_t res = file.tellg();
    
    return res;
}

//merge sort of two files to the third one
void merge(const string& file1_name, const string& file2_name, const string& out_name)
{
    ifstream file1(file1_name, ifstream::binary | ifstream::in);
    if (!file1.is_open())
        throw runtime_error(open_err_str + file1_name);
        
    ifstream file2(file2_name, ifstream::binary | ifstream::in);
    if (!file2.is_open())
        throw runtime_error(open_err_str + file2_name);
        
    ofstream out_file(out_name, fstream::binary | fstream::out); 
    if (!out_file.is_open())
        throw runtime_error(open_err_str + out_name);
        
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
    
    remove(file1_name.c_str());
    remove(file2_name.c_str());
    
}
            

string process(const string& filename)
{
    ifstream file(filename, ifstream::binary | ifstream::in);
    if (!file.is_open())
        throw runtime_error(open_err_str + filename);
    
    unique_ptr<uint64_t[]> buffer(new uint64_t[buf_size]);
    uint64_t* buf = buffer.get();
    
    string thr_num = filename.substr(0, 1);
    
    size_t numbers_left = getFileSizeInBytes(filename)/typesize;
    size_t read_size;
    
    string file1_name = "thr" + thr_num + "_file1_0.bin";
    const string file2_name = "thr" + thr_num + "_file2.bin";
    string new_file1_name;
    string file_buf_name;
    
    
   
    // file1 - the main file, collects total sorted sequence 
    // file2 - sorted sequence of buf_size
    // every file2 mergs with file1
    
    size_t cnt = 0; 
    while (numbers_left) {
        read_size = min(numbers_left, buf_size);
        numbers_left -= read_size;
        
        file.read(reinterpret_cast<char*>(buf), read_size * typesize);
        std::sort(buf, buf + read_size);
        
        if (cnt) 
            file_buf_name = file2_name;
        else 
            file_buf_name = file1_name;
            
        ofstream file_buf(file_buf_name, fstream::binary | fstream::out); 
        if (!file_buf.is_open())
            throw runtime_error(open_err_str + file_buf_name);
        
        file_buf.write(reinterpret_cast<char*>(buf), read_size * typesize);
        file_buf.close();
        
        if (cnt) {
            // merging
            new_file1_name =  "thr" + thr_num + "_file1_" + to_string(cnt%2)+".bin";
            merge(file1_name, file2_name, new_file1_name);
            
            file1_name = new_file1_name;
        }
        
        cnt++;
    }
    
    return file1_name;
}        
    
   
int main()
{
    try {
    
        ifstream orig_file(orig_file_name, ifstream::binary | ifstream::in);
        if (!orig_file.is_open())
            throw runtime_error(open_err_str + orig_file_name);
        
        size_t fileSize = getFileSizeInBytes(orig_file_name);
        if (fileSize % typesize) 
            throw runtime_error(bad_data_err + orig_file_name);
            
        fileSize /= typesize;
        
        
        uint64_t val;
        
        //divide file into two files, which will be processed separately
        ofstream file1("1.bin", fstream::binary | fstream::out);
        if (!file1.is_open())
            throw runtime_error(open_err_str + "1.bin");
            
        for (size_t i = 0; i < fileSize/2; ++i) {
            orig_file.read(reinterpret_cast<char*>(&val), typesize);
            file1.write(reinterpret_cast<char*>(&val), typesize);
        }
        
        
        ofstream file2("2.bin", fstream::binary | fstream::out);
        if (!file2.is_open())
            throw runtime_error(open_err_str + "2.bin");
             
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
        
        remove("1.bin");
        remove("2.bin");
        
        merge(filename1, filename2, sorted_file_name);
        
    } catch (exception& exc) {
        cout << "Error! "<< exc.what() << endl; 
        return -1;
    }
    
    return 0;
}
    
    
