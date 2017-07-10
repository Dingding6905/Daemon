
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

#include "../src/log.h"
#include "../src/dlib.h"
#include "../src/daemon.h"

int main(int argc, char *argv[])
{
#if 0
    /* ��ʱ�� demo */
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
#endif

    /* �ź��� demo */
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

