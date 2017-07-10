#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "./daemon.h"
#include "./worker.h"
#include "./log.h"
#include "./dlib.h"

static WORKER_MAINPTHREAD_INFO_S g_stWorkerMainPthread;

void *worker_pthread_start()
{
    int i = 0;
    int iErrCode = ERROR_SUCCESS;

    for (i = 0; i < WORKER_PTHREAD_MAX_NUM; i++)
    {
        if (0 == pthread_equal(g_stWorkerPthread[i].ulThreadId, pthread_self()))
        {
            continue;
        }

        iErrCode = LOG_InitOpen(&g_stWorkerPthread[i].stLogFd.iDbgFd, LOG_DEBUG);
        iErrCode |= LOG_InitOpen(&g_stWorkerPthread[i].stLogFd.iTrcFd, LOG_TRACE);
        iErrCode |= LOG_InitOpen(&g_stWorkerPthread[i].stLogFd.iErrFd, LOG_ERROR);
        LOG_Debug(0, g_stWorkerPthread[i].stLogFd.iDbgFd, "----pthread id is %lu\n", (unsigned long) pthread_self());

        break;
    }

    return ((void *)0);
}

int worker_pthread_init(int iPthreadNum)
{
    int i = 0;
    int iErrCode = ERROR_SUCCESS;

    memset(g_stWorkerPthread, 0, sizeof(g_stWorkerPthread));

    for (i = 0; i < iPthreadNum; i++)
    {
        iErrCode |= pthread_create(&g_stWorkerPthread[i].ulThreadId, NULL, worker_pthread_start, NULL);
    }

    for (i = 0; i < iPthreadNum; i++)
    {
        LOG_Debug(0, g_stWorkerMainPthread.stLogFd.iDbgFd, "g_stWorkerPthread[%d].ulThreadId=%lu\n", i, g_stWorkerPthread[i].ulThreadId);
    }

    return iErrCode;
}

static int worker_dlog_Init()
{
    int iErrCode = ERROR_SUCCESS;

    memset(&g_stWorkerMainPthread, 0, sizeof(g_stWorkerMainPthread));

    iErrCode = LOG_InitOpen(&g_stWorkerMainPthread.stLogFd.iDbgFd, LOG_DEBUG);
    iErrCode |= LOG_InitOpen(&g_stWorkerMainPthread.stLogFd.iTrcFd, LOG_TRACE);
    iErrCode |= LOG_InitOpen(&g_stWorkerMainPthread.stLogFd.iErrFd, LOG_ERROR);

    return iErrCode;
}

int worker_init()
{
    int iErrCode = ERROR_SUCCESS;

    worker_dlog_Init();

    iErrCode = worker_pthread_init(WORKER_PTHREAD_MAX_NUM);
    if (ERROR_SUCCESS != iErrCode)
    {
        perror("create worker pthread is failed\n");
    }

    return iErrCode;
}

int main(int argc, char *argv[])
{
    worker_init();
    LOG_Debug(1, g_stWorkerMainPthread.stLogFd.iDbgFd, "worker process is end\n");

    sleep(200);
    return 0;
}
