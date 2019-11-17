#include "serializer.h"

Serializer::Serializer(std::ostream& out_stream) : out(out_stream) {}
Error Serializer::process(bool val) {
    if (val) out << "true" << separator;
    else out << "false" << separator;
    
    return NoError;
}
 
Deserializer::Deserializer(std::istream& in_stream) : in(in_stream) {} 

void Deserializer::flush_buf() {
    for (unsigned i = 0; i < bufsize; ++i) 
        buf[i] = '\0';
}     
    
    
Error Deserializer::process() {
    in.getline(&buf[0], 20, separator);
    if (std::string(&buf[0]) != "")
        return CorruptedArchive;
    return NoError;
}
    
void Data::print() {
    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << (b ? "true" : "false") << std::endl;
    std::cout << "c: " << c << std::endl; 
}
    

