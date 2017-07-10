
#ifndef __DLIB_H__
#define __DLIB_H__

#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define ERROR_SUCCESS 0
#define ERROR_FAILED -1

#define SEM_NAME "my_semaphore"

#define	FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

/* 定时器 */
int TIMER_CreateTime(timer_t *pstTimerId);
int TIMER_DelTime(timer_t *pstTimerId);
int TIMER_StopTime(timer_t *pstTimerId);
int TIMER_SetTime(timer_t *pstTimerId, struct itimerspec *pstValue);
int TIMER_GetTime(timer_t *pstTimerId, struct itimerspec *pstValue);

/* posix 信号量 */
int SEM_Init(sem_t **ppSem, char *pcSemName);
int SEM_Post(sem_t *pSem);
int SEM_Wait(sem_t *pSem);
int SEM_GetValue(sem_t *pSem, int *piValue);

/* 获取随机数 */
int GetRandNum(unsigned long *pulRandNum);

#endif /* __DLIB_H__ */

