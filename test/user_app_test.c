/* *****************************
 Name:     Alon Nov
 Exercise: watchdog
 Date:	   07.01.2024   
 Reviewer: Zack Molko

********************************/

#include <unistd.h>

#include "wd.h"


int main(int argc, char* argv[])
{
	WDStart(2, 3, argc, argv);

	sleep(15);

	WDStop();

	return 0;
}



