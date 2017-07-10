
#include "./dsystem.h"

//打开系统 core dump
int Dsysteam_CoreDump()
{
    int iRet = 0;
    int iErrCode = -1;
    struct rlimit rlmt;

    memset(&rlmt, 0, sizeof(struct rlimit));

    iRet = getrlimit(RLIMIT_CORE, &rlmt);
    if (-1 == iRet)
    {
        printf("error: getrlimit is failed\n");
        return iErrCode;
    }
    //printf("Before set rlimit CORE dump current is:%d, max is:%d\n", (int)rlmt.rlim_cur, (int)rlmt.rlim_max);

    rlmt.rlim_cur = (rlim_t)CORE_SIZE;
    rlmt.rlim_max = (rlim_t)CORE_SIZE;

    iRet = setrlimit(RLIMIT_CORE, &rlmt);
    if (-1 == iRet)
    {
        printf("error: setlimit is failed\n");
        return iErrCode;
    }
#if 0
    iRet = getrlimit(RLIMIT_CORE, &rlmt);
    if (-1 == iRet)
    {
        printf("getlimit is failed\n");
        return iErrCode;
    }
    printf("After set rlimit CORE dump current is:%d, max is:%d\n", (int)rlmt.rlim_cur, (int)rlmt.rlim_max);

    /*测试非法内存，产生core文件*/
    int *ptr = NULL;
    *ptr = 10;
#endif

    iErrCode = 0;
    return iErrCode;
}

