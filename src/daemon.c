/*************************************************
  Copyright (C), 1988-1999, Huawei Tech. Co.,Ltd.
  File name:      // �ļ���
  Author:      Version:        Date: // ���ߡ��汾���������
  Description:    // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
                  // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
                  // �ơ�˳�򡢶����������ȹ�ϵ
  Others:         // �������ݵ�˵��
  Function List:  // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
            1. ....
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���
            1. Date:
               Author:
               Modification:
            2. ...
*************************************************/

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
#include "./dsystem.h"

/*****************************************************************
** FUNCTION: os_WriteMasterPid
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: write daemon pid to file
*****************************************************************/
int daemon_WriteMasterPid()
{
    int iFd = 0;
    char cPid[PID_MAX_LEN];
    int iErrCode = ERROR_SUCCESS;

    iFd = open(PROCESS_PID, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (0 > iFd)
    {
        iErrCode = ERROR_FAILED;
    }

    memset(cPid, 0, sizeof(cPid));
    snprintf(cPid, sizeof(cPid), "%d", getpid());
    write(iFd, cPid, strlen(cPid));

    close(iFd);
    //sleep(100);

    return iErrCode;
}

/*****************************************************************
** FUNCTION: os_CreateMaster
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: create master process
*****************************************************************/
int daemon_CreateMaster()
{
    int i = 0;
    int iFd = 0;
    pid_t iPid;
    int iErrCode = ERROR_FAILED;

    iPid = fork();
    switch (iPid)
    {
        case -1:
            return iErrCode;

        case 0:
            break;

        default:
            /* _exit �����������ǣ�ֱ��ʹ����ֹͣ���У������ʹ�õ��ڴ�ռ䣬����������ں˵ĸ������ݽṹ
                exit()�����ڵ���exit  ϵͳ����ǰҪ����ļ��Ĵ���������ļ��������е�����д���ļ� */
            _exit(EXIT_SUCCESS);
    }

    /* ʹ�ӽ��̳�Ϊ�鳤 */
    if (-1 == setsid())
    {
        return iErrCode;
    }

    iPid = fork();
    switch (iPid)
    {
        case -1:
            return iErrCode;

        case 0:
            break;

        default:
            _exit(EXIT_SUCCESS);
    }

    /* �رս��̴򿪵������ļ��������������Ҫ�õ��Ļ������� open �� */
    for (i = 0; i < NOFILE; i++)
    {
        close(i);
    }

    //chdir("/");
    umask(0);

    iFd = open("/dev/null", O_RDWR);
    if (STDIN_FILENO != iFd)
    {
        return iErrCode;
    }
    if (STDOUT_FILENO != dup2(STDIN_FILENO, STDOUT_FILENO))
    {
        return iErrCode;
    }
    if (STDERR_FILENO != dup2(STDIN_FILENO, STDERR_FILENO))
    {
        return iErrCode;
    }

    iErrCode = daemon_WriteMasterPid();

    return iErrCode;
}

/*****************************************************************
** FUNCTION: infosec_webenc_cfg_InitCfg
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: �Դ洢�����ļ������õ�������г�ʼ��
*****************************************************************/
static void daemon_cfg_Parse(unsigned char *pucCfgName, unsigned char *pucCfgValue, int iIndex)
{
    int iNameLen = 0;
    int iValueLen = 0;

    iNameLen = strlen((char *) pucCfgName);
    g_stConfInfo[iIndex].stKey.uiLen = iNameLen;
    memcpy(g_stConfInfo[iIndex].stKey.ucData, pucCfgName, iNameLen);

    iValueLen = strlen((char *) pucCfgValue);
    g_stConfInfo[iIndex].stValue.uiLen = iValueLen;
    memcpy(g_stConfInfo[iIndex].stValue.ucData, pucCfgValue, iValueLen);

    if (0 == strncmp("worker_processes", (char *)pucCfgName, sizeof("worker_processes") - 1))
    {

    }

    return;
}

/*****************************************************************
** FUNCTION: INFOSEC_webenc_ReadCfg
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: �������ļ��ж�ȡ����
*****************************************************************/
int DAEMON_ReadCfg()
{
    int iIndex = 0;
    FILE *pstFp = NULL;
    int iErrCode = ERROR_SUCCESS;
    unsigned char ucCfgBuf[CFG_MAX_LEN];
    unsigned char ucCfgName[CFG_MAX_LEN];
    unsigned char ucCfgValue[CFG_MAX_LEN];

    memset(ucCfgBuf, 0, sizeof(ucCfgBuf));
    memset(&g_stConfInfo, 0, sizeof(g_stConfInfo));

    pstFp = fopen("../conf/daemon.conf", "r"); //�޸�
    if (NULL == pstFp)
    {
        perror("error, fopen conf is failed.");
    }

    while (NULL != fgets((char *)ucCfgBuf, sizeof(ucCfgBuf), pstFp))
    {
        /* ȥ�� \n */
        ucCfgBuf[strlen((char *)ucCfgBuf) - 1] = 0;

        /* �����ļ��е�ע�� */
        if ('#' == ucCfgBuf[0])
        {
            memset(ucCfgBuf, 0, sizeof(ucCfgBuf));
            continue;
        }

        if (iIndex < CFG_MAX_NUM)
        {
            memset(ucCfgName, 0, sizeof(ucCfgName));
            memset(ucCfgValue, 0, sizeof(ucCfgValue));

            sscanf((char *) ucCfgBuf, "%s %s", (char *) ucCfgName, (char *) ucCfgValue);
            daemon_cfg_Parse(ucCfgName, ucCfgValue, iIndex);
            memset(ucCfgBuf, 0, sizeof(ucCfgBuf));
            iIndex++;
        }
        else
        {
            LOG_Error(0, g_stDameonInfo.stLogFd.iErrFd, "config file args is too more.\n");
            break;
        }
    }

    fclose(pstFp);

    return iErrCode;
}

int daemon_InitStart()
{
    int iRet = 0;
    int iErrCode = ERROR_SUCCESS;

    iRet = access(PROCESS_PID, 0);
    if (0 == iRet)
    {
        LOG_Error_Print(0, "error, mydaemon.pid is exist.\n");
        exit(0);
    }

    return iErrCode;
}

int daemon_dlog_Init()
{
    int iErrCode = ERROR_SUCCESS;

    iErrCode = LOG_InitOpen(&g_stDameonInfo.stLogFd.iDbgFd, LOG_DEBUG);
    iErrCode |= LOG_InitOpen(&g_stDameonInfo.stLogFd.iTrcFd, LOG_TRACE);
    iErrCode |= LOG_InitOpen(&g_stDameonInfo.stLogFd.iErrFd, LOG_ERROR);

    return iErrCode;
}

int daemon_Init()
{
    int iErrCode = ERROR_SUCCESS;

    memset(&g_stDameonInfo, 0, sizeof(g_stDameonInfo));
    memset(g_stWorkerNode, 0, sizeof(g_stWorkerNode));

    (void)Dsysteam_CoreDump();
    daemon_InitStart();
    daemon_CreateMaster();
    daemon_dlog_Init();
    DAEMON_ReadCfg();

    return iErrCode;
}

int daemon_Fini()
{
    int iErrCode = ERROR_SUCCESS;

    remove(PROCESS_PID);

    return iErrCode;
}

int main(int argc, char *argv[])
{
    int i = 0;
    int iErrCode = ERROR_SUCCESS;

    daemon_Init();

    LOG_Debug(0, g_stDameonInfo.stLogFd.iDbgFd, "== daemon is begin ==\n");

    for (i = 0; i < PID_MAX_NUM; i++)
    {
        iErrCode |= DAEMON_Worker_Create(i);
    }

    DAEMON_InitSignal();

#if 0
    for (i = 0; i < 5; i++)
    {
        memset(cBuf, 0, sizeof(cBuf));
        snprintf(cBuf, sizeof(cBuf), "%d ", i);
        write(g_stWorkerNode[i].iSockFd[1], cBuf, strlen(cBuf));
        printf("stWorkerNode[%d].ulPid=%lu\n", i, g_stWorkerNode[i].ulPid);
    }
#endif

    while (1)
    {
        usleep(500);
    }

    daemon_Fini();

    return 0;
}

