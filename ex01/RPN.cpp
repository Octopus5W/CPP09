#include "RPN.hpp"

#include <cstdlib>
#include <sstream>
#include <stack>


RPN::RPN() {}

RPN::RPN(const RPN &copy)
{
	(void)copy;
}

RPN &RPN::operator=(const RPN &copy)
{
	(void)copy;
	return (*this);
}

RPN::~RPN() {}

bool isOperator(const std::string &token)
{
	return (token == "+" || token == "-" || token == "*" || token == "/");
}

bool isSingleDigit(const std::string &token)
{
	return (token.length() == 1 && std::isdigit(token[0]));
}

int RPN::operation(const std::string &input) 
{
	std::stringstream ss(input);
	std::string token;
	std::stack<int> numbers;

	while (ss >> token)
	{
		if (isSingleDigit(token))
		{
			numbers.push(std::atoi(token.c_str()));
			continue;
		}
		if (!isOperator(token) || numbers.size() < 2)
			throw std::runtime_error("Error: syntax");

		int rhs = numbers.top();
		numbers.pop();
		int lhs = numbers.top();
		numbers.pop();

		if (token == "+")
			numbers.push(lhs + rhs);
		else if (token == "-")
			numbers.push(lhs - rhs);
		else if (token == "*")
			numbers.push(lhs * rhs);
		else
		{
			if (rhs == 0)
				throw std::runtime_error("Error: zero division not allowed");
			numbers.push(lhs / rhs);
		}
	}

	if (numbers.size() != 1)
		throw std::runtime_error("Error: syntax");
	return (numbers.top());
}
