    #include "bigint.h"
    
    size_t BigInt::get_len(uint64_t num) const {
        size_t res = 0; 
        
        do {
            res++;
            num /= 10;
        } while (num);
        
        return res;
    }  
    
    void BigInt::alloc_buf(size_t size = 10) {
        while (buf_size < size)
            buf_size *= 2;
            
        data = static_cast<char*>(malloc(buf_size));
        
        if (!data)
            throw std::bad_alloc();
    }
    
    void BigInt::expand_buf(size_t new_size) {
        if (new_size < buf_size) 
            return;
            
        while (buf_size <= new_size)
            buf_size *= 2;
        
        void* new_mem;
        
        if (new_mem = std::realloc(data, buf_size))
            data = static_cast<char*>(new_mem);
        else 
            throw std::bad_alloc();
    }
        
    void BigInt::change_sign() {
        if (is_zero() or sign == '-')
            sign = '+';
        else 
            sign = '-';
         
    }
    
    bool BigInt::is_zero() const {
        for (size_t i = 0; i < len; i++)
            if (data[i] - '0')
                return false;
                
        return true;
    }
    
    
    BigInt::BigInt() : sign('+'), len(1), buf_size(10) {
        alloc_buf();
        *data = '0';
    }
       
    BigInt::BigInt(const int64_t num) : buf_size(10) {
        if (num < 0)
            sign = '-';
        else
            sign = '+';
        
        uint64_t u_num = std::abs(num);
        len = get_len(u_num);
        
        alloc_buf(len);
        
        char *ptr = data;
        
        do {
            *ptr = (u_num % 10) + '0';
            u_num /= 10;
            ptr++;
        } while (u_num);
    }
    
    BigInt::BigInt(const BigInt& another) : buf_size(10) {
        sign = another.sign;
        len = another.len;
        alloc_buf(len);
        std::memcpy(data, another.data, len);
    }
    
    BigInt::~BigInt() {
        free(data);
    }
    
    
    
    BigInt& BigInt::operator=(const BigInt& another) {
        if (this == &another)    
            return *this;
            
        sign = another.sign;
        len = another.len;
        expand_buf(len);  
        std::memcpy(data, another.data, len);
    }  
    
    bool BigInt::operator==(const BigInt& another) const {
        
        if (sign != another.sign)
            return false;
        
        if (len != another.len)
            return false;
        
        for (size_t i = 0; i < len; i++)
            if (data[i] != another.data[i]) 
                return false;
        
        return true;
    }
    
    bool BigInt::operator!=(const BigInt& another) const {
        return !(*this == another);
    }
    
    bool BigInt::operator<(const BigInt& another) const {
        if (sign == '-' and another.sign == '+')
            return true;
        
        if (sign == '+' and another.sign == '-')
            return false;
        
        if (len < another.len)
            return sign == '+';
                
        if (len > another.len)
            return sign == '-'; 
            
        for (int i = len-1; i >= 0; i--)
            if (data[i] != another.data[i])
                if (sign == '+')
                    return data[i] < another.data[i];
                else
                    return !(data[i] < another.data[i]);
        
        return false;
    }
    
    bool BigInt::operator<=(const BigInt& another) const {
        return (*this < another) || (*this == another);
    }
    
    bool BigInt::operator>(const BigInt& another) const {
        return !(*this <= another);
    }
    
    bool BigInt::operator>=(const BigInt& another) const {
        return !(*this < another);
    }
    
            
    
    
    BigInt BigInt::operator-() const {
         BigInt result(*this);
         result.change_sign();
         
         return result;
    }
    
    BigInt BigInt::operator+(const BigInt& another) const {
        if (sign != another.sign) {
            if (sign == '+')
                return operator-(-another);
            else 
                return another.operator-(-(*this));
        }
        
        const BigInt* first = this;
        const BigInt* second = &another;
        
        if (this -> len < another.len) {
            first = &another;
            second = this;
        }
        
        BigInt result(*first);
        
        char* a = result.data;
        char* b = second->data;
        unsigned c, over = 0;
        size_t a_left = result.len;
        size_t b_left = second->len;
        
        while (a_left and b_left) {
            c = *a - '0' + *b - '0' + over;
            over = c / 10;
            c %= 10;
                
            *a = c + '0';
                
            a++;
            b++;
                
            a_left--;
            b_left--;
        }
            
        while (a_left) {
            c = *a - '0' + over;
            over = c / 10;
            c %= 10;
                
            *a = c + '0'; 
            a++;
            a_left--;
        }
            
        if (over) {
            result.len++;
            result.expand_buf(result.len);
            
            *a = over + '0';
        }
        
        return result;
        
    }
    
    BigInt BigInt::operator-(const BigInt& another) const {
        
        if (sign != another.sign) {
            return operator+(-another);
        }
        
        bool result_change_sign = false;
        
        const BigInt* first = this;
        const BigInt* second = &another;
        
        if (sign == '+' and *this < another or sign == '-' and *this > another) {
            first = &another;
            second = this;
            result_change_sign = true;  
            
        }
        BigInt result(*first);
        if (result_change_sign)
            result.change_sign();
        
        
        char* a = result.data;
        char* b = second->data;
        int c;
        unsigned over = 0;
        size_t a_left = result.len;
        size_t b_left = second->len;
        
        while (a_left and b_left) {
        
            c = *a - *b - over;
            if (c < 0) {
                c += 10;
                over = 1;
            } else { 
                over = 0;
            }
                
            *a = c + '0';
                
            a++;
            b++;
                
            a_left--;
            b_left--;
        }
        
        while (a_left) {
        
            c = *a - '0' - over;
            if (c < 0) {
                c += 10;
                over = 1;
            } else { 
                over = 0;
            }
                
            *a = c + '0';
                
            a++;
            a_left--;
        }
        
        
        a--;
        while (result.len && *a == '0') {
            a--;
            result.len--;
        }
        
        //zero case
        if (result.len == 0) {
            result.len = 1;
            result.sign = '+';
        }
        
        return result;          
    }   
    

            
            
    std::ostream& operator<<(std::ostream& out, const BigInt& obj) {
        if (obj.sign == '-')
            out << '-';
        for (int i = obj.len - 1; i >= 0; i--)
            out << obj.data[i];
        
        return out;
    }     

