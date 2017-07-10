
#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "./log.h"

#define PID_MAX_LEN 8

/* worker ���̸��� */
#define PID_MAX_NUM 5
#define CFG_MAX_LEN 1024

/* �����ļ������� */
#define CFG_MAX_NUM 5

#define PROCESS_PID "../run/mydaemond.pid"

typedef struct tagDaemonInfo
{
    pid_t ulPid;
    LOG_FD_NAME_S stLogFd;
}DAEMON_INFO_S;

typedef struct tagWorkerInfo
{
    int iSockFd[2];
    pid_t ulPid;
    int iLogFd;
}WORKER_INFO_S;

/* �Զ����ַ��� */
typedef struct tagString
{
    unsigned int  uiLen;
    unsigned char ucData[1024];
}STRING_S;

/* �����ļ���Ϣ�ṹ�� */
typedef struct tagConfInfo
{
    STRING_S stKey;
    STRING_S stValue;
}CONF_INFO_S;

/* worker ���̽ṹ������ */
WORKER_INFO_S g_stWorkerNode[PID_MAX_NUM];
/* ���������ļ��ṹ������ */
CONF_INFO_S g_stConfInfo[CFG_MAX_NUM];

DAEMON_INFO_S g_stDameonInfo;

#endif /* __DAEMON_H__ */
