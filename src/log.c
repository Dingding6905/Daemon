#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/time.h>
#include <fcntl.h>

#include "./log.h"
#include "./dlib.h"

#if 0
#include "lig_log.h"

int iDbgDay = 0;
int iTrcDay = 0;
int iErrDay = 0;

FILE *pDbgFile = NULL;
FILE *pTrcFile = NULL;
FILE *pErrFile = NULL;

char cCfgBuf[MAXFILENAME];
char c_TimedayCheck[MAXFILENAME];
char cTrcTimeBufCheck[MAXFILENAME];
char cErrTimeBufCheck[MAXFILENAME];

pthread_mutex_t pFileMutex = PTHREAD_MUTEX_INITIALIZER;

void Dui_MutexLock()
{
	pthread_mutex_lock(&pFileMutex);

	return;
}

void Dui_MutexUnlock()
{
	pthread_mutex_unlock(&pFileMutex);

	return;
}

FILE *Dui_GetDbgFile()
{
	return pDbgFile;
}

FILE *Dui_GetTrcFile()
{
	return pTrcFile;
}

FILE *Dui_GetErrFile()
{
	return pErrFile;
}

void Dui_InitDbg()
{
	time_t t;
	struct tm *tp = NULL;

	char  c_timeday[MAXFILENAME];
	char cDbgLogName[MAXFILENAME];
	char cErrLogName[MAXFILENAME];
	char cTrcLogName[MAXFILENAME];

	memset(c_timeday, 0, sizeof(c_timeday));
	memset(cDbgLogName, 0, sizeof(cDbgLogName));
	memset(cErrLogName, 0, sizeof(cErrLogName));
	memset(cTrcLogName, 0, sizeof(cTrcLogName));

	time(&t);
	tp = (struct tm *) localtime(&t);

	memcpy(cDbgLogName, cCfgBuf, sizeof(cDbgLogName));
	memcpy(cErrLogName, cCfgBuf, sizeof(cErrLogName));
	memcpy(cTrcLogName, cCfgBuf, sizeof(cTrcLogName));

	//sprintf(c_timeday, "DUI_DEBUG_%4d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday);
	/*delete 3*/
	sprintf(c_timeday, "LIG_DEBUG_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
	strcat(cDbgLogName, c_timeday);

	//delete hour
	memset(c_timeday, 0, sizeof(c_timeday));
	sprintf(c_timeday, "LIG_TRACE_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
	strcat(cTrcLogName, c_timeday);

//delete hour
	memset(c_timeday, 0, sizeof(c_timeday));
	sprintf(c_timeday, "LIG_ERROR_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
	strcat(cErrLogName, c_timeday);

	if (NULL == pDbgFile)
	{
		pDbgFile = fopen(cDbgLogName, "a");
	}

	if (NULL == pErrFile)
	{
		pErrFile = fopen(cErrLogName, "a");
	}

	if (NULL == pTrcFile)
	{
		pTrcFile = fopen(cTrcLogName, "a");
	}

	memset(c_TimedayCheck, 0, sizeof(c_TimedayCheck));
	memcpy(c_TimedayCheck, cDbgLogName, sizeof(c_TimedayCheck));

	memset(cTrcTimeBufCheck, 0, sizeof(cTrcTimeBufCheck));
	memcpy(cTrcTimeBufCheck, cTrcLogName, sizeof(cTrcTimeBufCheck));

	memset(cErrTimeBufCheck, 0, sizeof(cErrTimeBufCheck));
	memcpy(cErrTimeBufCheck, cErrLogName, sizeof(cErrTimeBufCheck));

	/*delete 1*********************/
	iDbgDay = tp->tm_mday;
	iTrcDay = tp->tm_mday;

	return;
}

/* 去掉字符串末尾的空格 */
void Dui_trim(char *s)
{
    char *c = s + strlen(s) - 1;
    while (isspace(*c) && c > s) {
        *c = '\0';
        --c;
    }

    if(c == s)
    {
        *c = '\0';
    }

	return;
}

/* 读取配置文件 */
void Dui_GetCfgFromFile(FILE *fp)
{
    int iLine = 0;
	char cConfigBuf[MAXFILENAME];

	memset(cConfigBuf, 0, sizeof(cConfigBuf));

    while (fgets(cConfigBuf, MAXFILENAME, fp) != NULL)
    {
        ++iLine;

        if (cConfigBuf[0] == '\0' || cConfigBuf[0] == '#' || cConfigBuf[0] == '\n')
        {
            memset(cConfigBuf, 0, MAXFILENAME);
            continue;
        }

        Dui_trim(cConfigBuf);
        if (cConfigBuf[0] == '\0')
        {
            continue;
		}

		/* 处理配置文件中的第一行 */
		if (1 == iLine)
		{
			memcpy(cCfgBuf, cConfigBuf, sizeof(cConfigBuf));
		}
		else
		{
			/* 读取第二行操作 */
		}

		memset(cConfigBuf, 0, sizeof(cConfigBuf));

    }
}


void Dui_InitConf()
{
    FILE *fp = NULL;
    if ((fp = fopen("../cfg/lig.conf", "r")) == NULL)
    {
        printf( "Can't open conf file %s .\n", "lig.conf");
        exit(1);
    }

	/* 读取配置文件 */
    Dui_GetCfgFromFile(fp);

	fclose(fp);

	return;
}


/* 根据文件大小判断是否新建文件 */
void DUI_CheckFileSize(DUI_SYSLOG_TYPE_E enType, int *piDay, FILE **ppFile, char *cTimedayCheck)
{
	time_t t;
	struct tm *tp = NULL;
	long lFileSize = 0;
	struct stat f_stat;

	char  c_timeday[MAXFILENAME];
	char cConfigBuf[MAXFILENAME];

	if (0 == stat(cTimedayCheck, &f_stat))
	{
//		printf("get stat file success cTimedayCheck = %s\n", cTimedayCheck);

		lFileSize = (long)f_stat.st_size;
//文件大小待设定

		if (MAXLOGSIZE < lFileSize)
		{
	//		printf("stat come in\n");
			fclose(*ppFile);
			*ppFile = NULL;

			time(&t);
			tp = (struct tm *) localtime(&t);

			memset(cConfigBuf, 0, sizeof(cConfigBuf));
			memset(c_timeday, 0, sizeof(c_timeday));

			memcpy(cConfigBuf, cCfgBuf, sizeof(cConfigBuf));

			if (DUI_LOG_DEBUG == enType)
			{
				//sprintf(c_timeday, "DUI_DEBUG_%4d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday);
				/*delete 4*/
				sprintf(c_timeday, "LIG_DEBUG_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
				strcat(cConfigBuf, c_timeday);
			}
			else if (DUI_LOG_TRACE == enType)
			{
				sprintf(c_timeday, "LIG_TRACE_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
				strcat(cConfigBuf, c_timeday);
			}
			else
			{
				sprintf(c_timeday, "LIG_ERROR_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
				strcat(cConfigBuf, c_timeday);
			}

			*ppFile = fopen(cConfigBuf, "a");
			if (NULL != *ppFile)
			{
				memset(cTimedayCheck, 0, MAXFILENAME);
				memcpy(cTimedayCheck, cConfigBuf, MAXFILENAME);

				//iDay = tp->tm_mday;
				/*delete 5*/
				*piDay = tp->tm_mday;
			}
		}
	}

	return;
}


/* 根据时间判断是否新建文件 */
void DUI_CheckFileDate(DUI_SYSLOG_TYPE_E enType, int *piDay, FILE **ppFile, char *cTimedayCheck)
{
	time_t t;
	struct tm *tp = NULL;
	struct tm *tpCur = NULL;

	char  c_timeday[MAXFILENAME];
	char cConfigBuf[MAXFILENAME];

	time(&t);
	tpCur = (struct tm *) localtime(&t);

	//if (iDay != tpCur->tm_mday)
	/*delete 2*/
	if (*piDay != tpCur->tm_mday)
	{
	//	printf("day is come in\n");
		fclose(*ppFile);
		*ppFile = NULL;

		time(&t);
		tp = (struct tm *) localtime(&t);

		memset(cConfigBuf, 0, sizeof(cConfigBuf));
		memset(c_timeday, 0, sizeof(c_timeday));

		memcpy(cConfigBuf, cCfgBuf, sizeof(cConfigBuf));

		if (DUI_LOG_DEBUG == enType)
		{
			//sprintf(c_timeday, "DUI_DEBUG_%4d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday);
			/*delete 4*/
			sprintf(c_timeday, "LIG_DEBUG_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
			strcat(cConfigBuf, c_timeday);
		}
		else if(DUI_LOG_TRACE == enType)
		{
			//delete 9 去掉hour
			sprintf(c_timeday, "LIG_TRACE_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
			strcat(cConfigBuf, c_timeday);
		}

		*ppFile = fopen(cConfigBuf, "a");
		if (NULL != *ppFile)
		{
			memset(cTimedayCheck, 0, MAXFILENAME);
			memcpy(cTimedayCheck, cConfigBuf, MAXFILENAME);

			//iDay = tp->tm_mday;
			/*delete 5*/
			*piDay = tp->tm_mday;
		}
	}

	return;
}


void *DUI_CheckLogFile()
{
	while(1)
	{
		pthread_mutex_lock(&pFileMutex);
		if ((NULL == pDbgFile) || (NULL == pErrFile) || (NULL == pTrcFile))
		{
			Dui_InitDbg();
		}
		else
		{
			/* 检查 DEBUG 文件是否需要创建新文件 */
			DUI_CheckFileDate(DUI_LOG_DEBUG, &iDbgDay, &pDbgFile, c_TimedayCheck);
			DUI_CheckFileSize(DUI_LOG_DEBUG, &iDbgDay, &pDbgFile, c_TimedayCheck);

			/* 检查 TRACE 文件是否需要创建新文件 */
			DUI_CheckFileDate(DUI_LOG_TRACE, &iTrcDay, &pTrcFile, cTrcTimeBufCheck);
			DUI_CheckFileSize(DUI_LOG_TRACE, &iTrcDay, &pTrcFile, cTrcTimeBufCheck);

			/* 检查 ERROR 文件是否需要创建新文件 */
			DUI_CheckFileSize(DUI_LOG_ERROR, &iErrDay, &pErrFile, cErrTimeBufCheck);
		}
		pthread_mutex_unlock(&pFileMutex);

		sleep(300);
	}
}

int LIG_MainLog()
{
	pthread_t thPrint;

	Dui_InitConf();

//	printf("main cCfgBuf = %s\n",cCfgBuf);
	Dui_InitDbg();

	if (0 != pthread_create(&thPrint, NULL, DUI_CheckLogFile, NULL))
	{
//		printf("create checklogfile thread failed\n");
		return -1;
	}

	pthread_detach(thPrint);

	return 0;
}
#endif


static void log_GetCurTime(char *pszCurTime)
{
    struct tm sttp;
    struct timeval tv;
    struct tm *tp = NULL;


    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &sttp);
    tp = &sttp;

    sprintf(pszCurTime, "%4d/%02d/%02d %02d:%02d:%02d.%06d",1900 + (int)tp->tm_year, 1 + (int)tp->tm_mon,
            (int)tp->tm_mday, (int)tp->tm_hour, (int)tp->tm_min, (int)tp->tm_sec, (int)tv.tv_usec);

    return;
}

int log_CreateTimeLogFileName(LOG_TYPE_E iType, char *pcFileName)
{
	time_t t;
	struct tm *tp = NULL;
    int iErrCode = ERROR_SUCCESS;

	time(&t);
	tp = (struct tm *) localtime(&t);

    switch (iType)
    {
        case LOG_DEBUG:
        	sprintf(pcFileName, "LOG_DEBUG_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon,
                    tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
            break;

        case LOG_TRACE:
        	sprintf(pcFileName, "LOG_TRACE_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon,
                    tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
            break;

        case LOG_ERROR:
        	sprintf(pcFileName, "LOG_ERROR_%4d%02d%02d%02d%02d%02d.log",1900+tp->tm_year, 1+tp->tm_mon,
                    tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
            break;

        default:
            iErrCode = ERROR_FAILED;
            break;
    }

    return iErrCode;
}

void LOG_Debug(unsigned int uiLevel, int iFd, char *fmt, ...)
{
    va_list ap;
    char szBuf[LOG_MSG_MAX_LEN];
    char szCurTime[LOG_TIME_MAX_LEN];
    char szLogMsg[LOG_MSG_MAX_LEN];

    memset(szBuf, 0, sizeof(szBuf));
    memset(szLogMsg, 0, sizeof(szLogMsg));
    memset(szCurTime, 0, sizeof(szCurTime));

    va_start(ap, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, ap);

    log_GetCurTime(szCurTime);
    snprintf(szLogMsg, sizeof(szLogMsg), "%s [debug] %lu#%lu: *%d %s", szCurTime,
            (unsigned long)getpid(), (unsigned long)pthread_self(), uiLevel, szBuf);

    write(iFd, szLogMsg, strlen(szLogMsg));

    va_end(ap);

    return;
}

void LOG_Error(unsigned int uiLevel, int iFd, char *fmt, ...)
{
    va_list ap;
    char szBuf[LOG_MSG_MAX_LEN];
    char szCurTime[LOG_TIME_MAX_LEN];
    char szLogMsg[LOG_MSG_MAX_LEN];

    memset(szBuf, 0, sizeof(szBuf));
    memset(szLogMsg, 0, sizeof(szLogMsg));
    memset(szCurTime, 0, sizeof(szCurTime));

    va_start(ap, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, ap);

    log_GetCurTime(szCurTime);
    snprintf(szLogMsg, sizeof(szLogMsg), "%s [error] %lu#%lu: *%d %s", szCurTime,
            (unsigned long)getpid(), (unsigned long)pthread_self(), uiLevel, szBuf);

    write(iFd, szLogMsg, strlen(szLogMsg));

    va_end(ap);

    return;
}

void LOG_Debug_Print(unsigned int uiLevel, char *fmt, ...)
{
    va_list ap;
    char szBuf[LOG_MSG_MAX_LEN];
    char szCurTime[LOG_TIME_MAX_LEN];

    memset(szBuf, 0, sizeof(szBuf));
    memset(szCurTime, 0, sizeof(szCurTime));

    va_start(ap, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, ap);

    log_GetCurTime(szCurTime);

    printf("%s [debug] %lu#%lu: *%d %s", szCurTime, (unsigned long)getpid(), (unsigned long)pthread_self(), uiLevel, szBuf);
    //DEBUG(LOG_DEBUG, "%s,%s", szCurTime, szBuf)
    fflush(stdout);

    va_end(ap);

    return;
}

void LOG_Error_Print(unsigned int uiLevel, char *fmt, ...)
{
    va_list ap;
    char szBuf[LOG_MSG_MAX_LEN];
    char szCurTime[LOG_TIME_MAX_LEN];

    memset(szBuf, 0, sizeof(szBuf));
    memset(szCurTime, 0, sizeof(szCurTime));

    va_start(ap, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, ap);

    log_GetCurTime(szCurTime);

    printf("%s [error] %lu#%lu: *%d %s", szCurTime, (unsigned long)getpid(), (unsigned long)pthread_self(), uiLevel, szBuf);
    //DEBUG(LOG_DEBUG, "%s,%s", szCurTime, szBuf)
    fflush(stdout);

    va_end(ap);

    return;
}

int LOG_InitOpen(int *piFd, LOG_TYPE_E iType)
{
    int iErrCode = ERROR_SUCCESS;
    char cFileName[LOG_FILE_NAME_LEN];

    memset(cFileName, 0, sizeof(cFileName));

    switch (iType)
    {
        case LOG_DEBUG:

            *piFd = open(LOG_DEBUG_NAME, O_RDWR|O_CREAT|O_APPEND, FILE_MODE);
            if (ERROR_FAILED == *piFd)
            {
                iErrCode = ERROR_FAILED;
            }
            break;

        case LOG_TRACE:

            *piFd = open(LOG_TRACE_NAEM, O_RDWR|O_CREAT|O_APPEND, FILE_MODE);
            if (ERROR_FAILED == *piFd)
            {
                iErrCode = ERROR_FAILED;
            }
            break;

        case LOG_ERROR:

            *piFd = open(LOG_ERROR_NAME, O_RDWR|O_CREAT|O_APPEND, FILE_MODE);
            if (ERROR_FAILED == *piFd)
            {
                iErrCode = ERROR_FAILED;
            }
            break;

        default:
            iErrCode = ERROR_FAILED;
            break;
    }

    return iErrCode;
}

