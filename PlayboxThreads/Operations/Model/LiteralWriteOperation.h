/*
 * LiteralWriteOperation.h
 *
 *  Created on: Jan 13, 2018
 *      Author: e2l3n
 */

#ifndef LITERALWRITEOPERATION_H_
#define LITERALWRITEOPERATION_H_

#include "OperationInterface.h"
#include "../../Utilities/Utilities.h"

namespace pbox
{

	class LiteralWriteOperation : public OperationInterface<std::string>
	{
	public:
		LiteralWriteOperation(bool isSleepSimulationEnabled, bool isLogEnabled) : OperationInterface<std::string>(isSleepSimulationEnabled, isLogEnabled) {};

		virtual void operator()(std::list<ResourceInterface<std::string>*>& resources, unsigned countOfIterations = 1) override
		{
			std::thread::id this_id = std::this_thread::get_id();

			if (resources.size() == 0 || countOfIterations == 0)
			{
				if (this->isLogEnabled())
				{
					std::cout << "WRITE[will stop writing.], ID:" << this_id << std::endl;
				}
				return;
			}

			if (this->isLogEnabled())
			{
				std::cout << "WRITE[will write.], ID:" << this_id << std::endl;
			}
				
			// Sleep for some amount of milliseconds before execute procedure.
			if (this->isSleepSimulationEnabled())
			{
				pbox_utils::sleepForRandomMilliseconds(500, 1000);
			}

			while (countOfIterations > 0)
			{
				bool shouldAddNewData = pbox_utils::generateRandomValue(0, 1);

				if (shouldAddNewData)
				{
					this->pushBackNewRandomLiteralResource(resources);
				}
				else
				{
					this->deleteRandomLiteralResource(resources);
				}

				countOfIterations--;
			}

			if (this->isLogEnabled())
			{
				std::cout << "WRITE[will stop writing.], ID:" << this_id << std::endl;
			}
		}

		virtual void operator()(ResourceInterface<std::string>* resource, unsigned countOfIterations = 1) override {};

		void pushBackNewRandomLiteralResource(std::list<ResourceInterface<std::string>*>& resources)
		{
			ResourceInterface<std::string>* literal = new LiteralResource();
			unsigned amount = pbox_utils::generateRandomValue(1, 100);

			literal->generateResource(amount);
			resources.push_back(literal);
		}

		void deleteRandomLiteralResource(std::list<ResourceInterface<std::string>*>& resources)
		{
			unsigned index = pbox_utils::generateRandomValue(0, resources.size() - 1);
			std::list<ResourceInterface<std::string>*>::iterator iterator = std::next(resources.begin(), index);

			while (iterator != resources.end())
			{
				if ((*iterator)->getReadersCount() == 0 && (*iterator)->getWritersCount() == 0)
				{
					resources.erase(iterator);
					break;
				}

				iterator++;
			}
		}
	};

} /* namespace pbox */

#endif /* LITERALWRITEOPERATION_H_ */
