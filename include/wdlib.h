/* *****************************
 Name:     Alon Nov
 Exercise: watchdog
 Date:	   07.01.2024   
 Reviewer: Zack Molko

********************************/

#ifndef _WDLIB_H_ 
#define _WDLIB_H_

#include <stdlib.h> 	/* malloc, free, NULL, */
#include <stdio.h>		/* puts, fflush, stdout */
#include <semaphore.h>	/* sem_t, sem_open, sem_close, sem_post, sem_wait, sem_unlink */
#include <fcntl.h> 		/* O_CREAT */
#include <stdatomic.h>	/* sig_atomic_t, atomic_store, atomic_load, atomic_fetch_add */
#include <string.h> 	/* strcpy, strlen */
#include <sys/wait.h>	/* pid_t, siginfo_t, struct sigaction, fork, execvp, getpid, 
						   kill, raise, sigaction, waitpid, sleep, pthread_create, pthread_join, SIGUSR1, SIGUSR2 */

#include "uid.h"		  /* UnIDIsEqual */
#include "scheduler.h" /* SchedulerCreate, SchedulerDestroy, SchedulerAddTask*/

typedef struct thread_arg
{
	size_t interval;
	size_t threshold;
	int argc;
	char** argv;
}args_t;

int StartHelper(void* args);
void StopHelper(void);
int WDHelper(args_t args);
void* Thread(void* args);
int Init(void* args);
int DogInit(void);
int SigInit();
int SchedInit(void* args);
int ArgvInit(void* args);
void Handler(int signum);
time_t Checkup(void* args);
time_t CheckTimeout(void* args);

#endif /* _WDLIBS_H_ */