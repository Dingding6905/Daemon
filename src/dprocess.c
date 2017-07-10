#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>

#include "./daemon.h"
#include "./log.h"
#include "./dsignal.h"
#include "./dprocess.h"
#include "./dlib.h"

static int daemon_Execute_Proc()
{
    int iErrCode = ERROR_SUCCESS;

    char *pcNewEviron[] = {NULL};
    /* pcNewArgv[0]:启动的子进程的名字可自己定义，pcNewArgv[1]...[2]...[3]:传入子进程的参数 */
    char *pcNewArgv[] = {"mydaemond", ":wroker process", NULL, NULL};

    /* int execve(const char *filename, char *const argv[],char *const envp[]);
     * filename:可执行文件名;argv[]:传入filename的多个参数;*/
    iErrCode = execve("./worker", pcNewArgv, pcNewEviron);

    return iErrCode;
}

static void daemon_WorkerProcess(int *piSockFd)
{
    char cBuf2[64];

    close(piSockFd[0]);
    memset(cBuf2, 0, sizeof(cBuf2));
    snprintf(cBuf2, sizeof(cBuf2), "%lu", (unsigned long)getpid());
    send(piSockFd[1], cBuf2, strlen(cBuf2), 0);
#if 0
    memset(cBuf2, 0, sizeof(cBuf2));
    memset(cBuf, 0, sizeof(cBuf));
    snprintf(cBuf, sizeof(cBuf), "%lu.txt", getpid());
    int iFd = 0;
    iFd = open(cBuf, O_RDWR|O_CREAT|O_APPEND/*|O_TRUNC*/, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    snprintf(cBuf2, sizeof(cBuf2), "fd[0]=%d, fd[1]=%d\n", piSockFd[0], piSockFd[1]);
    write(iFd, cBuf2, strlen(cBuf2));

    for ( ; ; )
    {
        memset(cBuf2, 0, sizeof(cBuf2));
        read(piSockFd[0], cBuf2, sizeof(cBuf2));
        write(iFd, cBuf2, strlen(cBuf2));
    }

    int i = 0;
    while (i < 10)
    {
        sleep(10);
        //break;
    }
    //abort();

    //exit(0);
#endif

}

/*****************************************************************
** FUNCTION: os_CreateWorker
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: create worker process
*****************************************************************/
static int daemon_CreateWorker(WORKER_INFO_S *pstWorkerInfo)
{
    pid_t iPid;
    //int iSockFd[2];
    int iErrCode = ERROR_SUCCESS;

    //向fd[0]写入，可从fd[1]读取；向fd[1]写入，可从fd[0]读取
    if (0 != socketpair(AF_UNIX, SOCK_STREAM, 0, pstWorkerInfo->iSockFd))
    {
        perror("error, socketpair is failed\n");
        iErrCode = ERROR_FAILED;

        return iErrCode;
    }

    iPid = fork();
    switch (iPid)
    {
        case -1:
            perror("error, fork worker process is failed\n");
            iErrCode = ERROR_FAILED;
            break;

        case 0:
            daemon_WorkerProcess(pstWorkerInfo->iSockFd);
            daemon_Execute_Proc();
            break;

        default:
            //pstWorkerInfo->iSockFd[1]可读可写，和子进程进行通信
            close(pstWorkerInfo->iSockFd[1]);
            break;
    }

    return iErrCode;
}

int DAEMON_Worker_Create(int iIndex)
{
    char *pcStr = NULL;
    char cPid[PID_MAX_LEN];
    int iErrCode = ERROR_SUCCESS;

    memset(cPid, 0, sizeof(cPid));
    memset(&g_stWorkerNode[iIndex], 0, sizeof(WORKER_INFO_S));

    iErrCode = daemon_CreateWorker(&g_stWorkerNode[iIndex]);
    recv(g_stWorkerNode[iIndex].iSockFd[0], cPid, sizeof(cPid), 0);
    g_stWorkerNode[iIndex].ulPid = strtol(cPid, &pcStr, 10);
    //printf("g_stWorkerNode[%d].ulPid=%d\n", iIndex, g_stWorkerNode[iIndex].ulPid);

    return iErrCode;
}


