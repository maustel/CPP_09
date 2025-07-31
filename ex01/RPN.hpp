/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:48:43 by maustel           #+#    #+#             */
/*   Updated: 2025/07/31 17:02:32 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <stdexcept>
# include <string>
# include <stack>
#include <iterator>

bool calculateRPN(const std::string& expression, std::stack<int>& stack);

#endif