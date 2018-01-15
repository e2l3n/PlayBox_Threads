/*
 * OperationController.h
 *
 *  Created on: Jan 11, 2018
 *      Author: e2l3n
 */

 /** @file OperationController.h
  *
  *  @brief Singleton class that provides multithreaded management
  *  over a collection of resources.
  *
  *  Singleton class that provides multithreaded management over a
  *  collection of resource wrappers.
  *	There are three main types of operations that could be performed:
  *	1. Read resources from the collection.
  *	- When read operation is scheduled, controller claims read interest
  *	on the wrapper object of the particular resource and if no write operation
  *	has locked the object or the whole collection, the read operation begins.
  *	After the read operation is complete, controller then notifies all worker
  *	threads that want to write.
  *	2. Update resources from the collection.
  *	- Update operation over particular resource waits to begin until there
  *	isn't read interest for the resource. Controller then locks the resource
  *	wrapper for update operation and after that releases the lock and notifies
  *	all worker threads.
  *	3. Write operation.
  *	- This operation waits until there are no active operations on the
  *	collection and then locks the whole collection for writing.
  *
  *  @author Toma Popov
  *
  *  @bug No known bugs.
  */

#ifndef OPERATIONCONTROLLER_H_
#define OPERATIONCONTROLLER_H_

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <string>

#include "OperationCounter.h"

namespace pbox
{

	class OperationController : public OperationCounter<std::string>
	{
	public:
		/** @brief Access singleton instance.
		 *
		 *  Atomic access point to singleton instance.
		 *
		 *  @return singleton.
		 */
		static OperationController& sharedInstance();

		/** @brief Auto generate literal resources.
		 *
		 *  Generate automatically list of literal resources.
		 *
		 *  @param countOfResources number of resources to create.
		 *  @return true if operation is successful.
		 */
		bool setupLiteralResources(const unsigned& countOfResources) const;

		/** @brief Print all existing resources.
		 *
		 *  @return true if operation is successful.
		 */
		bool printAllResources(std::ostream& ostr = std::cout) const;

		/** @brief Delete all existing literal resources.
		 *
		 *  @return true if operation is successful.
		 */
		bool cleanUpResources();

		/** @brief Schedule new operation that inserts into or deletes from collection.
		 *
		 *  @param countOfIterations number of iterations to performs.
		 */
		void scheduleWriteOperation(const unsigned& countOfIterations = 1);

		/**
		 * @brief Schedule new operation that updates resources.
		 *  @param countOfIterations number of iterations to performs.
		 */
		void scheduleUpdateOperation(const unsigned& countOfIterations = 1);

		/** @brief Schedule new operation that reads from collection.
		 *
		 *  @param countOfIterations number of iterations to performs.
		 */
		void scheduleReadOperation(const unsigned& countOfIterations = 1);

		/** @brief Join all existing operations.
		 *
		 */
		void joinAllOperations();

		/** @brief Detach all threads.
		 *
		 *  Detach all working threads.
		 *
		 *  @param async performAsynchronous
		 */
		void detachAllOperations(bool performAsynchronous = false, void(*callback)() = nullptr);

	private:

		OperationController() = default;

		virtual ~OperationController() = default;

		OperationController(OperationController const&) = delete;

		OperationController& operator=(OperationController const&) = delete;

		void removeThreadFromStorage(std::thread::id id);



		std::condition_variable _conditionNoWriter;
		std::condition_variable _conditionNoActive;

		std::vector<std::thread> _threadsStorage;

		mutable std::mutex _mutexThreadsStorage;
		mutable std::mutex _controllerMutex;
	};

} /* namespace pbox */

#endif /* OPERATIONCONTROLLER_H_ */
