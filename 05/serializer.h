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
    Error operator()(Types... args) {
        return process(args...);
    }
    
private:
    static const char separator = ' ';
    std::ostream& out;
    
    Error serialize_value(uint64_t val);
    Error serialize_value(bool val);
    
    Error process();
    
    template <class T, class ... Types>
    Error process(T val, Types ... args) {
        if (serialize_value(val) != NoError)
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
    std::string buf;
    static const char separator = ' ';
    std::istream& in;
    
    Error deserialize_value(bool& val);
    
    Error deserialize_value(uint64_t& val);
    
    Error process();
    
    template <class T, class... Types>
    Error process(T& val, Types& ... args) 
    {
        if (deserialize_value(val) != NoError)
	        return CorruptedArchive;
            
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


    
