#include "format.h"

void get_args(vector<string>& vec) {}

size_t get_num(const string& str, string::const_iterator& it) 
{
    ++it;
    if (it == str.cend())
        throw runtime_error(err_str_1);

    size_t res = 0;
	while (*it != '}') {
        if (it == str.end() or *it < '0' or *it > '9') 
    	    throw runtime_error(err_str_1);

        res *= 10;
        res += (*it - '0');
        ++it;
    }
    ++it;

    return res;
}
