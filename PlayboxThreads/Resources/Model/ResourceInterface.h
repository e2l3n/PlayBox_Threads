/*
 * ResourceInterface.h
 *
 *  Created on: Jan 14, 2018
 *      Author: e2l3n
 */

#ifndef RESOURCEINTERFACE_H_
#define RESOURCEINTERFACE_H_

#include <iostream>

namespace pbox
{
	template<class U>
	class OperationCounter;

	template<class T>
	class ResourceInterface
	{
	public:
		ResourceInterface() : _readersInProgress(0), _writersInProgress(0) {}

		ResourceInterface(int readersInProgress, int writersInProgress) : _readersInProgress(readersInProgress), _writersInProgress(writersInProgress) {}

		virtual ~ResourceInterface() = default;

		virtual T getResourceCopy() const = 0;

		virtual unsigned getResourceSize() const = 0;

		virtual void generateResource(const unsigned& amount = 0) = 0;

		virtual void setResource(const T& literal) = 0;

		virtual void print(std::ostream& ostr) const = 0;

	protected:
		friend class OperationCounter<T>;
		friend class OperationController;
		friend class LiteralWriteOperation;

		inline virtual int getReadersCount() const
		{
			return _readersInProgress;
		}

		inline virtual int getWritersCount() const
		{
			return _writersInProgress;
		}

		inline virtual int registerReader()
		{
			return ++_readersInProgress;
		}

		inline virtual int unregisterReader()
		{
			return --_readersInProgress;
		}

		inline virtual int registerWriter()
		{
			return ++_writersInProgress;
		}

		inline virtual int unregisterWriter()
		{
			return --_writersInProgress;
		}

	private:
		std::atomic<int> _readersInProgress;
		std::atomic<int> _writersInProgress;
	};

}
;
/* namespace pbox */

#endif /* RESOURCEINTERFACE_H_ */
