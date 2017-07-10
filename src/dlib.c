
/*
 * Copyright (C)
 * daemon function lib
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <semaphore.h>

#include "./log.h"
#include "./dlib.h"
#include "./daemon.h"

static void Timer_TimeOut(union sigval sig)
{

	LOG_Debug_Print(0, "Time out\n");

	return;
}

/*****************************************************************
** FUNCTION: TIMER_DelTime
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: delete the timer
*****************************************************************/
int TIMER_DelTime(timer_t *pstTimerId)
{
    int iErrCode = 0;

    iErrCode = timer_delete(*pstTimerId);

    return iErrCode;
}

/*****************************************************************
** FUNCTION: TIMER_CreateTime
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: create timer
*****************************************************************/
int TIMER_CreateTime(timer_t *pstTimerId)
{
	int iErrCode = ERROR_SUCCESS;
	struct sigevent sev;

	memset(&sev, 0, sizeof(struct sigevent));

	sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_int = 0;
    sev.sigev_notify_function = Timer_TimeOut;
    sev.sigev_notify_attributes = NULL;

	iErrCode = timer_create(CLOCK_REALTIME, &sev, pstTimerId);
	if (ERROR_SUCCESS != iErrCode)
	{
		LOG_Error_Print(0, "timer create is failed\n");
	}

	return iErrCode;
}

/*****************************************************************
** FUNCTION: TIMER_StopTime
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: disarm the timer
*****************************************************************/
int TIMER_StopTime(timer_t *pstTimerId)
{
	struct itimerspec its;
	int iErrCode = ERROR_SUCCESS;

	memset(&its, 0, sizeof(struct itimerspec));

	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

	iErrCode = timer_settime(*pstTimerId, 0, &its, NULL);
	if (ERROR_SUCCESS != iErrCode)
	{
		LOG_Error_Print(0, "timer settime stop is failed\n");
	}
	else
	{
		LOG_Debug_Print(0, "timer settime stop is successful\n");
	}

	return iErrCode;
}

/*****************************************************************
** FUNCTION: TIMER_SetTime
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: set the timer
*****************************************************************/
int TIMER_SetTime(timer_t *pstTimerId, struct itimerspec *pstValue)
{
	struct itimerspec its;
	int iErrCode = ERROR_SUCCESS;

	memset(&its, 0, sizeof(struct itimerspec));

	its.it_value.tv_sec = pstValue->it_value.tv_sec;
	its.it_value.tv_nsec = pstValue->it_value.tv_nsec;
    its.it_interval.tv_sec = pstValue->it_interval.tv_sec;
    its.it_interval.tv_nsec = pstValue->it_interval.tv_nsec;

	iErrCode = timer_settime(*pstTimerId, 0, &its, NULL);
	if (ERROR_SUCCESS != iErrCode)
	{
		LOG_Error_Print(0, "timer settime is failed\n");
	}
    else
    {
        LOG_Debug_Print(0, "timer settime is success\n");
    }

	return iErrCode;
}

/*****************************************************************
** FUNCTION: TIMER_GetTime
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: get the timer
*****************************************************************/
int TIMER_GetTime(timer_t *pstTimerId, struct itimerspec *pstValue)
{
    int iErrCode = ERROR_SUCCESS;

    /* 距离下次到时的时间记录在 pstValue->it_value.tv_sec, pstValue->it_value.tv_nsec 里面 ，两个值相加。*/
    iErrCode = timer_gettime(*pstTimerId, pstValue);

    LOG_Debug_Print(0, "pstValue->it_value.tv_sec=%lu, %ld, pstValue->it_interval.tv_sec=%lu, %ld\n",
                  pstValue->it_value.tv_sec, pstValue->it_value.tv_nsec,
                  pstValue->it_interval.tv_sec, pstValue->it_interval.tv_nsec);

    return iErrCode;
}

/*****************************************************************
** FUNCTION: SEM_GetValue
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: get the semaphore value
*****************************************************************/
int SEM_GetValue(sem_t *pSem, int *piValue)
{
    int iErrCode = ERROR_SUCCESS;

    iErrCode = sem_getvalue(pSem, piValue);

    return iErrCode;
}

/*****************************************************************
** FUNCTION: SEM_Post
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: post the semaphore
*****************************************************************/
int SEM_Post(sem_t *pSem)
{
	int iErrCode = 0;

	iErrCode = sem_post(pSem);
	if (ERROR_SUCCESS != iErrCode)
	{
		LOG_Error_Print(0, "sem post is failed\n");
	}
	else
	{
		LOG_Debug_Print(0, "sem post is successful\n");
	}
	return iErrCode;
}

