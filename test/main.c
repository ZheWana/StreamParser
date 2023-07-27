#include "../StreamParser.h"
#include "stdio.h"
#include "string.h"

char testStr[] = "$GPGGA,092204.999,4250.5589,S,14718.5084,E,1,04,24.4,19.7,M,,,,0000*1F";
char* chptr = testStr;
char buffList[4][STRING_BUFFER_SIZE];
metaData_t databuff[20] = {
    [0] = buffList[0],
    [3] = buffList[1],
    [5] = buffList[2],
    [10] = buffList[3]
};

int main(void)
{
    sParser_t parser;
    char* strList[] = {
        "ssssfsddffs",
        "Parsed intenger [%d]: %d\r\n",
        "Parsed float    [%d]: %f\r\n",
        "Parsed string   [%d]: %s\r\n",
        "Parse ignored   [%d]\r\n",
    };

    SParser_Init(&parser, "$GP", "*1F", ",", strList[0]);
    while (*chptr != '\0') {
        if (SParser_Parse(&parser, databuff, *chptr++)) {
            for (int i = 0; i < strlen(strList[0]); i++) {
                switch (strList[0][i]) {
                case 's':
                    printf(strList[3], i, databuff[i].strPtr == NULL ? "NULL" : databuff[i].strPtr);
                    break;
                case 'f':
                    printf(strList[2], i, databuff[i]);
                    break;
                case 'd':
                    printf(strList[1], i, databuff[i]);
                    break;
                case ' ':
                    printf(strList[4], i);
                    break;
                }
            }
        }
    }
    return 0;
}