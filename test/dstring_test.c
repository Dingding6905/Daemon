#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../src/dstring.h"

int main_test()
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

//测试时需要./test2 "GET " 或 ./a.out "POST"
int main(int argc, char *argv[])
{
    char cBuf[128];
    memset(cBuf, 0, sizeof(cBuf));

    memcpy(cBuf, argv[1], strlen(argv[1]));
    //memcpy(cBuf, "GET 111", sizeof("GET 111"));
    //如果为真则表示是GET
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

    main_test();

    return 0;
}




