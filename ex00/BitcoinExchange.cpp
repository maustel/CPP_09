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
Function to validate the date format and its validity
------------------------------------------------------*/
bool isValidDate(const std::string& date)
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	int year = std::stoi(date.substr(0, 4));
	int month = std::stoi(date.substr(5, 2));
	int day = std::stoi(date.substr(8, 2));

	if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;

	return true;
}

/*-----------------------------------------------------------------------
Helper function to trim leading and trailing spaces
str.find_first_not_of(" \t"):
This finds the index of the first character in the string
str that is not a space (' ') or a tab ('\t').
Return an empty string if no non-space/tab characters are found
else: Return the trimmed substring (last - first + 1: The length
of the substring, calculated as the distance between first and last)
--------------------------------------------------------------------------*/
std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");

	if (first == std::string::npos || last == std::string::npos)
		return ("");
	else
		return str.substr(first, last - first + 1);
}

/*---------------------------------------------------
Function to validate the value in the database
------------------------------------------------------*/
void convertValidateDatabaseValue(double &value, const std::string& line, std::string value_str)
{
	try {
		value = std::stod(value_str);
	} catch (const std::exception&) {
		throw std::runtime_error("invalid value in data.csv: " + line);
	}

	if (value < 0)
		throw std::runtime_error("negative value in data.csv: " + line);

	if (value > std::numeric_limits<int>::max())
		throw std::runtime_error("value exceeds maximum integer limit in data.csv: " + line);
}


/*---------------------------------------------------
store data from data.csv into map
------------------------------------------------------*/
void parse_dataBase(std::map<std::string, double>& data_map)
{
	std::ifstream data_file("data.csv");
	if (!data_file.is_open())
		throw std::runtime_error("unable to open data.csv");

	std::string line;
	std::getline(data_file, line);

	if (line != "date,exchange_rate")
		throw std::runtime_error("invalid header in data.csv: " + line);

	while (std::getline(data_file, line))
	{
		std::istringstream ss(line);
		std::string date, value_str;
		double value;

		if (!std::getline(ss, date, ',') || !std::getline(ss, value_str))
			throw std::runtime_error("invalid data format in data.csv: " + line);

		if (!isValidDate(date))
			throw std::runtime_error("invalid date format in data.csv: " + line);

		convertValidateDatabaseValue(value, line, value_str);

		data_map[date] = value;
	}
}

/*---------------------------------------------------
Check and validate the value of the input-data
------------------------------------------------------*/
bool convertCheckInputValue(const std::string& value_str, double& value, const std::string& line)
{
	try {
		value = std::stod(value_str);
	} catch (const std::exception&) {
		std::cerr << "Error: invalid value in input => " << line << std::endl;
		return false;
	}

	if (value < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}

	if (value > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}

	return true;
}

/*---------------------------------------------------
Check and validate the date of input file
and find the closest date in the map.

The lower_bound function searches for the first date in data_map
that is not earlier than the given date.

If the exact date is not found, move to the closest earlier date (--it;)
------------------------------------------------------*/
bool checkDate(const std::string& date, const std::map<std::string, double>& data_map, std::map<std::string, double>::const_iterator& closest_it)
{
	if (!isValidDate(date))
	{
		std::cerr << "Error: bad input => " << date << std::endl;
		return false;
	}

	auto it = data_map.lower_bound(date);

	if (it == data_map.begin() && date < it->first)
	{
		std::cerr << "Error: date is too early and not in data map => " << date << std::endl;
		return false;
	}

	if (it == data_map.end() || it->first != date)
		--it;

	closest_it = it;

	return true;
}

/*---------------------------------------------------
Check and extract input data from the input file

std::getline(ss, date, '|'):
The extracted part (before the |) is stored in the variable date.
If the extraction fails (the delimiter | is missing or the
stream is empty), std::getline returns false.

After extracting the date, std::getline(ss, value_str) attempts
to extract the remaining part of the line (after the |)

When the continue statement is encountered:
The remaining code in the current iteration of the loop is skipped.
The loop proceeds to the next iteration.
------------------------------------------------------*/
void extract_input(const std::string& input_file, const std::map<std::string, double>& data_map)
{
	std::ifstream input(input_file);
	if (!input.is_open())
		throw std::runtime_error("unable to open input file");

	std::string line;
	std::getline(input, line);

	if (line != "date | value")
		throw std::runtime_error("invalid header in input file: " + line);

	while (std::getline(input, line))
	{
		std::istringstream ss(line);
		std::string date, value_str;
		double value;

		if (!std::getline(ss, date, '|') || !std::getline(ss, value_str))
		{
			std::cerr << "Error: invalid input format => " << line << std::endl;
			continue;
		}

		date = trim(date);
		value_str = trim(value_str);

		std::map<std::string, double>::const_iterator closest_it;

		if (!checkDate(date, data_map, closest_it))
			continue;

		if (!convertCheckInputValue(value_str, value, line))
			continue;

		if (closest_it != data_map.end())
			std::cout << date << " => " << value << " = " << (value * closest_it->second) << '\n';
		else
			std::cerr << "Error: no matching data for date => " << date << std::endl;
	}
}
