//============================================================================
// Name        : PlayBox_Threads.cpp
// Author      : Toma Popov
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <chrono>
#include <thread>

#include "Operations/Controller/OperationController.h"
#include "Resources/Model/LiteralResource.h"

using namespace std;
using namespace pbox;

#include <list>

int main() {

	OperationController::sharedInstance().setupLiteralResources(10);
	OperationController::sharedInstance().printAllResources();

	// Schedule 10 threads with 50 iterations that modify random literal resources.
	for (unsigned i = 0; i < 10; i++)
	{
		OperationController::sharedInstance().scheduleUpdateOperation(50);
	}

	// Schedule 20 threads each with 50 iterations that read random resources.
	for (unsigned i = 0; i < 20; i++)
	{
		OperationController::sharedInstance().scheduleReadOperation(50);
	}

	// Schedule 10 thread with 50 iterations that inserts into / deletes from collection.
	for (unsigned i = 0; i < 10; i++) 
	{
		OperationController::sharedInstance().scheduleWriteOperation(50);
	}

	OperationController::sharedInstance().joinAllOperations();
	OperationController::sharedInstance().printAllResources();

	std::cout << "Finished.";

	return 0;
}
