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

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		void start(int argc, char **argv);
		void parseInput(int argc, char **argv);
		void checkContainers();
};


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
elementSize means the size of one part of a pair
-> in the beginning it is a number (elementSize = 1)
-> then it is a pair of numbers (elementSize = 2)
if the last number of B is bigger than the last number of A,
then swap the elements of this pair in the container

*/
template <typename T>
T FordJohnson(T& container, int elementSize)
{
	for (auto i = container.begin(); distance(i, container.end()) >= elementSize * 2; i += elementSize * 2)
	{
		auto itB = i;
		auto itA = i + elementSize;

		if (*(itB + elementSize - 1) > *(itA + elementSize - 1))
		{
			std::swap_ranges(itA, itA + elementSize, itB);
		}
	}
	if (container.size() >= static_cast<size_t>(elementSize * 2))
	{
		FordJohnson(container, elementSize * 2);
		//create pair
		//devide in main chain and pending chain
		std::pair<T,T> chains = buildChains(container, elementSize);
		//binaryInsertion(container, elementSize, chains);
	}
	return (container);
}

#endif
