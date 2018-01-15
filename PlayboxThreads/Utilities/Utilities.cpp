/*
 * Utilities.cpp
 *
 *  Created on: Jan 12, 2018
 *      Author: e2l3n
 */

#include <stdexcept>
#include <random>
#include <iostream>
#include <thread>
#include <chrono>

#include "Utilities.h"

namespace pbox_utils
{

	unsigned generateRandomValue(const unsigned& lowerBound, const unsigned& upperBound)
	{
		if (lowerBound > upperBound)
		{
			const std::string errorMsg = "ResourceInterface::modifyLiteralAtIndex(const unsigned&, const char&) - Invalid arguments.";
			throw std::runtime_error(errorMsg);
		}

		if (lowerBound == upperBound)
		{
			return lowerBound;
		}

		static std::random_device rd;
		static std::mt19937 rng(rd());
		std::uniform_int_distribution<unsigned> uni(lowerBound, upperBound);
		unsigned randomInteger = uni(rng);

		return randomInteger;
	}

	std::string generateRandomLiteralString(const unsigned& length)
	{
		static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string result;
		result.resize(length);

		for (unsigned i = 0; i < length; i++)
		{
			result[i] = charset[rand() % charset.length()];
		}

		return result;
	}

	void sleepForRandomMilliseconds(const unsigned& lowerBound, const unsigned& upperBound)
	{
		if (lowerBound > upperBound)
		{
			const std::string errorMsg = "ResourceInterface::modifyLiteralAtIndex(const unsigned&, const char&) - Invalid arguments.";
			throw std::runtime_error(errorMsg);
		}

		unsigned time = pbox_utils::generateRandomValue(lowerBound, upperBound);
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
	}

} /* namespace pbox_utils */
