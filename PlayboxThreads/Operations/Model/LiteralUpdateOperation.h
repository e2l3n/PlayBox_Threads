/*
 * LiteralUpdateOperation.h
 *
 *  Created on: Jan 12, 2018
 *      Author: e2l3n
 */

#ifndef LITERALUPDATEOPERATION_H_
#define LITERALUPDATEOPERATION_H_

#include "OperationInterface.h"
#include "../../Utilities/Utilities.h"

namespace pbox
{

	class LiteralUpdateOperation : public OperationInterface<std::string>
	{
	public:
		LiteralUpdateOperation(bool isSleepSimulationEnabled, bool isLogEnabled) : OperationInterface<std::string>(isSleepSimulationEnabled, isLogEnabled) {}

		virtual void operator()(std::list<ResourceInterface<std::string>*>& resources, unsigned countOfIterations = 1) override {}

		virtual void operator()(ResourceInterface<std::string>* resource, unsigned countOfIterations = 1) override
		{
			std::thread::id this_id = std::this_thread::get_id();

			if (resource == nullptr || countOfIterations == 0)
			{
				if (this->isLogEnabled())
				{
					std::cout << "UPDATE[will update.], ID:" << this_id << std::endl;
				}
				return;
			}

			if (this->isLogEnabled())
			{
				std::cout << "UPDATE[will update.], ID:" << this_id << std::endl;
			}

			// Sleep for some amount of milliseconds before execute procedure.
			if (this->isSleepSimulationEnabled())
			{
				pbox_utils::sleepForRandomMilliseconds(500, 1000);
			}

			while (countOfIterations > 0)
			{
				LiteralResource* literalResource = reinterpret_cast<LiteralResource*>(resource);
				unsigned index = pbox_utils::generateRandomValue(0, literalResource->getResourceSize() - 1);

				literalResource->modifyResourceAtIndex(index, '*');
				countOfIterations--;
			}

			if (this->isLogEnabled())
			{
				std::cout << "UPDATE[will stop updating.], ID:" << this_id << std::endl;
			}
		};
	};

} /* namespace pbox */

#endif /* LITERALUPDATEOPERATION_H_ */
