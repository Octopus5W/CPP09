#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

const PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	(void)other;
	return (*this);
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void)other;
}

bool PmergeMe::parseArgs(int ac, char **av)
{
	_int_odd = -1;
	for (int i = 1; i < ac; ++i)
	{
		errno = 0;
		char *end = NULL;
		long n = std::strtol(av[i], &end, 10);
		if (errno == ERANGE || *end != '\0' || n <= 0 || n > INT_MAX)
		{
			std::cerr << "Error: Invalid argument " << av[i] << std::endl;
			return false;
		}
		if (i == ac - 1 && (ac - 1) % 2 != 0)
		{
			_int_odd = n;
			continue;
		}
		_list.push_back(n);
		_vector.push_back(n);
	}
	return true;
}

void PmergeMe::sort_pair(std::list<int>& list, std::list<int>::iterator it)
{
	if (it == list.end())
		return;
	std::list<int>::iterator next_it = it;
	++next_it;
	if (*it >= *next_it)
	{
		_list_pairs.push_back(std::make_pair(*it, *next_it));
		list.erase(next_it);
	}
	else
	{
		_list_pairs.push_back(std::make_pair(*next_it, *it));
		it = list.erase(it);
	}

	sort_pair(list, ++it);
	list.insert(std::lower_bound(list.begin(), list.end(), _list_pairs.back().second), _list_pairs.back().second);
	_list_pairs.pop_back();
};

void PmergeMe::print(const std::list<int>& list)
{
	for (std::list<int>::const_iterator it = list.begin(); it != list.end(); ++it)
	std::cout << *it << " ";
	std::cout << std::endl;
}

void PmergeMe::sort_pair(std::vector<int>& vect, std::vector<int>::iterator it)
{
	if (it == vect.end())
		return;
	std::vector<int>::iterator next_it = it;
	++next_it;

	if (*it >= *next_it)
	{
		_vector_pairs.push_back(std::make_pair(*it, *next_it));
		vect.erase(next_it);
	}
	else
	{
		_vector_pairs.push_back(std::make_pair(*next_it, *it));
		it = vect.erase(it);
	}

	sort_pair(vect, ++it);
	vect.insert(std::lower_bound(vect.begin(), vect.end(), _vector_pairs.back().second), _vector_pairs.back().second);
	_vector_pairs.pop_back();
}

void PmergeMe::print(const std::vector<int>& vect)
{
	for (std::vector<int>::const_iterator it = vect.begin(); it != vect.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

void PmergeMe::run(int ac, char **av)
{
	PmergeMe run;
	if (!run.parseArgs(ac, av))
		return;
	run.sort_pair(run._list, run._list.begin());
	run.sort_pair(run._vector, run._vector.begin());
	if (run._int_odd != -1)
	{
		run._list.insert(std::lower_bound(run._list.begin(), run._list.end(), run._int_odd), run._int_odd);
		run._vector.insert(std::lower_bound(run._vector.begin(), run._vector.end(), run._int_odd), run._int_odd);
	}
	std::cout << "Sorted list: ";
	run.print(run._list);
	std::cout << "Sorted vector: ";
	run.print(run._vector);
}
