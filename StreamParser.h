/**
 * @file StreamParser.h
 * @author ZheWana (zhewana@qq.com)
 * @brief A parser that can help to parse specific information from byte stream.
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __STREAM_HANDLER__
#define __STREAM_HANDLER__

#include "stdint.h"

/**
 * @brief Config whether to use double other than float for "f" type.
*/
#define USE_DOUBLE (1)


#if USE_DOUBLE
typedef double floatTypdef;
#else
typedef float floatTypdef;
#endif

/**
 * @brief Define metadata for return value to use.
 * 
 */
typedef union MetaDataTypedef {
    void* strPtr;
    floatTypdef flt;
    int intenger;
} metaData_t, *pMetaData_t;

typedef struct StreamParser {
    char* headStr;
    char* divStr;
    char* tailStr;

    /**
     * @note "typeList" should be input as follow:
     * " " for ignore
     * "s" for string
     * "d" for intenger
     * "f" for float
     */
    char* typeList;
    char* typePtr;
    
    char* chPtr[2];

    /**
     * @brief parser buffer refer as:
     * | sign | intenger | decimal | flags |
     */
    int buff[4];
    metaData_t temp;

    enum {
        FindHead,
        Parse
    } mState;

    uint8_t headStrLen;
    uint8_t divStrLen;
    uint8_t TailStrLen;
} sParser_t, *pSParser_t;

void SParser_Init(pSParser_t parser, char* headStr, char* tailStr, char* divStr, char* typeList);

int SParser_Parse(pSParser_t parser, pMetaData_t dataArray, const char ch);

#endif // __STREAM_HANDLER__