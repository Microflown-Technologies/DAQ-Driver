
#include "Manager.h"

#include <windows.h>

// Define ENABLE_VIRTUAL_TERMINAL_PROCESSING, if it isn't defined.
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
Manager manage;

void testFunction(Events hallo) {
	std::string* test = NULL;

	switch (hallo) {

	case DISCONNECT:
		std::cout << "EVENT DISCONNECT" << std::endl;
		break;

	case CONNECT:
		std::cout << "EVENT CONNECT" << std::endl;
		break;

	case DATARECEIVED:
		std::cout << "EVENT DATARECEIVED" << std::endl;
		break;

	case DATAREADY:
		test = manage.getData();
		if (test->size()) {			
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			std::cout << *test << std::endl;
		}
		manage.provideStorageVector(test);
		break;

	case NEWCONFIGURATION:
		std::cout << "EVENT NEWCONFIGURATION" << std::endl;
		break;

	case ALLBUFFERSUSED:
		std::cout << "EVENT ALLBUFFERSUSED" << std::endl;
		break;

	case _ERROR:
		std::cout << "EVENT _ERROR" << std::endl;
		break;

	default:
		std::cout << "EVENT UNKNOWN" << std::endl;
		break;

	}
}

int main()
{


	manage.setLibraryEventCallback(&testFunction);

	getchar();

	if (manage.isVoyagerConnected()) {
		std::cout << "De Voyager is connected!" << std::endl;	}
	else {
		std::cout << "De Voyager is niet connected" << std::endl;
	}	

	std::string* test1 = new std::string;
	std::string* test2 = new std::string;
	std::string* test3 = new std::string;

	manage.provideStorageVector(test1);
	manage.provideStorageVector(test2);
	manage.provideStorageVector(test3);

	manage.start();


	getchar();
	return 0;
}