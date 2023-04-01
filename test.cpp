#include <iostream>
#include <stdarg.h>
#include <sstream>

using std::string;

	template <typename T>
	std::string to_string(const T &t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	template <typename T>
	std::string concat_helper(const T &t)
	{
		return to_string(t);
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
	template <typename T, typename... Args>
	std::string concat_helper(const T &t, const Args &...args)
	{
		return append(to_string(t), concat_helper(args...));
	}

	template <typename... Args>
	std::string pathAppend(const Args &...args)
	{
		return concat_helper(args...);
	}

int	main(void)
{
	string root1("root");
	string root2("root/");
	string path1("path/");
	string path2("/path");
	// std::cout << urlAppend(root1, path1) << std::endl;
	std::cout << pathAppend(root2, path2) << std::endl;
}