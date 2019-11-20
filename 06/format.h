#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <sstream>

using namespace std;

const string err_str_1("incorrect arg num");
const string err_str_2("arg num > args amnt");

void get_args(vector<string>& vec);


template <class T, class ... Types>
void get_args(vector<string>& vec, T arg, Types ... args)
{	
	stringstream s;
	s << arg;
	vec.push_back(s.str());
	get_args(vec, args...);
}

size_t get_num(string str, string::iterator& it);
		

template <class ... Types>
string format(string main_str, Types ... args) 
{
	vector <string> vec;
	get_args(vec, args...);	
	
	string res_str;
	string::iterator it = main_str.begin();
	size_t arg_num;
	
	while (it < main_str.end()) {
	
		if (*it == '}') {
			throw runtime_error(err_str_1);
		} else if (*it == '{') {
			arg_num = get_num(main_str, it);
			if (arg_num >= vec.size())	
				throw runtime_error(err_str_2);
				
			res_str.append(vec[arg_num]);
			
		} else {
			res_str.append(1, *it);
			++it;
		}
	}
	
	return res_str; 
}


