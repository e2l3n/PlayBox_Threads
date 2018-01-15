/*
 * LiteralResource.h
 *
 *  Created on: Jan 11, 2018
 *      Author: e2l3n
 */

#ifndef LITERALRESOURCE_H_
#define LITERALRESOURCE_H_

#include <string>
#include <atomic>

#include "ResourceInterface.h"

namespace pbox
{

	class LiteralResource : public ResourceInterface<std::string>
	{

	public:

		LiteralResource() : ResourceInterface<std::string>(0, 0) {};

		LiteralResource(const std::string& literal) : ResourceInterface<std::string>(0, 0)
		{
			_resource = literal;
		}

		LiteralResource& operator=(const LiteralResource& lr);

		virtual void generateResource(const unsigned& amount = 0) override;

		virtual void setResource(const std::string& literal) override;

		void modifyResourceAtIndex(const unsigned& idx, const char& ch);

		virtual ~LiteralResource() {}

		virtual std::string getResourceCopy() const override;

		virtual void print(std::ostream& ostr) const override;

		virtual unsigned getResourceSize() const override;

		std::ostream& operator<<(std::ostream& ostr);

		bool operator==(const LiteralResource& lr);

	private:
		mutable std::string _resource;
	};

} /* namespace pbox */

#endif /* LITERALRESOURCE_H_ */
