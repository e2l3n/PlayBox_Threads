/*
 * OperationInterface.h
 *
 *  Created on: Jan 12, 2018
 *      Author: e2l3n
 */

#ifndef OPERATIONINTERFACE_H_
#define OPERATIONINTERFACE_H_

#include <atomic>
#include <list>

#include "../../Resources/Model/ResourceInterface.h"

namespace pbox {

	template<class T>
	class OperationInterface {
	private:
		bool _isSleepSimulationEnabled,
			_isLogEnabled;

	protected:
		// Object instantiation and modification is restricted only
		// to classes of type 'OperationController'
		friend class OperationController;
		OperationInterface() = default;
		OperationInterface(bool isSleepSimulationEnabled, bool isLogEnabled) : _isSleepSimulationEnabled(isSleepSimulationEnabled), _isLogEnabled(isLogEnabled) {};
	public:

		virtual ~OperationInterface() = default;

		virtual void operator()(ResourceInterface<T>* resource, unsigned countOfIterations = 1) = 0;

		virtual void operator()(std::list<ResourceInterface<T>*>& resources, unsigned countOfIterations = 1) = 0;

		inline bool isSleepSimulationEnabled() const
		{
			return _isSleepSimulationEnabled;
		}

		inline bool isLogEnabled() const
		{
			return _isSleepSimulationEnabled;
		}
	};

}
;
/* namespace pbox */

#endif /* OPERATIONINTERFACE_H_ */
