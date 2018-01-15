/*
 * OperationController.cpp
 *
 *  Created on: Jan 11, 2018
 *      Author: e2l3n
 */
#include <mutex>
#include <vector>
#include <future>
#include <algorithm>

#include "OperationController.h"
#include "../../Resources/Model/LiteralResource.h"
#include "../../Utilities/Utilities.h"
#include "../Model/LiteralOperations.h"

namespace pbox
{
	// Static list of literal resources.
	static std::list<ResourceInterface<std::string>*> LiteralResources;

	OperationController& OperationController::sharedInstance()
	{
		static OperationController operationController;

		return operationController;
	}

	bool OperationController::setupLiteralResources(const unsigned& countOfResources) const
	{
		if (this->getCountOfOperationsInProgress() > 0 || LiteralResources.size() > 0)
		{
			return false;
		}

		std::lock_guard<std::mutex> lockGuard(this->_controllerMutex);
		for (unsigned i = 0; i < countOfResources; i++)
		{
			ResourceInterface<std::string> *literal = new LiteralResource();
			unsigned amount = pbox_utils::generateRandomValue(1, 100);
			literal->generateResource(amount);
			LiteralResources.push_back(literal);
		}

		return true;
	}

	bool OperationController::printAllResources(std::ostream &ostr) const
	{
		if (this->getCountOfOperationsInProgress() > 0)
		{
			return false;
		}

		std::lock_guard<std::mutex> lockGuard(this->_controllerMutex);
		ostr << "------------------------------------" << std::endl;
		for (auto const &resource : LiteralResources)
		{
			resource->print(ostr);
		}
		ostr << "------------------------------------" << std::endl;

		return true;
	}

	bool OperationController::cleanUpResources()
	{
		if (this->getCountOfOperationsInProgress() > 0) {
			return false;
		}

		std::lock_guard<std::mutex> lockGuard(this->_controllerMutex);

		for (auto const &resource : LiteralResources) {
			delete resource;
		}

		LiteralResources.clear();

		return LiteralResources.size() == 0;
	}

	void OperationController::scheduleWriteOperation(const unsigned& countOfIterations)
	{
		std::lock_guard<std::mutex> lock(_mutexThreadsStorage);
		_threadsStorage.push_back(std::thread([&](unsigned countOfIterations) {
			while (countOfIterations > 0)
			{
				// Sleep for some amount of milliseconds before execute procedure.
				pbox_utils::sleepForRandomMilliseconds(100, 1000);

				LiteralWriteOperation operation(true, true);
				// Ensure first that there are no  reader / writer worker
				// threads in progress  and then lock and modify collection
				// atomically.

				std::unique_lock<std::mutex> lock(this->_controllerMutex);
				this->_conditionNoActive.wait(lock, [&]() {
					return !this || this->getCountOfOperationsInProgress() == 0;
				});

				operation(std::ref(LiteralResources));
				countOfIterations--;

				lock.unlock();
				this->_conditionNoActive.notify_all();
			}
		}, countOfIterations));
	}

	void OperationController::scheduleUpdateOperation(const unsigned& countOfIterations)
	{
		std::lock_guard<std::mutex> lock(_mutexThreadsStorage);
		_threadsStorage.push_back(std::thread([&](unsigned countOfIterations) {
			while (countOfIterations > 0)
			{
				// Sleep for some amount of milliseconds before execute procedure.
				pbox_utils::sleepForRandomMilliseconds(100, 1000);

				LiteralUpdateOperation operation(true, true);
				// Acquire lock on the main mutex.
				std::unique_lock<std::mutex> lock(this->_controllerMutex);

				// Choose random resource to operate on.
				ResourceInterface<std::string>* resource = nullptr;

				if (LiteralResources.size() > 0)
				{
					unsigned index = pbox_utils::generateRandomValue(0, LiteralResources.size() - 1);
					std::list<ResourceInterface<std::string>*>::iterator iterator = std::next(LiteralResources.begin(), index);
					resource = *iterator;
				}

				// Wait for availability.
				this->_conditionNoActive.wait(lock, [&resource]() {
					return !resource || (resource->getReadersCount() == 0 && resource->getWritersCount() == 0);
				});

				// Register update operation on current resource.
				this->registerUpdateOperation(resource);

				lock.unlock();

				// Process update operation.
				operation(resource);

				// Unregister operation and notify.
				this->unregisterUpdateOperation(resource);
				// Use 'notify_all' in order to give higher
				// priority to readers
				this->_conditionNoWriter.notify_all();
				this->_conditionNoActive.notify_one();

				countOfIterations--;
			}

		}, countOfIterations));
	}

	void OperationController::scheduleReadOperation(const unsigned& countOfIterations)
	{
		std::lock_guard<std::mutex> lock(_mutexThreadsStorage);
		this->_threadsStorage.push_back(std::thread([&](unsigned countOfIterations) {
			while (countOfIterations > 0)
			{
				// Sleep for some amount of milliseconds before execute procedure.
				pbox_utils::sleepForRandomMilliseconds(100, 1000);

				LiteralReadOperation operation(true, true);
				// Acquire lock on the main mutex.
				std::unique_lock<std::mutex> lock(this->_controllerMutex);
				// Choose random resource to operate on.
				ResourceInterface<std::string>* resource = nullptr;
				if (LiteralResources.size() > 0)
				{
					unsigned index = pbox_utils::generateRandomValue(0, LiteralResources.size() - 1);
					std::list<ResourceInterface<std::string>*>::iterator iterator = std::next(LiteralResources.begin(), index);
					resource = *iterator;
				}

				// Wait for read availability.
				this->_conditionNoWriter.wait(lock, [&resource]() {
					return !resource || resource->getWritersCount() == 0;
				});

				// Register read operation on current resource.
				this->registerReadOperation(resource);

				lock.unlock();

				// Process read operation.
				operation(std::ref(resource));

				// Unregister operation and notify.
				this->unregisterReadOperation(resource);
				this->_conditionNoActive.notify_one();

				countOfIterations--;
			}
		}, countOfIterations));
	}

	void OperationController::joinAllOperations()
	{
		std::lock_guard<std::mutex> lock(_mutexThreadsStorage);
		for (auto &t : this->_threadsStorage)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
	}

	void OperationController::detachAllOperations(bool performAsynchronous, void(*callback)())
	{
		auto detachAllOperations = [&, callback](void(*callback)()) {
			std::lock_guard<std::mutex> lock(_mutexThreadsStorage);
			for (auto iterator = this->_threadsStorage.begin(); iterator != this->_threadsStorage.end(); ++iterator)
			{
				iterator->detach();
				iterator = this->_threadsStorage.erase(iterator);
			}

			if (callback)
			{
				callback();
			}
		};

		if (performAsynchronous)
		{
			std::async(detachAllOperations, callback);
		}
		else
		{
			detachAllOperations(nullptr);
		}

	}

	void OperationController::removeThreadFromStorage(std::thread::id id)
	{
		std::lock_guard<std::mutex> lock(_mutexThreadsStorage);
		for (auto iterator = this->_threadsStorage.begin(); iterator != this->_threadsStorage.end(); ++iterator)
		{
			if (iterator->get_id() == id)
			{
				if (!iterator->joinable())
				{
					iterator->detach();
					iterator = this->_threadsStorage.erase(iterator);
				}
			}
		}
	}

} /* namespace pbox */
