/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:01:12 by maustel           #+#    #+#             */
/*   Updated: 2025/07/24 18:01:12 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <map>
# include <string>
# include <fstream>
# include <stdexcept>
# include <sstream>

void parse_dataBase(std::map<std::string, double>& data_map);
void extract_input(const std::string& input_file, const std::map<std::string, double>& data_map);

#endif
