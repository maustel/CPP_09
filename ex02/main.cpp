/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:47:39 by maustel           #+#    #+#             */
/*   Updated: 2025/08/23 14:42:52 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
random number generator:
https://stattrek.com/statistics/random-number-generator
*/

#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: wrong number of arguments." << std::endl;
		return (1);
	}
	PmergeMe pmerge;
	pmerge.start(argc, argv);
	return (0);
}
