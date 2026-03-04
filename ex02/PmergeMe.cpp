#include "PmergeMe.hpp"

#include <iostream>
#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <iterator>
#include <vector>
#include <list>
#include <ostream>

PmergeMe::PmergeMe()
{
}

PmergeMe::~PmergeMe()
{
}

const PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	(void)other;
	return (*this);
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void)other;
}

bool PmergeMe::parseArgs(int ac, char **av, std::vector<int> &_vector, std::list<int> &_list)
{
	for (int i = 1; i < ac; ++i)
	{
		for (std::size_t j = 0; av[i][j] != '\0'; ++j)
		{
			if (!std::isdigit(static_cast<unsigned char>(av[i][j])))
			{
				std::cerr << "Error: invalid argument: " << av[i] << std::endl;
				return (false);
			}
		}

		errno = 0;
		char *end = NULL;
		long value = std::strtol(av[i], &end, 10);

		if (errno == ERANGE || *end != '\0' || value <= 0 || value > INT_MAX)
		{
			std::cerr << "Error: invalid argument: " << av[i] << std::endl;
			return (false);
		}

		_vector.push_back(value);
		_list.push_back(value);
	}
	return (!_vector.empty() && !_list.empty());
}

std::list< std::pair<int, int> > makePairs(const std::list<int> &list, std::size_t pairCount)
{
	std::list< std::pair<int, int> > pairs;
	std::list<int>::const_iterator it = list.begin();
	for (std::size_t i = 0; i < pairCount; ++i)
	{
		int left = *it;
		++it;
		int right = *it;
		++it;

		std::pair<int, int> pair;
		if (left > right)
		{
			pair.first = left;
			pair.second = right;
		}
		else
		{
			pair.first = right;
			pair.second = left;
		}
		pairs.push_back(pair);
	}
	return (pairs);
}

