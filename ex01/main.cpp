#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error : syntax => " << argv[0] << " \"operation\"" << std::endl;
		return (1);
	}

	try
	{
		std::cout << RPN::operation(argv[1]) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr  << e.what() << std::endl;
		return (1);
	}
	return (0);
}
