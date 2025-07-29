/********************************
 Name:     Alon Nov
 Exercise: scheduler 
 Date:	   12.11.2024   
 Reviewer: Liad Tiblum

********************************/

#include <stdlib.h>  /* size_t, malloc, free */
#include <string.h>  /* memset */
#include <assert.h>  /* assert */

#include "task.h"

struct task
{
    unid_t unid;
    time_t when;
    void* operation_params;
    operation_func_t operation;    
    void* cleanup_params;
    cleanup_func_t cleanup;
};


/**************************TaskCreate************************************/
task_t* TaskCreate(operation_func_t operation, cleanup_func_t cleanup, 
				   time_t when, void* opt_params, void* cleanup_params)
{
	task_t* task = NULL;
	
	assert(operation);
	assert(when);
	assert(opt_params);
		
	task = (task_t*)malloc(sizeof(task_t));
	if(!task)
	{
		return NULL;
	}
	
	task->unid = UnIDGenerate();
	if(UnIDIsEqual(task->unid, bad_unid))
	{
		free(task);
		return NULL;
	}

	task->when = when;
	task->operation_params = opt_params;
	task->operation = operation;
	task->cleanup_params = cleanup_params;
	task->cleanup = cleanup;
			
	return task;						
}	


/**************************TaskGetUnID**************************************/	
unid_t TaskGetUnID(task_t* task)
{	
	assert(task);
	
	return task->unid;
}		
		

/***************************TaskGetTime*************************************/	
time_t TaskGetTime(task_t* task)
{
	assert(task);
	
	return task->when;	
}	


/**************************TaskSetTime**************************************/
void TaskSetTime(task_t* task, time_t when)
{
	assert(task);	
	
	task->when = when;
}


/***************************TaskRun***************************************/
time_t TaskRun(task_t* task)
{
	assert(task);
	
	return task->operation(task->operation_params);
}


/***************************TaskClean***************************************/
void TaskClean(task_t* task)
{
	assert(task);
	
	if(task->cleanup)
	{
		task->cleanup(task->cleanup_params);
	}
	free(task);	
}