std::vector< std::pair<int, int> > makePairs(const std::vector<int> &vector, std::size_t pairCount)
{
	std::vector< std::pair<int, int> > pairs;
	pairs.reserve(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
	{
		int left = vector[i * 2];
		int right = vector[(i * 2) + 1];
		std::pair<int, int> pair;
		if (left > right)
		{
			pair.first = left;
			pair.second = right;
		}
		else
		{
			pair.first = right;
			pair.second = left;
		}
		pairs.push_back(pair);
	}
	return (pairs);
}

std::list< std::pair<int, int> > sortPairs(const std::list<int> &sortedA, const std::list< std::pair<int, int> > &pairs)
{
	std::list< std::pair<int, int> > sortedB;
	for (std::list<int>::const_iterator it = sortedA.begin(); it != sortedA.end(); ++it)
	{
		for (std::list< std::pair<int, int> >::const_iterator pairIt = pairs.begin(); pairIt != pairs.end(); ++pairIt)
		{
			if (pairIt->first == *it)
			{
				sortedB.push_back(*pairIt);
				break;
			}
		}
	}
	return (sortedB);
}

std::vector< std::pair<int, int> > sortPairs(const std::vector<int> &sortedA, const std::vector< std::pair<int, int> > &pairs)
{
	std::vector< std::pair<int, int> > sortedB;
	std::vector<bool> used(pairs.size(), false);
	sortedB.reserve(pairs.size());
	for (std::size_t i = 0; i < sortedA.size(); ++i)
	{
		for (std::size_t j = 0; j < pairs.size(); ++j)
		{
			if (!used[j] && pairs[j].first == sortedA[i])
			{
				sortedB.push_back(pairs[j]);
				used[j] = true;
				break;
			}
		}
	}
	return (sortedB);
}

void set_main(std::list<int> &main, std::list<int> &a_i, std::list< std::pair<int, int> > &sortedB)
{
	main.push_back(sortedB.begin()->second);
	a_i.push_back(-1);
	std::size_t current_a_i = 0;
	for (std::list< std::pair<int, int> >::const_iterator it = sortedB.begin(); it != sortedB.end(); ++it, ++current_a_i)
	{
		main.push_back(it->first);
		a_i.push_back(current_a_i);
	}
}

void set_main(std::vector<int> &main, std::vector<int> &a_i, const std::vector< std::pair<int, int> > &sortedB, std::size_t initialSize)
{
	main.reserve(initialSize);
	a_i.reserve(initialSize);
	main.push_back(sortedB[0].second);
	a_i.push_back(-1);
	for (std::size_t i = 0; i < sortedB.size(); ++i)
	{
		main.push_back(sortedB[i].first);
		a_i.push_back(static_cast<int>(i));
	}
}

void set_pend(std::list<int> &pend, std::list<int> &p_i, std::list< std::pair<int, int> > &sortedB)
{
	std::size_t current_a_i = 0;
	for (std::list< std::pair<int, int> >::const_iterator it = sortedB.begin(); it != sortedB.end(); ++it, ++current_a_i)
	{
		if (current_a_i == 0)
			continue;
		pend.push_back(it->second);
		p_i.push_back(current_a_i);
	}
}

void set_pend(std::vector<int> &pend, std::vector<int> &p_i, const std::vector< std::pair<int, int> > &sortedB)
{
	pend.reserve((sortedB.size() > 1) ? sortedB.size() - 1 : 0);
	p_i.reserve((sortedB.size() > 1) ? sortedB.size() - 1 : 0);
	for (std::size_t i = 1; i < sortedB.size(); ++i)
	{
		pend.push_back(sortedB[i].second);
		p_i.push_back(static_cast<int>(i));
	}
}

std::list<std::size_t> set_insert_order(std::list<std::size_t> &bInsertOrder, std::size_t pairCount)
{
	std::size_t lastBlockEnd = 1;
	std::size_t jacobPrev = 1;
	std::size_t jacobCurr = 3;
	std::size_t blockEnd = 0;
	while (blockEnd != pairCount)
	{
		blockEnd = std::min(jacobCurr, pairCount);

		for (std::size_t pairNum = blockEnd; pairNum > lastBlockEnd; --pairNum)
			bInsertOrder.push_back(pairNum);
		lastBlockEnd = blockEnd;
		std::size_t jacobNext = jacobCurr + (2 * jacobPrev);
		jacobPrev = jacobCurr;
		jacobCurr = jacobNext;
	}
	return (bInsertOrder);
}

std::vector<std::size_t> set_insert_order(std::vector<std::size_t> &bInsertOrder, std::size_t pairCount)
{
	std::size_t lastBlockEnd = 1;
	std::size_t jacobPrev = 1;
	std::size_t jacobCurr = 3;
	std::size_t blockEnd = 0;
	while (blockEnd != pairCount)
	{
		blockEnd = std::min(jacobCurr, pairCount);
		for (std::size_t pairNum = blockEnd; pairNum > lastBlockEnd; --pairNum)
			bInsertOrder.push_back(pairNum);
		lastBlockEnd = blockEnd;
		std::size_t jacobNext = jacobCurr + (2 * jacobPrev);
		jacobPrev = jacobCurr;
		jacobCurr = jacobNext;
	}
	return (bInsertOrder);
}

void PmergeMe::fordJohnsonList(std::list<int> &list)
{
	if (list.size() < 2)
		return ;

	std::size_t pairCount = list.size() / 2;
	bool hasOdd = (list.size() % 2 != 0);
	int oddValue = 0;
	if (hasOdd)
	{
		oddValue = list.back();
		list.pop_back();
	}

	std::list< std::pair<int, int> > pairs = makePairs(list, pairCount);

	std::list<int> sortedA;
	for (std::list< std::pair<int, int> >::iterator it = pairs.begin(); it != pairs.end(); ++it)
		sortedA.push_back(it->first);

	fordJohnsonList(sortedA);

	std::list< std::pair<int, int> > sortedB = sortPairs(sortedA, pairs);
	
	std::list<int> main;
	std::list<int> a_i;
	set_main(main, a_i, sortedB);

	std::list<int> pend;
	std::list<int> p_i;
	set_pend(pend, p_i, sortedB);

	std::list<std::size_t> bInsertOrder;
	set_insert_order(bInsertOrder, pairCount);
	
	for (std::list<std::size_t>::iterator it = bInsertOrder.begin(); it != bInsertOrder.end(); ++it)
	{
		std::size_t pendIndex = *it - 2;
		if (pendIndex >= pend.size())
			continue;

		std::list<int>::iterator pendValueIt = pend.begin();
		std::list<int>::iterator pendAnchorIt = p_i.begin();
		std::advance(pendValueIt, pendIndex);
		std::advance(pendAnchorIt, pendIndex);

		std::list<int>::iterator a_iPos = std::find(a_i.begin(), a_i.end(), *pendAnchorIt);
		std::list<int>::iterator mainPos = main.begin();
		std::advance(mainPos, std::distance(a_i.begin(), a_iPos));

		std::list<int>::iterator insertMainPos = main.begin();
		insertMainPos = std::lower_bound(main.begin(), mainPos, *pendValueIt);

		std::list<int>::iterator insertAnchorIt = a_i.begin();
		for (std::list<int>::iterator it = main.begin(); it != insertMainPos; ++it)
			++insertAnchorIt;
		main.insert(insertMainPos, *pendValueIt);
		a_i.insert(insertAnchorIt, -1);
	}

	if (hasOdd)
	{
		std::list<int>::iterator insertValueIt = main.begin();
		while (insertValueIt != main.end() && *insertValueIt < oddValue)
			++insertValueIt;

		std::list<int>::iterator insertAnchorIt = a_i.begin();
		for(std::list<int>::iterator it = main.begin(); it != insertValueIt; ++it)
			++insertAnchorIt;
		main.insert(insertValueIt, oddValue);
		a_i.insert(insertAnchorIt, -1);
	}

	list.assign(main.begin(), main.end());
}

void PmergeMe::fordJohnsonVector(std::vector<int> &vector)
{
	if (vector.size() < 2)
		return ;

	std::size_t pairCount = vector.size() / 2;
	bool hasOdd = (vector.size() % 2 != 0);
	int oddValue = 0;
	if (hasOdd)
	{
		oddValue = vector.back();
		vector.pop_back();
	}

	std::vector< std::pair<int, int> > pairs = makePairs(vector, pairCount);

	std::vector<int> sortedA;
	sortedA.reserve(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
		sortedA.push_back(pairs[i].first);
	fordJohnsonVector(sortedA);

	std::vector< std::pair<int, int> > sortedB = sortPairs(sortedA, pairs);

	std::vector<int> main;
	std::vector<int> a_i;
	set_main(main, a_i, sortedB, vector.size());

	std::vector<int> pend;
	std::vector<int> p_i;
	set_pend(pend, p_i, sortedB);

	std::vector<std::size_t> bInsertOrder;
	set_insert_order(bInsertOrder, pairCount);

	for (std::size_t i = 0; i < bInsertOrder.size(); ++i)
	{
		std::size_t pairNumber = bInsertOrder[i];
		std::size_t pendIndex = pairNumber - 2;
		if (pendIndex >= pend.size())
			continue;

		const int pendValue = pend[pendIndex];
		const int pendAnchor = p_i[pendIndex];
		std::vector<int>::iterator a_iPos = std::find(a_i.begin(), a_i.end(), pendAnchor);
		if (a_iPos == a_i.end())
			continue;

		std::vector<int>::iterator mainPos = main.begin();
		std::advance(mainPos, std::distance(a_i.begin(), a_iPos));
		std::vector<int>::iterator insertMainPos = std::lower_bound(main.begin(), mainPos, pendValue);
		std::vector<int>::iterator insertAnchorIt = a_i.begin();
		std::advance(insertAnchorIt, std::distance(main.begin(), insertMainPos));

		main.insert(insertMainPos, pendValue);
		a_i.insert(insertAnchorIt, -1);
	}

	if (hasOdd)
	{
		std::vector<int>::iterator insertMainPos = std::lower_bound(main.begin(), main.end(), oddValue);
		std::vector<int>::iterator insertAnchorIt = a_i.begin();
		std::advance(insertAnchorIt, std::distance(main.begin(), insertMainPos));
		main.insert(insertMainPos, oddValue);
		a_i.insert(insertAnchorIt, -1);
	}

	vector = main;
}

void PmergeMe::print(const std::vector<int> &vector) const
{
	for (std::vector<int>::const_iterator it = vector.begin(); it != vector.end(); ++it)
		std::cout << *it << ' ';
	std::cout << std::endl;
}

void PmergeMe::print(const std::list<int> &list) const
{
	for (std::list<int>::const_iterator it = list.begin(); it != list.end(); ++it)
		std::cout << *it << ' ';
	std::cout << std::endl;
}

bool PmergeMe::run(int ac, char **av)
{
	PmergeMe pmergeme;
	std::vector<int> _vector;
	std::list<int> _list;
	if (!pmergeme.parseArgs(ac, av, _vector, _list))
	return (false);
	
	std:: cout << "Before: ";
	pmergeme.print(_vector);

	std::clock_t vecStart = std::clock();
	pmergeme.fordJohnsonVector(_vector);
	double vecUs = (std::clock() - vecStart) * 1000000.0 / CLOCKS_PER_SEC;

	std::clock_t listStart = std::clock();
	pmergeme.fordJohnsonList(_list);
	double listUs = (std::clock() - listStart) * 1000000.0 / CLOCKS_PER_SEC;

	std::cout << "After:  ";
	pmergeme.print(_vector);
	std::cout << "Time to process a range of " << _vector.size()
		<< " elements with std::vector : " << vecUs << " us" << std::endl;
	std::cout << "Time to process a range of " << _list.size()
		<< " elements with std::list   : " << listUs << " us" << std::endl;
	return (true);
}
