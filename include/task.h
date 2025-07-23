/********************************
 Name:     Alon Nov
 Exercise: scheduler 
 Date:	   12.11.2024   
 Reviewer: Liad Tiblum

********************************/

#ifndef _TASK_H_ 
#define _TASK_H_

#include <stddef.h> /* size_t */
#include <time.h>	/* time_t */

#include "uid.h" 

typedef struct task task_t;

/*@Desc - runs an operation with givin parameters
* @Params - params: paramater for operation
* @Return - 0 (success)
* @Return - non 0 value (failure)
* @Return - scheduler context: returns 0 if function is single use or time(seconds) until next run
* @Return - 0 if iterator_data and params are equal */
typedef time_t (*operation_func_t)(void* params);

/*@Desc - deletes any data/files created by the task
* @Params - params: paramater for cleanup if needed
* @Return - N/A */
typedef void (*cleanup_func_t)(void* params);


/*@Desc - creates a task
* @Params - operation: operation function (created by the user)
					   the created task will do.
* @Params - cleanup: cleanup function (created by the user) 
					 the created task will have.
					 cleans all temporary files/data created by the 
					 operation function 
* @Params - when: time for the created task to start at
* @Params - opt_params: given operation function parameter
* @Params - cleanup_params: given cleanup function parameter
* @Return - uuid of the added task
* @Return - bad uuid ({0})
* @Warning - N/A
* @Warning - cleanup parameter can be NULL(no cleanup needed)
* @time complexity - O(1)   
* @Space complexity - O(1) */
task_t* TaskCreate(operation_func_t operation, cleanup_func_t cleanup, time_t when, void* opt_params, void* cleanup_params);

/*@Desc - gets given task's uuid
* @Params - task: task to get uuid of
* @Return - task's uuid
* @Warning - if task param is not a pointer to a valid task,
			  can cause UB
* @time complexity - O(1) 
* @Space complexity - O(1) */
unid_t TaskGetUnID(task_t* task);

/*@Desc - gets given task's start time
* @Params - task: task to get start time of
* @Return - task's start time
* @Warning - if task param is not a pointer to a valid task,
			  can cause UB
* @time complexity - O(1) 
* @Space complexity - O(1) */
time_t TaskGetTime(task_t* task);

/*@Desc - changes given task's start time
* @Params - task: task to change start time of
* @Params - when: task's new start time
* @Return - N/A
* @Warning - if task param is not a pointer to a valid task,
			  can cause UB
* @Warning - when param must be higher than current time (time(NULL))
* @time complexity - O(1) 
* @Space complexity - O(1) */
void TaskSetTime(task_t* task, time_t when);

/*@Desc - executes given task's operation function
* @Params - task: task to execute
* @Return - 0 on success
* @Return - non 0 value on failure
* @return - scheduler context: returns 0 if function is single use or time(seconds) until next run  
* @Warning - if task param is not a pointer to a valid task,
			  can cause UB			  
 			  
* @time complexity - O(1)
* @Space complexity - O(1) */
time_t TaskRun(task_t* task); 


/*@Desc - removes given task
* @Params - task: task to remove
* @Return - N/A
* @Warning - if task param is not a pointer to a valid task,
			 can cause UB			  
* @Warning - N/A			  
* @time complexity - O(1) 
* @Space complexity - O(1) */
void TaskClean(task_t* task);
	
 
#endif
