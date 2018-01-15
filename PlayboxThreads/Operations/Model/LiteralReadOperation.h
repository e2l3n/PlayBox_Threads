/*
 * LiteralReadOperation.h
 *
 *  Created on: Jan 12, 2018
 *      Author: e2l3n
 */

#ifndef LITERALREADOPERATION_H_
#define LITERALREADOPERATION_H_


#include "OperationInterface.h"
#include "../../Utilities/Utilities.h"

namespace pbox
{

	class LiteralReadOperation : public OperationInterface<std::string>
	{
	public:
		LiteralReadOperation(bool isSleepSimulationEnabled, bool isLogEnabled) : OperationInterface<std::string>(isSleepSimulationEnabled, isLogEnabled) {}

		virtual void operator()(std::list<ResourceInterface<std::string>*>& resources, unsigned countOfIterations = 1) override {};

		virtual void operator()(ResourceInterface<std::string>* resource, unsigned countOfIterations = 1) override
		{
			std::thread::id this_id = std::this_thread::get_id();

			if (resource == nullptr || countOfIterations == 0)
			{
				if (this->isLogEnabled())
				{
					std::cout << "READ[will read.], ID:" << this_id << std::endl;
				}
				return;
			}

			if (this->isLogEnabled())
			{
				std::cout << "READ[will read.], ID:" << this_id << std::endl;
			}

			// Sleep for some amount of milliseconds before execute procedure.
			if (this->isSleepSimulationEnabled())
			{
				pbox_utils::sleepForRandomMilliseconds(500, 1000);
			}

			while (countOfIterations > 0)
			{
				std::cout << "READ[reading ...], ID:" << this_id << std::endl;
				resource->print(std::cout);
				countOfIterations--;
			}

			if (this->isLogEnabled())
			{
				std::cout << "READ[will stop reading.], ID:" << this_id << std::endl;
			}
		};
	};

} /* namespace pbox */

#endif /* LITERALREADOPERATION_H_ */
