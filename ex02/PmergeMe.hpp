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
		int _nbrInsertedElements;  // Total number of inserted elements
		int _currentRangeInserted; // Number of inserted elements in current Jacobsthal range
		int _searchRange;

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

	// Skip processed elements (marked as -1)
	while (it < pend.end() && *it == -1)
	{
		std::advance(it, elementSize);
		_pendIndex++;
		if (it >= pend.end())
		{
			// Wrap around or find next valid Jacobsthal position
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
void binarySearch(typename T::iterator PendIterator, T& main, int elementSize)
{
	typename T::iterator startRange = main.begin();
	typename T::iterator endRange = main.begin();

	getSearchRange(endRange, main, elementSize);

	//print in YEL the part of main from startRange to endRange
	std::cout << YEL << "Main (before binary search): ";
	for (auto it = startRange; it != endRange; ++it)
		std::cout << *it << " ";
	std::cout << RESET << std::endl;

	// Binary search by comparing with last element of each block
	typename T::iterator left = startRange;
	typename T::iterator right = endRange;
	typename T::iterator insertPos = endRange;

	while (left < right) {
		// Find the middle block
		int blocksFromStart = std::distance(startRange, left) / elementSize;
		int blocksFromEnd = std::distance(startRange, right) / elementSize;
		int middleBlock = (blocksFromStart + blocksFromEnd) / 2;

		typename T::iterator mid = startRange;
		std::advance(mid, middleBlock * elementSize);

		// Compare _toCompare with the last element of the middle block
		if (mid + elementSize - 1 < main.end() && *(mid + elementSize - 1) >= _toCompare)
		{
			insertPos = mid;
			right = mid;
		}
		else
		{
			left = mid;
			std::advance(left, elementSize);
		}
	}

	// Check if insertion is at the very right of the search range
	if (insertPos == endRange)
		_searchRange--;

	// Insert the entire block from PendIterator (elementSize elements) at insertPos
	main.insert(insertPos, PendIterator, PendIterator + elementSize);

	// Mark the inserted elements as processed in the pending chain
	std::fill(PendIterator, PendIterator + elementSize, -1);

	// Increment both counters
	_nbrInsertedElements++;        // Total insertions
	_currentRangeInserted++;       // Current Jacobsthal range insertions

	std::cout << GRN << "Inserted block: ";
	for (auto it = PendIterator; it != PendIterator + elementSize; ++it)
		std::cout << *it << " ";
	std::cout << RESET << std::endl;

	std::cout << BLU << "Number of inserted elements: " << _nbrInsertedElements << RESET << std::endl;

	// Print the entire main chain (after insertion)
	std::cout << YEL << "Main (after binary search): ";
	for (const auto& elem : main)
		std::cout << elem << " ";
	std::cout << RESET << std::endl;
}

/*
Binary insertion sort: sort from chain into main
Use jacobsthal numbers to find out which position gets sorted in first.
Start binary search in main at position left from corresponding pair number.
Same as before: compare always last number of elementSize-block
*/
template <typename T>
void binaryInsertion(T& container, int elementSize, std::pair<T,T>& chains)
{
	T& main = chains.first;
	T& pend = chains.second;
	_jacobsthal = 3;
	_prevJacobsthal = 1;
	_pendIndex = 0;
	_toCompare = 0;
	_nbrInsertedElements = 0;     // Total inserted elements
	_currentRangeInserted = 0;    // Elements inserted in current Jacobsthal range

	// size_t totalElements = main.size() + pend.size();
	if (pend.empty())
		return;

	// findJacobsthalNumber();
	typename T::iterator PendIterator = getPendIndex(elementSize, pend);
	_searchRange = _jacobsthal + _nbrInsertedElements;

	std::cout << MAG << "Jacobsthal: " << _jacobsthal << RESET << std::endl;
	std::cout << RED << "Pend iterator: " << *PendIterator << RESET << std::endl;
	std::cout << RED << "To search: " << _toCompare << RESET << std::endl;

	binarySearch(PendIterator, main, elementSize);

	// Ford-Johnson algorithm main loop - continue until all non-(-1) elements are processed
	while (std::find_if(pend.begin(), pend.end(), [](int x) { return x != -1; }) != pend.end() &&
	       _nbrInsertedElements < static_cast<int>(pend.size() / elementSize))
	{
		// Check if we've inserted all elements for current Jacobsthal range
		if (_currentRangeInserted == _jacobsthal - _prevJacobsthal)
		{
			// Find new Jacobsthal numbers using the implemented function
			findJacobsthalNumber();
			// Reset the current range counter for the new range
			_currentRangeInserted = 0;
			// Update pendIndex and searchRange for new Jacobsthal
			PendIterator = getPendIndex(elementSize, pend);
			_searchRange = _jacobsthal + _nbrInsertedElements;

			std::cout << MAG << "New Jacobsthal: " << _jacobsthal << " (prev: " << _prevJacobsthal << ")" << RESET << std::endl;
			std::cout << RED << "New Pend iterator: " << *PendIterator << RESET << std::endl;
		}
		else
		{
			// Step back in pendIndex by elementSize
			_pendIndex--;
			if (_pendIndex > 0 && PendIterator >= pend.begin() + elementSize)
			{
				PendIterator -= elementSize;
				_toCompare = *(PendIterator + elementSize - 1);
				std::cout << YEL << "Stepped back - Pend iterator: " << *PendIterator << " To search: " << _toCompare << RESET << std::endl;
			}
			else
			{
				// Can't step back further, get new Jacobsthal
				findJacobsthalNumber();
				PendIterator = getPendIndex(elementSize, pend);
				_searchRange = _jacobsthal + _nbrInsertedElements;
			}
		}

		// Perform binary search and insertion
		binarySearch(PendIterator, main, elementSize);
	}

	// Insert remaining elements from the end of container into main
	// The remaining elements are those not included in main+pend chains
	size_t processedElements = main.size() + pend.size();
	if (processedElements < container.size()) {
		auto restStart = container.begin() + processedElements;
		main.insert(main.end(), restStart, container.end());
		std::cout << CYN << "Inserted " << std::distance(restStart, container.end()) << " remaining elements from container" << RESET << std::endl;
	}

	// Update container with the sorted main chain
	container = main;	std::cout << GRN << "Final sorted container: ";
	for (const auto& elem : container)
		std::cout << elem << " ";
	std::cout << RESET << std::endl;
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
	//push the first elementSize numbers into mainChain
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
	//print container in green
	std::cout << GRN << "Container: ";
	for (const auto& elem : container)
		std::cout << elem << " ";
	std::cout << RESET << std::endl;

	std::cout << BLU << "elementSize: " << elementSize << RESET << std::endl;

	//print main chain and pending chain
	std::cout << "Main chain: ";
	for (const auto& elem : mainChain)
		std::cout << elem << " ";
	std::cout << "\nPending chain: ";
	for (const auto& elem : pendingChain)
		std::cout << elem << " ";
	std::cout << std::endl;

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

		if (*(itB + elementSize - 1) > *(itA + elementSize - 1))
			std::swap_ranges(itA, itA + elementSize, itB);
	}
	if (container.size() >= static_cast<size_t>(elementSize * 2))
	{
		FordJohnson(container, elementSize * 2);
		std::pair<T,T> chains = buildChains(container, elementSize);
		binaryInsertion(container, elementSize, chains);
	}
	return (container);
}
};
#endif
