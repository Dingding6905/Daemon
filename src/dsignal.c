#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "./daemon.h"
#include "./log.h"
#include "./dsignal.h"
#include "./dprocess.h"
#include "./dlib.h"

static void daemon_sig_ChildHandler(int iSigno)
{
    int i = 0;
    pid_t iPid;
    int iStatus = 0;

    if (SIGCHLD == iSigno)
    {
        for ( ; ; )
        {
            /* WNOHANG 如果没有任何已经结束的子进程则马上返回，不予以等待 */
            iPid = waitpid(-1, &iStatus, WNOHANG);
            if (0 < iPid)
            {
                for (i = 0; i < PID_MAX_NUM; i++)
                {
                    //printf("signal signal iSigno=%d, SIGCHLD=%d, iPid=%d,g_stWorkerNode[%d].ulPid=%d\n", iSigno, SIGCHLD,iPid,i, g_stWorkerNode[i].ulPid);

                    if (iPid == g_stWorkerNode[i].ulPid)
                    {
                        g_stWorkerNode[i].ulPid = 0;
                        g_stWorkerNode[i].iSockFd[0] = -1;
                        g_stWorkerNode[i].iSockFd[1] = -1;
                        LOG_Error(0, g_stDameonInfo.stLogFd.iErrFd, "signal pid=%lu, i=%d\n", (unsigned long)iPid, i);

                        DAEMON_Worker_Create(i);
                        //break;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    return;
}

static void daemon_sig_Masterhandler(int iSigno)
{
    int i = 0;
    int iErrCode = ERROR_SUCCESS;

    LOG_Debug(0, g_stDameonInfo.stLogFd.iDbgFd, "master is catch signal\n");

    for (i = 0; i < PID_MAX_NUM; i++)
    {
        if (-1 != g_stWorkerNode[i].iSockFd[0])
        {
            /* sigaction 当前正在处理信号函数中，产生新的信号将阻塞新的信号，等当前的处理完成后在处理等待
               的信号。此处会产生多个 SIGKILL，发送给每个子进程，同时会产生多个 SIGCHLD 信号，但是会合并成一个 SIGCHLD 信号发送到 master 进程，
               但是在 daemon_sig_ChildHandler 中 waitpid 函数会循环等待每一个死掉的进程，故只发送一次 SIGCHLD，也会知道杀死了所有的进程，全部重新启动
               同时后续无论来多少个SIGINT，仅处理一个SIGINT，sigaction会对后续SIGINT进行排队合并处理*/
            iErrCode = kill(g_stWorkerNode[i].ulPid, SIGKILL);
            if (ERROR_SUCCESS != iErrCode)
            {
                LOG_Error(0, g_stDameonInfo.stLogFd.iErrFd, "error, kill wroker process is failed, pid=%lu\n", (unsigned long)g_stWorkerNode[i].ulPid);
            }
            LOG_Debug(0, g_stDameonInfo.stLogFd.iDbgFd, "g_stWorkerNode[%d].ulPid=%lu\n", i, (unsigned long) g_stWorkerNode[i].ulPid);
        }
        else
        {
            continue;
        }
    }

    remove(PROCESS_PID);

    exit(0);
}

static void daemon_Dsignal_Handler(int iSigno)
{
    switch(iSigno)
    {
        case SIGCHLD:
            daemon_sig_ChildHandler(iSigno);
            break;

        case SIGINT:
        case SIGTERM:
            daemon_sig_Masterhandler(iSigno);
            break;

        default:
            break;
    }

    return;
}

static int daemon_Signal_SigChild()
{
	struct sigaction stAct;
    int iErrCode = ERROR_SUCCESS;

	stAct.sa_handler = daemon_Dsignal_Handler;
	stAct.sa_flags = 0;
	sigemptyset(&stAct.sa_mask);

	if (sigaction(SIGCHLD, &stAct, NULL) != 0)
	{
        LOG_Error(0, g_stDameonInfo.stLogFd.iErrFd, "error, sigaction is failed.\n");
		return ERROR_FAILED;
	}

    return iErrCode;
}

static int daemon_Signal_MasterStop()
{
	struct sigaction stAct;
    int iErrCode = ERROR_SUCCESS;

	stAct.sa_handler = daemon_Dsignal_Handler;
	stAct.sa_flags = 0;
	sigemptyset(&stAct.sa_mask);

    /* ctrl + c，该信号会对所有的进程进行杀死，包括 master 和 worker，会先杀死 worker 进程，
       这时 master 会捕捉到 worker 挂掉的信号，进程会重新创建一组新的 worker 进程，然后
       master 进程才会捕捉到该信号，然后在一个一个的杀死，但是下面的 kill 杀死进程方法不会产生该问题 */
	if (sigaction(SIGINT, &stAct, NULL) != 0)
	{
        LOG_Error(0, g_stDameonInfo.stLogFd.iErrFd, "error, master SIGINT sigaction is failed.\n");
		return ERROR_FAILED;
	}

    /* kill master pid */
	if (sigaction(SIGTERM, &stAct, NULL) != 0)
	{
        LOG_Error(0, g_stDameonInfo.stLogFd.iErrFd, "error, master SIGTERM kill sigaction is failed.\n");
		return ERROR_FAILED;
	}

    return iErrCode;
}

static int daemon_Signal_SigPipe()
{
	struct sigaction stAct;
    int iErrCode = ERROR_SUCCESS;

	stAct.sa_handler = SIG_IGN;
	stAct.sa_flags = 0;
	sigemptyset(&stAct.sa_mask);

    /* 如果用 socketpair 进行父子进程通信，如果子进程退出，会产生SIGPIPE信号，
       父进程如果不忽略该信号，则父进程会退出 */
	if (sigaction(SIGPIPE, &stAct, NULL) != 0)
	{
        LOG_Error(0, g_stDameonInfo.stLogFd.iErrFd, "error, sigaction is failed.\n");
		return ERROR_FAILED;
	}

    return iErrCode;
}

int DAEMON_InitSignal()
{
    int iErrCode = ERROR_SUCCESS;

    iErrCode = daemon_Signal_SigChild();
    if (ERROR_SUCCESS == iErrCode)
    {
        iErrCode = daemon_Signal_MasterStop();
    }

    if (ERROR_SUCCESS == iErrCode)
    {
        /* 子进程正常退出，如果父子进程用 socketpair 进行通信，则会产生
           SIGPIPE 信号发送给父进程，父进程如果不忽略该信号，则父进程也会退出 */
        iErrCode = daemon_Signal_SigPipe();
    }

    return iErrCode;
}

