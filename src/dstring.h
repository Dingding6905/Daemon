
#ifndef __DSTRING_H__
#define __DSTRING_H__

/* �����ַ���дתСд */
#define CHAR_TOLOWER(c)      (unsigned char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define CHAR_TOUPPER(c)      (unsigned char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

/* �ַ�����дתСд */
void STRING_Upper2Lowercase(unsigned char *pucDst, unsigned char *pucSrc, size_t uiSrcLen);
void STRING_Lower2Uppercase(unsigned char *pucDst, unsigned char *pucSrc, size_t uiSrcLen);

char *STRING_Strnchr(char *pcSrc, char c, size_t n);
char *STRING_Strnstr(char *pcHaystack, char *pcNeedle, size_t n);

#if 0
���ַ��ȽϺ���   mΪ�ַ�����ʼ��ַ��c0,c1,c2,c3 �ֱ�Ϊ�ַ����ĵ�һ�������ַ�
------------------------------------------------------
��4��charת��Ϊһ���������бȽ��Ƿ����
#define ngx_strGet_cmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)
#define ngx_strPostcmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

//���Ϊ�����ʾ��GET
if (ngx_strGet_cmp(r->header_in->pos, 'G', 'E', 'T', ' '))
#endif

/* m Ϊ�ַ������ַ�������׵�ַ(������)��c0,c1,c2,c3 Ϊ��Ҫ�Ƚϵ������ַ� ngx_str3_cmp(cBuf, 'G', 'E', 'T', ' ')*/
#define STR_3_CMP(m, c0, c1, c2, c3)                                       \
        *(unsigned int *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define STR_4_CMP(m, c0, c1, c2, c3)                                        \
        *(unsigned int *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define STR_5_CMP(m, c0, c1, c2, c3, c4)                                    \
        *(unsigned int *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && m[4] == c4

#define STR_6_CMP(m, c0, c1, c2, c3, c4, c5)                                \
        *(unsigned int *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && (((unsigned int *) m)[1] & 0xffff) == ((c5 << 8) | c4)

#define STR_9_CMP(m, c0, c1, c2, c3, c4, c5, c6, c7, c8)                    \
        *(unsigned int *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && ((unsigned int *) m)[1] == ((c7 << 24) | (c6 << 16) | (c5 << 8) | c4)  \
        && m[8] == c8



#endif /* __DSTRING_H__ */

