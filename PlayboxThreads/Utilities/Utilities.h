/*
 * Utilities.h
 *
 *  Created on: Jan 12, 2018
 *      Author: e2l3n
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <string>

namespace pbox_utils
{

	unsigned generateRandomValue(const unsigned& lowerBound, const unsigned& upperBound);

	std::string generateRandomLiteralString(const unsigned& length);

	void sleepForRandomMilliseconds(const unsigned& lowerBound, const unsigned& upperBound);

} /* namespace pbox_utils */

#endif /* UTILITIES_H_ */
