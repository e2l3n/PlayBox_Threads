/*
 * OperationCounter.h
 *
 *  Created on: Jan 16, 2018
 *      Author: e2l3n
 */

#ifndef OPERATIONCOUNTER_H_
#define OPERATIONCOUNTER_H_

#include <atomic>
#include "../../Resources/Model/ResourceInterface.h"

namespace pbox
{

	template<class T>
	class OperationCounter
	{
	public:
		OperationCounter() : _countOfOperationsInProgress(0) {};

		inline int getCountOfOperationsInProgress() const
		{
			return _countOfOperationsInProgress;
		}

		inline void registerReadOperation(ResourceInterface<T>* resource)
		{
			_countOfOperationsInProgress++;
			if (resource)
				resource->registerReader();
		}

		inline void unregisterReadOperation(ResourceInterface<T>* resource)
		{
			_countOfOperationsInProgress--;
			if (resource)
				resource->unregisterReader();
		}

		inline void registerUpdateOperation(ResourceInterface<T>* resource)
		{
			_countOfOperationsInProgress++;
			if (resource)
				resource->registerWriter();
		}

		inline void unregisterUpdateOperation(ResourceInterface<T>* resource)
		{
			_countOfOperationsInProgress--;
			if (resource)
				resource->unregisterWriter();
		}

	private:
		std::atomic<int> _countOfOperationsInProgress;
	};

} /* namespace pbox */

#endif /* OPERATIONCOUNTER_H_ */
