/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:42:45 by maustel           #+#    #+#             */
/*   Updated: 2025/07/31 17:22:27 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

/*-----------------------------------------------------------------------
Function to handle the operator in the Reverse Polish Notation (RPN) expression
The function takes a stack of integers and an iterator to the current position in the expression.
It pops the top two integers from the stack, applies the operator, and pushes the result back onto the stack.
If there are not enough operands in the stack, it returns false.
--------------------------------------------------------------------------*/
bool handleOperator(std::stack<int>& stack, std::string::const_iterator& it)
{
	if (stack.size() < 2)
		return (false);
	int b = stack.top();
	stack.pop();
	int a = stack.top();
	stack.pop();
	switch (*it)
	{
		case '+':
			stack.push(a + b);
			break;
		case '-':
			stack.push(a - b);
			break;
		case '*':
			stack.push(a * b);
			break;
		case '/':
			if (b == 0)
				return (false);
			stack.push(a / b);
			break;
		default:
			return (false);
	}
	return (true);
}

/*-----------------------------------------------------------------------
Function to calculate the result of a Reverse Polish Notation (RPN) expression
The function uses a stack to evaluate the expression.
The expression is expected to be a string containing integers and operators (+, -, *, /).

The continue statement skips the rest of the current loop and starts from the beginning.
--------------------------------------------------------------------------*/
bool calculateRPN(const std::string& expression, std::stack<int>& stack)
{
	std::string::const_iterator it = expression.begin();
	if (expression.empty())
		return (false);
	while(it != expression.end())
	{
		if (isspace(*it))
		{
			++it;
			continue;
		}
		else if (isdigit(*it))
		{
			if (*it - '0' < 0 || *it - '0' > 9)
				return (false);
			stack.push(*it - '0');
		}
		else if (*it == '+' || *it == '-' || *it == '*' || *it == '/')
		{
			if (!handleOperator(stack, it))
				return (false);
		}
		else
			return (false);
		++it;
	}
	if (stack.size() != 1)
		return (false);
	return (true);
}
