#include "../StreamParser.h"
#include "stdio.h"
#include "string.h"

char testStr[] = "$GPGGA,092204.999,4250.5589,S,14718.5084,E,1,04,24.4,19.7,M,,,,0000*1F";
char* chptr = testStr;

sParser_t parser;
char buffList[4][STRING_BUFFER_SIZE];
metaData_t databuff[20] = {
    [0] = buffList[0],
    [3] = buffList[1],
    [5] = buffList[2],
    [10] = buffList[3]
};
char* fmtStr = "ssssfs,dffs";

void XXX_UART_ReceiveCompltCallBack(const char ch)
{
    static char* strList[] = {
        "Parsed intenger [%d]: %d\r\n",
        "Parsed float    [%d]: %f\r\n",
        "Parsed string   [%d]: %s\r\n",
        "Parse ignored   [%d]\r\n",
    };
    if (SParser_Parse(&parser, databuff, ch)) {
        for (int i = 0; i < strlen(fmtStr); i++) {
            switch (fmtStr[i]) {
            case 'f':
                printf(strList[1], i, databuff[i]);
                break;
            case 'd':
                printf(strList[0], i, databuff[i]);
                break;
            case ' ':
                printf(strList[3], i);
                break;
            case 's':
            default:
                printf(strList[2], i, databuff[i].strPtr == NULL ? "NULL" : databuff[i].strPtr);
                break;
            }
        }
    }
}

int main(void)
{
    SParser_Init(&parser, "$GP", "*1F", ",", fmtStr);
    while (*chptr != '\0') {
        // We just want to make it more like an interrupt in MCU.
        XXX_UART_ReceiveCompltCallBack(*chptr++);
    }
    return 0;
}