#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <iostream>

namespace ft
{
	template <typename T>
	std::string to_string(const T &t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	std::string append(std::string T1, std::string T2)
	{
		if (T1.back() == '/')
		{
			if (T2.front() == '/')
        		T1 += T2.substr(1, T2.size() - 1);
			else
				T1 += T2;
		}
		else
		{
			if (T2.front() == '/')
        		T1 += T2;
			else
				T1.append("/" + T2);
		}
		return T1;

	}
	template <typename T>
	std::string append_helper(const T &t1, const T &t2)
	{
		return append(to_string(t1), to_string(t2));
	}
	
	template <typename T>
	std::string pathAppend(const T &t1, const T &t2)
	{
		return append_helper(t1, t2);
	}

	template <typename T>
	std::string pathAppend(const T &t1, const T &t2, const T &t3)
	{
		return append_helper(append_helper(t1, t2), t3);
	}


} // namespace ft

#endif
