/* *****************************
 Name:     Alon Nov
 Exercise: watchdog
 Date:	   07.01.2024   
 Reviewer: Zack Molko

********************************/

#include "wdlib.h" /* private API */

int main(int argc, char* argv[])
{
	args_t args = {0};

	args.interval = (size_t)atoi(argv[1]);
	args.threshold = (size_t)atoi(argv[2]);
	args.argc = argc;
	args.argv = argv;
	
	return WDHelper(args);
}








