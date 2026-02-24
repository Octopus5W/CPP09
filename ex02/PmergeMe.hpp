#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <list>
#include <vector>
#include <deque>
#include <iostream>
#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstdlib>

class PmergeMe
{
	private:
		std::list<int> 				_list;
		std::list<std::pair<int, int> > 	_list_pairs;
		std::vector<int> 			_vector;
		std::vector<std::pair<int, int> > 	_vector_pairs;
		int 					_int_odd;
		PmergeMe();
		~PmergeMe();
		const PmergeMe &operator=(const PmergeMe &other);
		PmergeMe(const PmergeMe &other);
		
		bool parseArgs(int ac, char **av); 

		void sort_pair(std::list<int>& l, std::list<int>::iterator it);
		void sort(std::list<int>& l);
		void print(const std::list<int>& l);

		void sort_pair(std::vector<int>& v, std::vector<int>::iterator it);
		void sort(std::vector<int>& v);
		void print(const std::vector<int>& v);
	public:
		static void run(int ac, char **av);



};
#endif
