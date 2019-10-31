#include <iostream>
#include <string>
#include <stdexcept>

const std::string err_str("BAD INPUT");

bool is_operation(char c) 
{
    return c == '*' || c == '/' || c == '+' || c == '-'; 
}

bool is_space(char c) 
{
    return c == ' ' || c == '\n';
}

int get_digit(char c) {
    
    if (c > '9' || c < '0')
        return -1;
    
    return c - '0';
}

bool unary_minus(const std::string& str, size_t start_pos)
{
    int pos = start_pos - 1; 
    
    while (pos >= 0) {
        if (get_digit(str[pos]) >= 0) 
            return false;
        if (is_operation(str[pos]))
            return true;
        pos--;
    }
    return true; 
}


int get_number(const std::string& str) 
{   
    std::string::const_iterator it = str.begin();
    
    size_t minus_amnt = 0;
    while (it < str.end()) {
        if (is_space(*it))
            it++;
        else if (*it == '-') {
            minus_amnt++;
            it++;
        } else {
            break;
        }
    }
    
    if (it == str.end()) 
        throw std::invalid_argument(err_str);
    
    int result = 0;
    bool number = false;
    int digit;
    while (it < str.end() && (digit=get_digit(*it)) >= 0) {
            result *= 10;
            result += digit;
            number = true;
            it++;
    }
    
    if (number) {
    
        while (it < str.end() && is_space(*it)) 
            it++;
    
        if (it == str.end()) {
    
            if (minus_amnt % 2) 
                result *= -1;
                
        } else {
        
            throw std::invalid_argument(err_str);
        }
        
    } else {
    
        throw std::invalid_argument(err_str);
    }
        
    return result;
}
  
 
int down(const std::string& str)
{   
    size_t pos;
    if ((pos = str.find('+')) != std::string::npos) {
        return down(str.substr(0, pos)) + down(str.substr((pos+1), str.length()));
    } 
    
    for (pos=0; pos < str.length(); pos++)
        if (str[pos]=='-' && !unary_minus(str, pos)) 
            return down(str.substr(0, pos)) - down(str.substr((pos+1), str.length()));
    
    if ((pos = str.find('*')) != std::string::npos) {
        return down(str.substr(0, pos)) * down(str.substr((pos+1), str.length()));
    }
    
    if ((pos = str.find('/')) != std::string::npos) {
        return down(str.substr(0, pos)) / down(str.substr((pos+1), str.length()));
    } 
    
    return get_number(str);
} 

int main(int argc, char* argv[])
{  
    if (argc == 2) {
        int result;
        
        try {
            result = down(argv[1]);
            std::cout<< result <<std::endl;
            return 0;
        } catch(std::exception& exc) {
            std::cout<< err_str <<std::endl;
            return -1;
        }
    } 
    
    std::cout << err_str << std::endl;
    return -1;
}
