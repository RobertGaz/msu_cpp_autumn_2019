#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <stdint.h>

class BigInt
{
    char sign;
    char *data;
    
    size_t len;
    size_t buf_size;

    friend std::ostream& operator<<(std::ostream&, const BigInt&);
    
    size_t get_len(uint64_t num) const;
    
    void alloc_buf(size_t size);
    
    void expand_buf(size_t new_size);
        
    void change_sign();
    
    bool is_zero() const;
    
    
      
public:
    
    BigInt();
       
    BigInt(const int64_t num);
    
    BigInt(const BigInt& another);
    
    ~BigInt();
    
    
    BigInt& operator=(const BigInt& another);
    
    bool operator==(const BigInt& another) const;
    
    bool operator!=(const BigInt& another) const;
    
    bool operator<(const BigInt& another) const;
    
    bool operator<=(const BigInt& another) const;
    
    bool operator>(const BigInt& another) const;
    
    bool operator>=(const BigInt& another) const;
    
            
    
    
    BigInt operator-() const;
    
    BigInt operator+(const BigInt& another) const;
    
    BigInt operator-(const BigInt& another) const;
    
};   

