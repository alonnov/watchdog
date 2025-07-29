/* *****************************
 Name:     Alon Nov
 Exercise: watchdog
 Date:	   07.01.2024   
 Reviewer: Zack Molko

********************************/

#ifndef _wd_H_ 
#define _wd_H_

#include <stddef.h> /* size_t */

typedef enum status
{
	SUCCESS,
	FAILURE
}status_t;

/*@Desc - creates a watchdog for the calling process
* @Param - interval: interval (in seconds) of timeout check
* @Param - threshold: amount timeout checks to return negative before an action
		   is taken
* @Param - argc: number of command line arguments given
* @Param - argv: array of command line argument strings
* @Return - on succes, returns SUCCESS. if creation fails, returns FAILED, 
* @Warning - interval must be larger than 1
* @time complexity - O(1)  
* @Space complexity - O(1) */
status_t WDStart(size_t interval, size_t threshold, int argc, char* argv[]);

/*@Desc - stops the calling process' active watchdog
* @Param - N/A
* @Return - N/A
* @time complexity - O(1)  
* @Space complexity - O(1) */
void WDStop(void);

#endif /* _wd_H_ */