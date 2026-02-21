#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error syntax: ./btc input_file" << std::endl;
		return (1);
	}
	BitcoinExchange btc;
	btc.readData();
	btc.readInput(argv[1]);
	return (0);
}
