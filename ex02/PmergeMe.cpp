/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 12:23:32 by maustel           #+#    #+#             */
/*   Updated: 2025/08/08 12:23:32 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe():_jacobsthal(3),
					_prevJacobsthal(1),
					_pendIndex(0),
					_toCompare(0),
					_nbrInsertedElements(0),
					_currentRangeInserted(0),
					_searchRange(0),
					_nbrComparisons(0) {}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	*this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
		_jacobsthal = other._jacobsthal;
		_pendIndex = other._pendIndex;
		_toCompare = other._toCompare;
		_nbrInsertedElements = other._nbrInsertedElements;
		_currentRangeInserted = other._currentRangeInserted;
		_searchRange = other._searchRange;
		_nbrComparisons = other._nbrComparisons;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

/*
	check if input is valid and throw errors if not.
	Then write input numbers into containers.
*/
void PmergeMe::parseInput(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg.empty())
			throw std::invalid_argument("Empty input is not allowed.");
		size_t start = 0;
		if (arg[0] == '+')
			start = 1;
		if (start >= arg.length())
			throw std::invalid_argument("Invalid input: sign without digits.");
		if (arg.find_first_not_of("0123456789", start) != std::string::npos)
			throw std::invalid_argument("Invalid input: non-digit characters found in " + arg + " at position " + std::to_string(i));

		int num = std::stoi(arg);
		if (num < 0)
			throw std::invalid_argument("Negative numbers are not allowed.");
		if (std::find(_vec.begin(), _vec.end(), num) != _vec.end())
			throw std::invalid_argument("Duplicate numbers are not allowed.");

		_vec.push_back(num);
		_deq.push_back(num);
	}
}

/*
	check if containers are valid and throw errors if not.
*/
void PmergeMe::checkContainers()
{
	if (_vec.empty() || _deq.empty())
		throw std::runtime_error("No valid numbers to sort.");
	if (_vec.size() != _deq.size())
		throw std::runtime_error("Vectors and deques must have the same size.");
	if (_vec.size() == 1 && _deq.size() == 1)
		throw std::runtime_error("Only one number provided, nothing to sort.");
	if (std::is_sorted(_vec.begin(), _vec.end()) && std::is_sorted(_deq.begin(), _deq.end()))
		throw std::runtime_error("The input is already sorted.");
}

/*
	Maximal number of comparisons allowed for a given input size.
	For checking performance and optimization.
*/
int MaxComparisonsAllowed(int nbrElements)
{
	int sum = 0;
	for (int k = 1; k <= nbrElements; ++k)
	{
		double value = (3.0 / 4.0) * k;
		sum += static_cast<int>(ceil(log2(value)));
	}
	return sum;
}

/*
	Start the sorting process.

	vector is much faster, because it is more efficient with memory access
	patterns like iterator arithmetic and random access.
	deque uses a more complex memory structure, which can lead to slower
	access times in certain scenarios.
*/
void PmergeMe::start(int argc, char **argv)
{
	try
	{
		parseInput(argc, argv);
		checkContainers();
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "unsorted: ";
	for (const auto& elem : _vec)
		std::cout << elem << " ";
	std::cout << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	auto sortedVec = FordJohnson(_vec, 1);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::micro> duration = end - start;

	std::cout << "Sorted: ";
	for (const auto& elem : sortedVec)
		std::cout << YEL << elem << " ";
	std::cout << RESET << std::endl;

	std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector: " << duration.count() << " microseconds." << std::endl;
	// if (is_sorted(sortedVec.begin(), sortedVec.end()))
	// 	std::cout << GRN << "Vector is sorted correctly." << RESET << std::endl;
	// else
	// 	std::cout << RED << "Vector is NOT sorted correctly." << RESET << std::endl;
	// std::cout << CYN << "Total comparisons made for vector: " << _nbrComparisons << RESET << std::endl;

	_nbrComparisons = 0;

	auto startDeq = std::chrono::high_resolution_clock::now();
	auto sortedDeq = FordJohnson(_deq, 1);
	auto endDeq = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::micro> durationDeq = endDeq - startDeq;

	std::cout << "Time to process a range of " << _deq.size() << " elements with std::deque: " << durationDeq.count() << " microseconds." << std::endl;
	// if (is_sorted(sortedDeq.begin(), sortedDeq.end()))
	// 	std::cout << GRN << "Deque is sorted correctly." << RESET << std::endl;
	// else
	// 	std::cout << RED << "Deque is NOT sorted correctly." << RESET << std::endl;
	// std::cout << CYN << "Total comparisons made for deque: " << _nbrComparisons << RESET << std::endl;
	// int maxAllowed = MaxComparisonsAllowed(_vec.size());
	// std::cout << MAG << "Max comparisons allowed: " << maxAllowed << RESET << std::endl;
}

void PmergeMe::findJacobsthalNumber()
{
	int current = this->_jacobsthal;
	this->_jacobsthal = current + 2 * _prevJacobsthal;
	_prevJacobsthal = current;
}
