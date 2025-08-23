/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 12:22:17 by maustel           #+#    #+#             */
/*   Updated: 2025/08/08 12:22:17 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <ctime>
#include <sstream>
#include <chrono>
#include <cmath>

#include "colors.h"

class PmergeMe
{
	private:
		std::vector<int> _vec;
		std::deque<int> _deq;
		int _jacobsthal;
		int _prevJacobsthal;
		int _pendIndex;
		int _toCompare;
		int _nbrInsertedElements;
		int _currentRangeInserted;
		int _searchRange;
		int _nbrComparisons;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		void start(int argc, char **argv);
		void parseInput(int argc, char **argv);
		void checkContainers();
		void findJacobsthalNumber();

/*
	Find the starting iterator for the binary search.
	Use the Jacobsthal number to find the correct position.
	-> if jacobsthal = 3 -> take last number of b3 (which is the second element)
	std::advance(iterator, n); -> Moves the iterator n positions forward
	pendIndex is the index of the first number of the current element
*/
template <typename T>
typename T::iterator getPendIndex(int elementSize, T& pend)
{
	typename T::iterator it;
	if (static_cast<size_t>(_jacobsthal - 1) > (pend.size() / elementSize))
	{
		it = pend.end();
		std::advance(it, -elementSize);
		_pendIndex = (pend.size()) / elementSize;
	}
	else
	{
		it = pend.begin();
		std::advance(it, (_jacobsthal - 2) * elementSize);
		_pendIndex = _jacobsthal - 1;
	}

	while (it < pend.end() && *it == -1)
	{
		std::advance(it, elementSize);
		_pendIndex++;
		if (it >= pend.end())
		{
			it = pend.end();
			std::advance(it, -elementSize);
			_pendIndex = (pend.size()) / elementSize;
			break;
		}
	}

	if (it < pend.end() && it + elementSize - 1 < pend.end())
		_toCompare = *(it + elementSize - 1);
	return (it);
}

template <typename T>
void getSearchRange(typename T::iterator& endRange, T& main, int elementSize)
{
	if (_searchRange * elementSize > static_cast<int>(main.size()))
		endRange = std::prev(main.end());
	else
		std::advance(endRange, _searchRange * elementSize);
}

/*
	Perform binary search and insert into main
*/
template <typename T>
void binaryInsertion(typename T::iterator PendIterator, T& main, int elementSize)
{
	typename T::iterator startRange = main.begin();
	typename T::iterator endRange = main.begin();

	getSearchRange(endRange, main, elementSize);

	typename T::iterator left = startRange;
	typename T::iterator right = endRange;
	typename T::iterator insertPos = endRange;

	while (left < right)
	{
		int elementsFromStart = std::distance(startRange, left) / elementSize;
		int elementsFromEnd = std::distance(startRange, right) / elementSize;
		int middleElement = (elementsFromStart + elementsFromEnd) / 2;

		typename T::iterator mid = startRange;
		std::advance(mid, middleElement * elementSize);

		_nbrComparisons++;
		if (mid + elementSize - 1 < main.end() && *(mid + elementSize - 1) >= _toCompare)
		{
			insertPos = mid;
			right = mid;
		}
		else
		{
			left = mid;
			std::advance(left, elementSize);
			insertPos = left;
		}
	}
	main.insert(insertPos, PendIterator, PendIterator + elementSize);

	if (insertPos == endRange)
		_searchRange--;

	std::fill(PendIterator, PendIterator + elementSize, -1);
	_nbrInsertedElements++;
	_currentRangeInserted++;
}

/*
	Main sorting loop using Jacobsthal numbers and binary insertion:

*/
template <typename T>
void SortingMainLoop(T& container, int elementSize, std::pair<T,T>& chains)
{
	T& main = chains.first;
	T& pend = chains.second;
	_jacobsthal = 3;
	_prevJacobsthal = 1;
	_pendIndex = 0;
	_toCompare = 0;
	_nbrInsertedElements = 0;
	_currentRangeInserted = 0;

	if (pend.empty())
		return;

	typename T::iterator PendIterator = getPendIndex(elementSize, pend);
	_searchRange = _jacobsthal + _nbrInsertedElements;

	binaryInsertion(PendIterator, main, elementSize);

	while (std::find_if(pend.begin(), pend.end(), [](int x) { return x != -1; }) != pend.end() &&
			_nbrInsertedElements < static_cast<int>(pend.size() / elementSize))
	{
		if (_currentRangeInserted == _jacobsthal - _prevJacobsthal)
		{
			findJacobsthalNumber();
			_currentRangeInserted = 0;
			PendIterator = getPendIndex(elementSize, pend);
			_searchRange = _jacobsthal + _nbrInsertedElements;
		}
		else
		{
			_pendIndex--;
			if (_pendIndex > 0 && PendIterator >= pend.begin() + elementSize)
			{
				PendIterator -= elementSize;
				_toCompare = *(PendIterator + elementSize - 1);
			}
			else
			{
				findJacobsthalNumber();
				PendIterator = getPendIndex(elementSize, pend);
				_searchRange = _jacobsthal + _nbrInsertedElements;
			}
		}
		binaryInsertion(PendIterator, main, elementSize);
	}

	size_t processedElements = main.size();
	if (processedElements < container.size())
	{
		auto restStart = container.begin() + processedElements;
		main.insert(main.end(), restStart, container.end());
	}

	container = main;
}

/*
	push the first elementSize numbers into mainChain
	then add the next elementSize numbers also into mainChain
	alternate between mainChain and pendingChain always elementSize numbers
	so b1 and all a go to mainchain and the rest of b go to pendingChain
	rest numbers that are single, remain outside
*/
template <typename T>
std::pair<T,T> buildChains(T& container, int elementSize)
{
	T mainChain;
	T pendingChain;

	auto it = container.begin();
	for (int i = 0; i < elementSize && it != container.end(); ++i, ++it)
		mainChain.push_back(*it);
	int insert = 1;
	while (it != container.end()
		&& (container.size() - (insert * elementSize) >= static_cast<size_t>(elementSize)))
	{
		if (insert % 2 == 0)
			pendingChain.push_back(*it);
		else
			mainChain.push_back(*it);
		++it;
		if (distance(container.begin(), it) % elementSize == 0)
			++insert;
	}
	return std::make_pair(mainChain, pendingChain);
}

/*
Sort numbers with Ford Johnson algorithm
elementSize means the size of one part of a pair
-> in the beginning it is a number (elementSize = 1)
-> then it is a pair of numbers (elementSize = 2)
if the last number of B is bigger than the last number of A,
then swap the elements of this pair in the container.
Then build chains and do binary insertion sort with jacobsthalnumbers.
Since it is recursive, building chains and insertion sort will
start from highest recursion level and goes on until level 1.
*/
template <typename T>
T FordJohnson(T& container, int elementSize)
{
	for (auto i = container.begin(); distance(i, container.end()) >= elementSize * 2; i += elementSize * 2)
	{
		auto itB = i;
		auto itA = i + elementSize;

		_nbrComparisons++;
		if (*(itB + elementSize - 1) > *(itA + elementSize - 1))
			std::swap_ranges(itA, itA + elementSize, itB);
	}
	if (container.size() >= static_cast<size_t>(elementSize * 2))
	{
		FordJohnson(container, elementSize * 2);
		std::pair<T,T> chains = buildChains(container, elementSize);
		SortingMainLoop(container, elementSize, chains);
	}
	return (container);
}
};
#endif
