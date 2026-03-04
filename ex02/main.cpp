#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}
	return (PmergeMe::run(argc, argv) ? 0 : 1);
}
