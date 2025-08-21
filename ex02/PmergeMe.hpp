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
		int _pendIndex;
		int _toSearch;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		void start(int argc, char **argv);
		void parseInput(int argc, char **argv);
		void checkContainers();
		void findJacobsthalNumber(int& previous, int& current);


/*
	Find the starting iterator for the binary search.
	Use the Jacobsthal number to find the correct position.
	-> if jacobsthal = 3 -> take last number of b3 (which is the second element)
	std::advance(iterator, n); -> Moves the iterator n positions forward
	pendIndex is the index of the first number of the current element
*/
template <typename T>
typename T::iterator getStart(int elementSize, T& pend)
{
	typename T::iterator it;
	//print pend.size()/elementSize in YEL
	std::cout << YEL << "Pend size / Element size: " << (pend.size() / elementSize) << RESET << std::endl;
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
	_toSearch = *(it + elementSize - 1);
	return (it);
}

/*
	Perform binary search and insert into main
*/
template <typename T>
void binarySearch(typename T::iterator start, T& main, int elementSize)
{
	//search for the startvalue where to insert in main left from pendIndex*elementSize and do binary insertion in that part of the container
	typename T::iterator it = main.begin();
	std::advance(it, _pendIndex * elementSize);
	if (*start < *it)
		_pendIndex--;
	else
	//insert
		main.insert(it, *start);
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
	int current = 1;
	int previous = 1;

	size_t totalElements = main.size() + pend.size();
	// Check if pending chain is empty
	if (pend.empty())
		return;
	//maybe jacobsthal as local variable (?)
	findJacobsthalNumber(current, previous);
	typename T::iterator start = getStart(elementSize, pend);

	std::cout << MAG << "Jacobsthal: " << _jacobsthal << RESET << std::endl;
	std::cout << RED << "Start iterator: " << *start << RESET << std::endl;
	std::cout << RED << "To search: " << _toSearch << RESET << std::endl;

	// while (main.size() != totalElements)
	// {
	// 	//binary search and insert into main
	// 	binarySearch(start, main, elementSize);
	// }
	//insert rest numbers in the end
	//update container
	(void)container; // suppress unused variable warning
	(void)main; // suppress unused variable warning
	(void)totalElements;
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
