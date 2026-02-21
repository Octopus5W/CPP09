#include "BitcoinExchange.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &copy)
{
	*this = copy;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &assign)
{
	if (this != &assign)
		_data = assign._data;
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

bool isLeapYear(int year)
{
	return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool isValidDate(const std::string &date)
{
	if (date.length() != 10)
		return (false);
	if (date[4] != '-' || date[7] != '-')
		return (false);
	for (size_t i = 0; i < date.length(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (year < 0 || month < 1 || month > 12 || day < 1)
		return (false);

	static const int daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,31};
	
	int maxDay = daysPerMonth[month - 1];
	if (month == 2 && isLeapYear(year))
		maxDay = 29;
	return (day <= maxDay);
}

std::string trim(const std::string &str)
{
	size_t start = 0;
	size_t end = str.length();
	while (start < end && std::isspace(static_cast<unsigned char>(str[start])))
		++start;
	while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
		--end;
	return (str.substr(start, end - start));
}

bool parseDouble(const std::string &raw, double &out)
{
	char *endPtr = NULL;
	out = std::strtod(raw.c_str(), &endPtr);
	if (raw.empty() || endPtr == raw.c_str())
		return (false);
	while (*endPtr)
	{
		if (!std::isspace(static_cast<unsigned char>(*endPtr)))
			return (false);
		++endPtr;
	}
	return (true);
}

void BitcoinExchange::readData()
{
	std::ifstream file("data.csv");
	if (!file.is_open())
	{
		std::cerr << "Error: cant open data file." << std::endl;
		return;
	}

	std::string line;
	if (!std::getline(file, line))
		return;

	while (std::getline(file, line))
	{
		std::string date;
		std::string priceStr;
		double price;

		size_t sepPos = line.find(',');
		if (sepPos != std::string::npos)
		{
			date = line.substr(0, sepPos);
			priceStr = line.substr(sepPos + 1);
		}
		else
		{
			sepPos = line.find('|');
			if (sepPos == std::string::npos)
				continue;
			date = line.substr(0, sepPos);
			priceStr = line.substr(sepPos + 1);
		}

		date = trim(date);
		priceStr = trim(priceStr);
		if (!isValidDate(date) || !parseDouble(priceStr, price))
			continue;

		_data[date] = price;
	}
}

void BitcoinExchange::readInput(const std::string &inputFile)
{
	if (_data.empty())
	{
		std::cerr << "Error: input file is empty." << std::endl;
		return;
	}

	std::ifstream file(inputFile.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: cant open input file." << std::endl;
		return;
	}

	std::string line;
	if (!std::getline(file, line))
		return;

	while (std::getline(file, line))
	{
		size_t sepPos = line.find('|');
		if (sepPos == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = trim(line.substr(0, sepPos));
		std::string valueStr = trim(line.substr(sepPos + 1));
		double value;

		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}
		if (!parseDouble(valueStr, value))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (value < 0)
		{
			std::cerr << "Error: negative number not allowed." << std::endl;
			continue;
		}
		if (value > 1000)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}

		std::map<std::string, double>::const_iterator it = _data.lower_bound(date);
		if (it == _data.end() || it->first != date)
		{
			if (it == _data.begin())
			{
				std::cerr << "Error: bad input => " << date << std::endl;
				continue;
			}
			--it;
		}
		std::cout << date << " => " << value << " = " << (value * it->second) << std::endl;
	}
}
