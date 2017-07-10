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
            /* WNOHANG ���û���κ��Ѿ��������ӽ��������Ϸ��أ������Եȴ� */
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
            /* sigaction ��ǰ���ڴ����źź����У������µ��źŽ������µ��źţ��ȵ�ǰ�Ĵ�����ɺ��ڴ���ȴ�
               ���źš��˴��������� SIGKILL�����͸�ÿ���ӽ��̣�ͬʱ�������� SIGCHLD �źţ����ǻ�ϲ���һ�� SIGCHLD �źŷ��͵� master ���̣�
               ������ daemon_sig_ChildHandler �� waitpid ������ѭ���ȴ�ÿһ�������Ľ��̣���ֻ����һ�� SIGCHLD��Ҳ��֪��ɱ�������еĽ��̣�ȫ����������
               ͬʱ�������������ٸ�SIGINT��������һ��SIGINT��sigaction��Ժ���SIGINT�����ŶӺϲ�����*/
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

    /* ctrl + c�����źŻ�����еĽ��̽���ɱ�������� master �� worker������ɱ�� worker ���̣�
       ��ʱ master �Ჶ׽�� worker �ҵ����źţ����̻����´���һ���µ� worker ���̣�Ȼ��
       master ���̲ŻᲶ׽�����źţ�Ȼ����һ��һ����ɱ������������� kill ɱ�����̷���������������� */
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

    /* ����� socketpair ���и��ӽ���ͨ�ţ�����ӽ����˳��������SIGPIPE�źţ�
       ��������������Ը��źţ��򸸽��̻��˳� */
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
        /* �ӽ��������˳���������ӽ����� socketpair ����ͨ�ţ�������
           SIGPIPE �źŷ��͸������̣���������������Ը��źţ��򸸽���Ҳ���˳� */
        iErrCode = daemon_Signal_SigPipe();
    }

    return iErrCode;
}

