#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "./dstring.h"

/*****************************************************************
** FUNCTION: STRING_Upper2Lowercase
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: up to low
*****************************************************************/
void STRING_Upper2Lowercase(unsigned char *pucDst, unsigned char *pucSrc, size_t uiSrcLen)
{
    while (uiSrcLen) {
        *pucDst = CHAR_TOLOWER(*pucSrc);
        pucDst++;
        pucSrc++;
        uiSrcLen--;
    }
}

/*****************************************************************
** FUNCTION: STRING_Lower2Uppercase
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: low to up
*****************************************************************/
void STRING_Lower2Uppercase(unsigned char *pucDst, unsigned char *pucSrc, size_t uiSrcLen)
{
    while (uiSrcLen) {
        *pucDst = CHAR_TOUPPER(*pucSrc);
        pucDst++;
        pucSrc++;
        uiSrcLen--;
    }
}

/*****************************************************************
** FUNCTION: STRING_Strnchr
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: in the first n bytes of pcSrc to find c
*****************************************************************/
char *STRING_Strnchr(char *pcSrc, char c, size_t n)
{
    size_t i = 0;
    char *p = NULL;

    if (NULL == pcSrc)
    {
        return NULL;
    }

    p = pcSrc;
    for (i = 0; i < n; i++)
    {
        if (c == *p)
        {
            return p;
        }

        p++;
    }

    return NULL;
}

/*****************************************************************
** FUNCTION: STRING_Strnstr
** DATE    :
** INPUT   :
** OUTPUT  :
** RETURN  :void
** FUNCTION DESCRIPTION: in the first n bytes of pcHaystack to find pcNeedle
*****************************************************************/
char *STRING_Strnstr(char *pcHaystack, char *pcNeedle, size_t n)
{
    size_t i = 0;
    int iRet = 0;
    char *p = NULL;
    size_t iNeedleLen = 0;

    iNeedleLen = strlen(pcNeedle);
    if ((n < iNeedleLen) || (NULL == pcHaystack) || (NULL == pcNeedle))
    {
        return NULL;
    }

    p = pcHaystack;
    for (i = 0; i < (n - iNeedleLen + 1); i++)
    {
        iRet = memcmp(p, pcNeedle, iNeedleLen);
        if (0 == iRet)
        {
            return p;
        }

        p++;
    }

    return NULL;
}

#if 0
int main(int argc, char *argv[])
{
    unsigned char cDest[128];
    unsigned char cSrc[128];

    memset(cDest, 0, sizeof(cDest));
    memset(cSrc, 0, sizeof(cSrc));

    memcpy(cSrc, "HellO wORLd", 11);
    Str_Lowercase(cDest, cSrc, strlen(cSrc));

    printf("cSrc=%s\ncDest=%s\n", cSrc, cDest);
    Str_Uppercase(cDest, cSrc, strlen(cSrc));
    printf("cSrc=%s\ncDest=%s\n", cSrc, cDest);

    return 0;
}
#endif


#if 0
int main(int argc, char *argv[])
{
    char *p = NULL;
    p = Str_Strnchr(argv[1], 'c', strlen(argv[1]));
    printf("p=%s\n", p);

    p = Str_Strnstr(argv[2], "daemon11", strlen(argv[2]));
    printf("p2=%s\n", p);

    return 0;
}
#endif








#if 0
int main(int argc, char *argv[])
{
    char cBuf[128];
    memset(cBuf, 0, sizeof(cBuf));

    memcpy(cBuf, argv[1], strlen(argv[1]));
    //memcpy(cBuf, "GET 111", sizeof("GET 111"));
    //如果为真则表示是GET
    //测试时需要./test2 "GET " 或 ./a.out "POST"
    if (STR_3_CMP(cBuf, 'G', 'E', 'T', ' '))
    {
        printf("is get get\n");
    }
    else if (STR_4_CMP(cBuf, 'P', 'O', 'S', 'T')) // ./test2 POST
    {
        printf("is post post\n");
    }
    else
    {
        printf("others\n");
    }

    return 0;
}
#endif




