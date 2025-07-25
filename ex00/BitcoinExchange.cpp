/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maustel <maustel@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:02:24 by maustel           #+#    #+#             */
/*   Updated: 2025/07/24 18:02:24 by maustel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

/*---------------------------------------------------
Default constructor
The std::map class has a default constructor that will properly initialize it
to an empty map, so I dont need to initialize it
------------------------------------------------------*/
BitcoinExchange::BitcoinExchange()
{}

/*---------------------------------------------------
Constructor
------------------------------------------------------*/
//todo

/*---------------------------------------------------
Copy Constructor
------------------------------------------------------*/
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other): _data(other._data)
{}

/*---------------------------------------------------
Assignment operator
------------------------------------------------------*/
BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange &other)
{
	if (this != &other)
	{
		_data = other._data;
	}
	return (*this);
}

/*---------------------------------------------------
Destructor
------------------------------------------------------*/
BitcoinExchange::~BitcoinExchange()
{}

/*---------------------------------------------------
store data from data.csv into map
------------------------------------------------------*/
void BitcoinExchange::parse_dataBase()
{
	std::ifstream data_file("input.txt");
	if (!data_file.is_open())
		throw std::runtime_error("unable to open input.txt");
	std::string line;
	std::getline(data_file, line);	//skip header
	while (std::getline(data_file, line))
	{

	}

        // while (std::getline(file, line)) {
        //     std::stringstream ss(line);
        //     std::string dateStr;

        //     // Extract date part
        //     std::getline(ss, dateStr, ',');

        //     // Extract price part
        //     float price;
        //     ss >> price;

        //     _data[dateStr] = price;
        // }

        // return !file.fail();
}
