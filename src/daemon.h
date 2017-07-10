
#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "./log.h"

#define PID_MAX_LEN 8

/* worker 进程个数 */
#define PID_MAX_NUM 5
#define CFG_MAX_LEN 1024

/* 配置文件的行数 */
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

/* 自定义字符串 */
typedef struct tagString
{
    unsigned int  uiLen;
    unsigned char ucData[1024];
}STRING_S;

/* 配置文件信息结构体 */
typedef struct tagConfInfo
{
    STRING_S stKey;
    STRING_S stValue;
}CONF_INFO_S;

/* worker 进程结构体数组 */
WORKER_INFO_S g_stWorkerNode[PID_MAX_NUM];
/* 解析配置文件结构体数组 */
CONF_INFO_S g_stConfInfo[CFG_MAX_NUM];

DAEMON_INFO_S g_stDameonInfo;

#endif /* __DAEMON_H__ */
