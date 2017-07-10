
#ifndef __WORKER_H__
#define __WORKER_H__

#include "./log.h"

#define WORKER_PTHREAD_MAX_NUM 5

typedef struct tagWorkerPthreadInfo
{
    pthread_t ulThreadId;
    LOG_FD_NAME_S stLogFd;
}WORKER_PTHREAD_INFO_S;

typedef struct tagWorkerMainPthreadInfo
{
    pthread_t ulThreadId;
    LOG_FD_NAME_S stLogFd;
}WORKER_MAINPTHREAD_INFO_S;

WORKER_PTHREAD_INFO_S g_stWorkerPthread[WORKER_PTHREAD_MAX_NUM];

#endif /* __WORKER_H__ */