/*****************************************************************
** FUNCTION: SEM_Wait
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: wait the semaphore
*****************************************************************/
int SEM_Wait(sem_t *pSem)
{
	int iErrCode = 0;

	iErrCode = sem_wait(pSem);
	if (ERROR_SUCCESS != iErrCode)
	{
		LOG_Error_Print(0, "sem wait is failed\n");
	}
	else
	{
		LOG_Debug_Print(0, "sem wait is successful\n");
	}

	return iErrCode;
}

/*****************************************************************
** FUNCTION: SEM_Init
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: init the semaphore
*****************************************************************/
int SEM_Init(sem_t **ppSem, char *pcSemName)
{
	sem_t *pSem = NULL;
	int iErrCode = ERROR_SUCCESS;

	iErrCode = sem_unlink(pcSemName);
	if (ERROR_SUCCESS != iErrCode)
	{
        iErrCode = ERROR_FAILED;
		LOG_Error_Print(0, "sem_unlink is failed\n");
        return iErrCode;
	}

    /* 已经存在的信号量 sem_open(pcSemName, O_CREAT); 就可以了 */
	pSem = sem_open(pcSemName, O_CREAT, FILE_MODE, 0);
	if (SEM_FAILED == pSem)
	{
        iErrCode = ERROR_FAILED;
		LOG_Error_Print(0, "open sem failed\n");
	}

	*ppSem = pSem;

	return iErrCode;
}

/*****************************************************************
** FUNCTION: GetRandNum
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: get random number
*****************************************************************/
int GetRandNum(unsigned long *pulRandNum)
{
    int iFd = 0;
	int iLen = 0;
	unsigned long ulSeed = 0;
    int iErrCode = ERROR_SUCCESS;

    /* /dev/random 会阻塞 */
	iFd = open("/dev/urandom", O_RDONLY);
	if (ERROR_FAILED == iFd)
	{
		LOG_Error_Print(0, "error: open /dev/urandom is failed\n");
		iErrCode = ERROR_FAILED;
	}
	else
	{
		iLen = read(iFd, &ulSeed, sizeof(ulSeed));
		if (ERROR_FAILED == iLen)
		{
			iErrCode = ERROR_FAILED;
			LOG_Error_Print(0, "error: read is failed\n");
		}
		else
		{
            *pulRandNum = ulSeed;
		}

		close(iFd);
	}

    return iErrCode;
}

#if 0
int main(int argc, char *argv[])
{
    timer_t stTimerId;
    struct itimerspec stValue;
    struct itimerspec its;

    memset(&stValue, 0, sizeof(stValue));

    Timer_CreateTime(&stTimerId);

    memset(&its, 0, sizeof(its));
	its.it_value.tv_sec = (time_t)3;
	its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = (time_t) 5;
    its.it_interval.tv_nsec = 0;
    Timer_SetTime(&stTimerId, &its);

    Timer_GetTime(&stTimerId, &stValue);
    sleep(19);
    Timer_GetTime(&stTimerId, &stValue);
    sleep(1);
    printf("sleep is ok, set 2 && 4 begin\n");

    memset(&its, 0, sizeof(its));
	its.it_value.tv_sec = (time_t)2;
	its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = (time_t) 4;
    its.it_interval.tv_nsec = 0;
    Timer_SetTime(&stTimerId, &its);

    Timer_GetTime(&stTimerId, &stValue);
    sleep(20);
    printf("sleep is ok, stop is begin\n");
    Timer_StopTime(&stTimerId);
    Timer_GetTime(&stTimerId, &stValue);
    sleep(20);
    printf("sleep is ok\n");
    Timer_DelTime(&stTimerId);

    int iValue = 0;
    sem_t *pSem = NULL;
    Sem_Init(&pSem, SEM_NAME);
    Sem_GetValue(pSem, &iValue);
    LOG_Debug_Print(0, "sem1 init is ok, wait sem_wait, value = %d\n", iValue);
    Sem_Wait(pSem);
    Sem_GetValue(pSem, &iValue);
    LOG_Debug_Print(0, "sem1_wait is ok, after sleep 5 sem_post, value = %d\n", iValue);
    sleep(5);
    Sem_Post(pSem);
    Sem_GetValue(pSem, &iValue);
    LOG_Debug_Print(0, "sem1_post is ok, value = %d\n", iValue);

    return 0;
}
#endif

