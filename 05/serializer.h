#include <iostream>
#include <cstdint>
#include <sstream>
#include <array>
#include <cassert>
#include <stdexcept>
#include <string>

enum Error 
{
    NoError,
    CorruptedArchive
};

class Serializer 
{
public:
    Serializer(std::ostream& out_stream);
    
    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }
    
    template <class... Types>
    Error operator()(Types&... args) {
        return process(args...);
    }
    
private:
    static const char separator = ' ';
    std::ostream& out;
    
    template <class T>
    Error process(T val) {
        out<<val<<' ';
        return NoError;
    }
    
    Error process(bool val);
    
    template <class T, class ... Types>
    Error process(T& val, Types& ... args) {
        if (process(val) != NoError)
            return CorruptedArchive;
            
        return process(args...);
    }
};

class Deserializer
{

public:
    Deserializer(std::istream& in_stream);
    
    template <class T>
    Error load(T& object) {
        return object.deserialize(*this);
    }
    
    template <class... Types>
    Error operator()(Types& ...args) {
        return process(args...);
    }
    
private:
    static const unsigned char bufsize = 20;
    static const char separator = ' ';
    std::istream& in;
    std::array<char, 20> buf;

    void flush_buf();
    
    
    Error process();
    
    template <class... Types>
    Error process(bool& val, Types& ... args) {
        in.getline(&buf[0], 20, separator);
        if (std::string(&buf[0]) == "")
            return CorruptedArchive;
        if (std::string(&buf[0]) == "true")
            val = true;
        else if (std::string(&buf[0]) == "false")
            val = false;
        else 
            return CorruptedArchive;
        
        flush_buf();
        return process(args...);
    }
    
    template <class... Types>
    Error process(uint64_t& val, Types& ... args) {
        in.getline(&buf[0], 20, separator);
        
        unsigned num;
        uint64_t res = 0;
                
        unsigned i = 0;
        while (buf[i] != '\0') {
            if (buf[i] < '0' or buf[i] > '9')
                return CorruptedArchive;
            
            num = buf[i] - '0';
            res *= 10;
            res += num;
                
            ++i;
        }
        
        val = res;
        
        flush_buf();
        return process(args...);
    }
    
};

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;
    
    void print();
    
    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
    
    
    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
    
};


    
