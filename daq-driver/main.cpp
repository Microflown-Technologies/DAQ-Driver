
#include "Manager.h"

#include <windows.h>

// Define ENABLE_VIRTUAL_TERMINAL_PROCESSING, if it isn't defined.
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif


void testFunction(Events) {

	std::cout << "Event is binnengekomen" << std::endl;
}



int main()
{

	Manager manage;
	manage.setLibraryEventCallback(&testFunction);

	Sleep(2000);

	manage.start();

	Sleep(100000);

	return 0;
}