#include "communications_ex.h"

#include <string.h>

#include "communications.h"
#include "log.h"
#include "stddef.h"

#define ENDS_COUNT 2

static const uint8_t *ends[] = {"OK\r\n", "ERROR\r\n"};

static const uint16_t endSize[] = {4U, 7U};

bool DetectError(uint8_t symbol) {
    static uint8_t counter = 0;
    if (ends[1][counter] == symbol) {
        if (counter == endSize[1] - 1) {
            counter = 0;
            return true;
        }
        counter++;
    } else {
        counter = 0;
    }
    return false;
}

ResponseStatus Execute(const CommunicationHandle *hComm, const uint8_t *cmd,
                       uint16_t cmdSize, uint8_t *out, uint16_t outSize,
                       uint16_t *read) {
    uint8_t lastRead;
    ResponseStatus readStatus = RESPONSE_ERROR;
    uint16_t pos = 0;
    uint16_t endsCounters[ENDS_COUNT] = {0};

    Log("Executing: ", 11);
    Log(cmd, cmdSize);

    SendData(hComm, cmd, cmdSize);

    while (pos < outSize &&
           (readStatus = ReadByte(hComm, out + pos)) == RESPONSE_OK) {
        lastRead = out[pos];
        for (uint8_t i = 0; i < ENDS_COUNT; i++) {
            if (lastRead != ends[i][endsCounters[i]]) {
                endsCounters[i] = 0;
            } else {
                endsCounters[i] += 1;
                if (endsCounters[i] == endSize[i]) {
                    *read = pos + 1;
                    Log("Response: OK\r\n", 14);
                    return RESPONSE_OK;
                }
            }
        }
        pos += 1;
    }

    *read = pos;
    switch (readStatus) {
        case RESPONSE_OK:
            Log("Response: OK\r\n", 14);
            break;
        case RESPONSE_ERROR:
            Log("Response: ERROR\r\n", 17);
            break;
        case RESPONSE_TIMEOUT:
            Log("Response: TIMEOUT\r\n", 19);
            break;
    }
    return readStatus;
}
