/* *****************************
 Name:     Alon Nov
 Exercise: watchdog
 Date:	   07.01.2024   
 Reviewer: Zack Molko

********************************/

#include "wd.h" 		/* public API */
#include "wdlib.h"	  	/* private API */

status_t WDStart(size_t interval, size_t threshold, int argc, char* argv[])
{
	args_t args = {0};

	args.interval = interval;
	args.threshold = threshold;
	args.argc = argc;
	args.argv = argv;

	return StartHelper(&args);
}


void WDStop(void)
{
	StopHelper();
}




