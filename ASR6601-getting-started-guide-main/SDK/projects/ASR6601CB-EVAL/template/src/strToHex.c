#include "strToHex.h"

/** 
 * \brief hex string to hex array
 * \details if the converted str has hex,then fill the array out, the argument outlen is the number of the hex
 * \param str Point to the string will be converted
 * \param out Point to the string will be filled with the converted string
 * \param outlen Point to the variable that stores the length of the converted string
 * \author ZhaoZeqiang
*/
int StringToHex(char *str, uint8_t *out,uint8_t *outlen)
{
    char *p = str;
    char high = 0, low = 0;
    int tmplen = strlen(p), cnt = 0;
    tmplen = strlen(p);
    while(cnt < (tmplen / 2))
    {
        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
        low = (*(++ p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;
        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p ++;
        cnt ++;
    }
    if(tmplen % 2 != 0) out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;

    if(outlen != NULL) 
		*outlen = tmplen / 2 + tmplen % 2;

    return tmplen / 2 + tmplen % 2;
}