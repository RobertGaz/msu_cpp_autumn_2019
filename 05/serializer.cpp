#include "serializer.h"

Serializer::Serializer(std::ostream& out_stream) : out(out_stream) {}

Error Serializer::serialize_value(uint64_t val) {
    out<<val<<separator;
    return NoError;
}
    
Error Serializer::serialize_value(bool val) {
	if (val) out << "true" << separator;
	else out << "false" << separator;
    
	return NoError;
}

Error Serializer::process() 
{
	return NoError;
}

   
 
Deserializer::Deserializer(std::istream& in_stream) : in(in_stream) {} 

Error Deserializer::deserialize_value(bool& val) 
{
	buf = "";
	std::getline(in, buf, separator);
            
    if (buf == "")
        return CorruptedArchive;
    if (buf == "true")
        val = true;
    else if (buf == "false")
        val = false;
    else 
        return CorruptedArchive;
        
    return NoError;
}
    
Error Deserializer::deserialize_value(uint64_t& val) 
{	
	buf = "";
	std::getline(in, buf, separator);
        
    unsigned num;
    uint64_t res = 0;
                
    for (size_t i = 0; i < buf.size(); ++i) {
        if (buf[i] < '0' or buf[i] > '9')
            return CorruptedArchive;
            
        num = buf[i] - '0';
        res *= 10;
        res += num;
    }
        
    val = res;
        
    return NoError;
}
 
    
Error Deserializer::process() {
    return NoError;
}
    
void Data::print() {
    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << (b ? "true" : "false") << std::endl;
    std::cout << "c: " << c << std::endl; 
}
    


