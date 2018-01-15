/*
 * LiteralResource.cpp
 *
 *  Created on: Jan 11, 2018
 *      Author: e2l3n
 */

#include <stdexcept>
#include <iostream>
#include <string.h>

#include "LiteralResource.h"
#include "../../Utilities/Utilities.h"

namespace pbox
{

	void LiteralResource::generateResource(const unsigned& amount)
	{
		std::string randomLiteral = pbox_utils::generateRandomLiteralString(amount);

		this->setResource(randomLiteral);
	}

	void LiteralResource::setResource(const std::string& literal)
	{
		this->_resource = literal;
	}

	void LiteralResource::modifyResourceAtIndex(const unsigned& idx, const char& ch)
	{
		if (idx >= this->_resource.size())
		{
			const std::string errorMsg = "ResourceInterface::modifyLiteralAtIndex(const unsigned&, const char&) - Invalid arguments.";
			throw std::runtime_error(errorMsg);
		}

		this->_resource[idx] = ch;
	}

	std::string LiteralResource::getResourceCopy() const
	{
		return this->_resource;
	}

	unsigned LiteralResource::getResourceSize() const
	{
		return this->_resource.size();
	}

	void LiteralResource::print(std::ostream& ostr) const
	{
		ostr << this->_resource << std::endl;
	}

	std::ostream& LiteralResource::operator<<(std::ostream& ostr)
	{
		ostr << this->_resource << std::endl;
		return ostr;
	}

	bool LiteralResource::operator==(const LiteralResource& lr)
	{
		bool result = lr._resource == this->_resource;
		return result;
	}

} /* namespace pbox */
