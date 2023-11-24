#ifndef BSP_BOARD_INCLUDE_LOG
#define BSP_BOARD_INCLUDE_LOG

#include "communications.h"

#define Log(X, size) _Generic((X), default: LogD, char *: LogS)(X, size)

void InitLog(CommunicationHandle *);

void LogD(uint8_t[], uint16_t);

void LogS(char[], uint16_t);

#endif /* BSP_BOARD_INCLUDE_LOG */
