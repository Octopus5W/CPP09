#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <list>
#include <vector>
#include <string>
#include <utility>
#include <cstddef>

class PmergeMe
{
	private:
		PmergeMe();
		~PmergeMe();
		const PmergeMe &operator=(const PmergeMe &other);
		PmergeMe(const PmergeMe &other);

		bool parseArgs(int ac, char **av, std::vector<int> &_vector, std::list<int> &_list);

		void fordJohnsonList(std::list<int> &l);

		void fordJohnsonVector(std::vector<int> &v);
		
		void print(const std::vector<int> &v) const;
		void print(const std::list<int> &l) const;


	public:
		static bool run(int ac, char **av);
};

#endif
