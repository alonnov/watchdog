/* *****************************
 Name:     Alon Nov
 Exercise: watchdog
 Date:	   07.01.2024   
 Reviewer: Zack Molko

********************************/

#include "wdlib.h" 		/* private API */
#include "wd.h"			/* SUCCESS, FAILURE */

#define NUM_ARGS (3)
#define PATH ("./watchdog.out")
#define SIZE (20)

volatile sig_atomic_t g_counter = 0;
pthread_t g_tid = 0;
scheduler_t* g_sched = NULL;
char** g_argv = NULL;
char g_argv0[SIZE] = {0};
char g_argv1[SIZE] = {0};
char g_argv2[SIZE] = {0};
pid_t g_wdpid = 0;
pid_t g_upid = 0;
sem_t* sem1 = NULL;
sem_t* sem2 = NULL;
char* sem1_name = "sem1";
char* sem2_name = "sem2";

int StartHelper(void* args)
{
	g_upid = getpid();

	if(FAILURE == ArgvInit(args))
	{
	#ifndef NDEBUG
		puts("user ArgvInit error");
	#endif
		return FAILURE;
	}
	
	if(FAILURE == Init(args))
	{
	#ifndef NDEBUG
		puts("user Init error");
	#endif
		return FAILURE;
	}

	if(FAILURE == DogInit())
	{
	#ifndef NDEBUG
		puts("user DogInit error");
	#endif
		SchedulerDestroy(g_sched);
		StopHelper();
		return FAILURE;
	}

	if(0 != pthread_create(&g_tid, NULL, Thread, NULL))
	{
	#ifndef NDEBUG
		puts("user pthread_create error");
	#endif
		SchedulerDestroy(g_sched);
		StopHelper();
		return FAILURE;
	}

	return SUCCESS;
}


void StopHelper(void)
{
	raise(SIGUSR2);
	kill(g_wdpid, SIGUSR2);
	pthread_join(g_tid, NULL);
	waitpid(g_wdpid, NULL, 0);
	sem_unlink(sem1_name);
	sem_unlink(sem2_name);
	free(g_argv);
}


int WDHelper(args_t args)
{
	g_upid = getppid();
	g_wdpid = getpid();

	g_argv = args.argv + NUM_ARGS;
	
	if(FAILURE == Init(&args))
	{
	#ifndef NDEBUG
		puts("wd Init error");
	#endif
		return FAILURE;
	}
	
	kill(g_upid, SIGCONT);	
	sem_post(sem2);
	sem_wait(sem1);
	sem_close(sem1);
	sem_close(sem2);
	SchedulerRun(g_sched);

	SchedulerDestroy(g_sched);

	return SUCCESS;
}


void* Thread(void* param)
{	
	(void)param;

	sem_post(sem1);
	sem_wait(sem2);
	sem_close(sem1);
	sem_close(sem2);
	SchedulerRun(g_sched);

	SchedulerDestroy(g_sched);

	return NULL;
}


int ArgvInit(void* args)
{
	args_t temp = *(args_t*)args;
	int i = NUM_ARGS;
	int new_argc = temp.argc + NUM_ARGS;

	g_argv = (char**)malloc(sizeof(char*) * (new_argc + 1));	
	if(NULL == g_argv)
	{
		return FAILURE; 
	}

	strcpy(g_argv0, PATH);
	sprintf(g_argv1, "%ld", temp.interval);
	sprintf(g_argv2, "%ld", temp.threshold);

	g_argv[0] = g_argv0;
	g_argv[1] = g_argv1;
	g_argv[2] = g_argv2;
	
	for(; i < new_argc; ++i)
	{
		g_argv[i] = temp.argv[i - NUM_ARGS];
	}
	g_argv[new_argc] = NULL;

	return SUCCESS;
}


int Init(void* args)
{
	if(FAILURE == SigInit() || FAILURE == SchedInit(args))
	{
		return FAILURE;	
	}

	sem1 = sem_open(sem1_name, O_CREAT, 0644, 0);
	sem2 = sem_open(sem2_name, O_CREAT, 0644, 0);

	return SUCCESS;
}


int DogInit(void)
{
	g_wdpid = fork();
	if(-1 == g_wdpid)
	{
		return FAILURE;
	} 
	
	if(0 == g_wdpid)
	{
		if(-1 == execvp(g_argv[0], g_argv))
		{
			return FAILURE;
		} 
	}

	raise(SIGSTOP);
	
	return SUCCESS;
}


int SigInit()
{
	struct sigaction act = {0};

	act.sa_handler = Handler;

	if(-1 == sigaction(SIGUSR1, &act, NULL) || -1 == sigaction(SIGUSR2, &act, NULL)) 
	{
		return FAILURE;
	} 

	return SUCCESS;
}


int SchedInit(void* args)
{
	unid_t bad_uid = {0};

	g_sched = SchedulerCreate();
	if(NULL == g_sched)
	{
		return FAILURE;
	}
	
	if(UnIDIsEqual(bad_uid, SchedulerAddTask(g_sched, Checkup, NULL, 1, 
					args, NULL)) || 
	   UnIDIsEqual(bad_uid, SchedulerAddTask(g_sched, CheckTimeout, NULL, 1, 
					 args, NULL)))
	{
		SchedulerDestroy(g_sched);
		StopHelper();
		return FAILURE;
	}

	return SUCCESS;				 
}


void Handler(int signum)
{
	if(SIGUSR1 == signum)
	{
	#ifndef NDEBUG
		puts("user recieved 1");
	#endif		
		atomic_store(&g_counter, 0);
	}
	else
	{
	#ifndef NDEBUG
		puts("user recieved 2");
	#endif
		SchedulerStop(g_sched);
	}
}


time_t Checkup(void* args)
{
	pid_t pid = getpid() == g_upid ? g_wdpid : g_upid;
	atomic_fetch_add(&g_counter, 1);
	kill(pid, SIGUSR1);

	return (time_t)((args_t*)args)->interval;
}


time_t CheckTimeout(void* args)
{
	args_t temp = *(args_t*)args;

	if((int)temp.threshold <= atomic_load(&g_counter))
	{	
		if(getpid() == g_upid)
		{
		#ifndef NDEBUG
			puts("WD DEAD");
			fflush(stdout);
		#endif
			atomic_store(&g_counter, 0);
			waitpid(g_wdpid, NULL, 0);
			DogInit();
		}
		else
		{
		#ifndef NDEBUG
			puts("USER DEAD");
			fflush(stdout);
		#endif
			if(-1 == execvp(temp.argv[0], temp.argv))
			{
			#ifndef NDEBUG
				puts("wd user restart error");
			#endif
				SchedulerDestroy(g_sched);
				StopHelper();
				return 0;
			}
		}		
	}

	return (time_t)(temp.interval * temp.threshold);
}

