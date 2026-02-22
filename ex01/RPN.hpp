#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class RPN
{
	private:
		RPN();
		RPN(const RPN &copy);
		RPN &operator=(const RPN &copy);
		~RPN();
	public:
		static int operation(const std::string &input);
};

#endif
