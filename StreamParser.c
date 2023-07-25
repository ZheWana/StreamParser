/**
 * @file StreamParser.h
 * @author ZheWana(zhewana@qq.com)
 *
 * @date 2023-7-24
 */

#include "StreamParser.h"
#include "string.h"

#define IS_NUMBER(ch) (ch >= '0' && ch <= '9')

void SParser_Init(pSParser_t parser, char* headStr, char* tailStr, char* divStr, char* typeList)
{
    parser->headStr = headStr;
    parser->headStrLen = headStr == NULL ? 0 : strlen(headStr);
    parser->tailStr = tailStr;
    parser->TailStrLen = tailStr == NULL ? 0 : strlen(tailStr);
    parser->divStr = divStr;
    parser->divStrLen = divStr == NULL ? 0 : strlen(divStr);
    parser->typeList = typeList;

    parser->chPtr[0] = headStr;
    parser->typePtr = parser->typeList;

    parser->buff[0] = parser->buff[1] = parser->buff[2] = parser->buff[3] = 0;

    parser->mState = FindHead;
}

static metaData_t Parse_Intenger(pSParser_t parser, const char ch)
{
    metaData_t temp = { 0 };

    if (parser->buff[0] == 0 && ch == '-') {
        parser->buff[0] = -1;
    } else if (IS_NUMBER(ch)) {
        parser->buff[2] = parser->buff[2] * 10 + ch - '0';
    } else if (ch == *parser->tailStr || ch == *parser->divStr || ch == '.') {
        temp.intenger = parser->buff[0] == 0 ? parser->buff[2] : parser->buff[2] * -1;
        parser->buff[0] = parser->buff[2] = 0;
        return temp;
    }

    return temp;
}

static metaData_t Parse_Float(pSParser_t parser, const char ch)
{
    metaData_t temp = { 0 };

    if ((temp = Parse_Intenger(parser, ch)).intenger) {
        parser->buff[1 + parser->buff[3]] = temp.intenger;
    }
    if (ch == '.') {
        parser->buff[3] = 1;
    }

    if (ch == *parser->tailStr || ch == *parser->divStr) {
        temp.flt = parser->buff[2];

        while (temp.flt > 1) {
            temp.flt /= 10;
        }
        temp.flt = (floatTypdef)parser->buff[1] + (parser->buff[1] >= 0 ? temp.flt : -temp.flt);
        memset(parser->buff, 0, 4 * sizeof(int));

        return temp;
    }

    return (metaData_t)0;
}

/**
 * @return return value as follow:
 * 1: compared equal
 * 0: compared not equal
 */
static int CompareString(pSParser_t parser, const char* str, const int strLen, const char ch, const int pSelect)
{
    if (parser->chPtr[pSelect] == NULL || *parser->chPtr[pSelect] == '\0') {
        return 1;
    } else if (ch == *parser->chPtr[pSelect]) {
    EqualHook:
        if (strLen == 1) {
            if (pSelect)
                return 1;
            parser->chPtr[pSelect] = NULL;
        } else {
            parser->chPtr[pSelect] = parser->chPtr[pSelect] + 1;
        }
    } else {
        parser->chPtr[pSelect] = (char*)str;
        if (ch == *parser->chPtr[pSelect]) {
            goto EqualHook;
        }
    }
    return 0;
}

int SParser_Parse(pSParser_t parser, pMetaData_t dataArray, const char ch)
{
    switch (parser->mState) {
    case FindHead:
        if (CompareString(parser, parser->headStr, parser->headStrLen, ch, 0)) {
            parser->chPtr[0] = parser->divStr;
            parser->chPtr[1] = parser->tailStr;
            parser->mState = Parse;
        } else {
            break;
        }
    case Parse:
        if (CompareString(parser, parser->divStr, parser->divStrLen, ch, 0)) {
            parser->chPtr[0] = parser->divStr;

            if (*parser->typePtr != ' ' && *parser->typePtr != '\0') {
                dataArray[parser->typePtr - parser->typeList] = parser->temp;
            }

            if (*(parser->typePtr) == '\0') {
                goto EndOfParse;
            } else {
                parser->typePtr++;
            }
        }

        switch (*parser->typePtr) {
            metaData_t temp;
        case ' ':
            break;
        case 'd':
            if ((temp = Parse_Intenger(parser, ch)).intenger)
                parser->temp = temp;
            break;
        case 'f':
            if ((temp = Parse_Float(parser, ch)).intenger)
                parser->temp = temp;
            break;
        case 's':
            break;

        default:
            break;
        }

        if (CompareString(parser, parser->tailStr, parser->TailStrLen, ch, 1)) {
            if (*parser->typePtr != ' ')
                dataArray[parser->typePtr - parser->typeList] = parser->temp;

        EndOfParse:
            parser->chPtr[0] = parser->headStr;
            parser->typePtr = parser->typeList;
            parser->mState = FindHead;
            return 1;
        }
    default:
        break;
    }

    return 0;
}
