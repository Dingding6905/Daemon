#ifndef __DUI_LOG_H__
#define __DUI_LOG_H__

#if 0
#define MAXFILENAME   256
#define LOG_DEBUG     0x00000001
#define LOG_TRACE     0x00000002
#define LOG_ERROR     0x00000003

#define MAXLOGSIZE    2147483648

typedef enum tagDUI_SysLogType
{
	DUI_LOG_DEBUG,
	DUI_LOG_TRACE,
	DUI_LOG_ERROR,

	DUI_LOG_MAX
}DUI_SYSLOG_TYPE_E;

void Dui_MutexLock();
void Dui_MutexUnlock();
FILE *Dui_GetDbgFile();
FILE *Dui_GetTrcFile();
FILE *Dui_GetErrFile();

/* 打印日志到文件 */
#define DEBUG(args, format, ...)    			                                                                                                    \
{																																					\
	time_t t;																																		\
	struct tm *tpCur = NULL;                                                                                                                        \
	FILE *pCurFile = NULL;																															\
	time(&t);																																		\
	tpCur = (struct tm *) localtime(&t);																											\
																																					\
	Dui_MutexLock();                      																							                \
	switch(args)																																	\
	{																																				\
		case LOG_DEBUG:																																\
			pCurFile = Dui_GetDbgFile();																											\
			break;																																	\
		case LOG_TRACE:                                                                                                                             \
			pCurFile = Dui_GetTrcFile();                                                                                                            \
			break;																																	\
		case LOG_ERROR:																																\
			pCurFile = Dui_GetErrFile();																											\
			break;																																 	\
		default:																																	\
			break;																																	\
	}																																				\
																																					\
	if (NULL != pCurFile)																															\
	{																																				\
		fprintf(pCurFile, format,##__VA_ARGS__);																										\
		fflush(pCurFile);																																\
	}																																					\
	Dui_MutexUnlock();																													                \
}

int LIG_MainLog();
#endif

typedef enum tagLogType
{
    LOG_DEBUG = 1,
    LOG_TRACE,
    LOG_ERROR,

    LOG_OTHERS
}LOG_TYPE_E;

typedef struct tagLogFdName
{
    int iDbgFd;
    int iTrcFd;
    int iErrFd;
}LOG_FD_NAME_S;

#define LOG_TIME_MAX_LEN    64
#define LOG_MSG_MAX_LEN     8192
#define LOG_FILE_NAME_LEN   256

#define LOG_DEBUG_NAME  "../log/DEBUG.log"
#define LOG_TRACE_NAEM  "../log/TRACE.log"
#define LOG_ERROR_NAME  "../log/ERROR.log"

void LOG_Debug(unsigned int uiLevel, int iFd, char *fmt, ...);
void LOG_Error(unsigned int uiLevel, int iFd, char *fmt, ...);
void LOG_Debug_Print(unsigned int uiLevel, char *fmt, ...);
void LOG_Error_Print(unsigned int uiLevel, char *fmt, ...);

int LOG_InitOpen(int *piFd, LOG_TYPE_E iType);

#endif
