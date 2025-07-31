/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:47:39 by maustel           #+#    #+#             */
/*   Updated: 2025/07/31 17:08:29 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: wrong number of arguments." << std::endl;
		return (1);
	}

	std::stack<int> stack;
	if (!calculateRPN(argv[1], stack))
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}
	std::cout << stack.top() << std::endl;

	return (0);
}
