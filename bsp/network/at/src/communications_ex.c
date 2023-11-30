#include "communications_ex.h"

#include <string.h>

#include "communications.h"
#include "detectors.h"
#include "log.h"
#include "stddef.h"
#include "stdio.h"

ResponseStatus Execute(const CommunicationHandle *hComm, const uint8_t *cmd,
                       uint16_t cmdSize, uint8_t *out, uint16_t outSize,
                       uint16_t *read) {
    uint8_t lastRead;
    ResponseStatus readStatus = RESPONSE_ERROR;
    uint16_t pos = 0;

    Log("Executing: ", 11);
    Log(cmd, cmdSize);

    SendData(hComm, cmd, cmdSize);

    while (pos < outSize &&
           (readStatus = ReadByte(hComm, out + pos)) == RESPONSE_OK) {
        lastRead = out[pos];
        bool ok_detected = Detect(lastRead, DETECTABLE_OK);
        bool error_detected = Detect(lastRead, DETECTABLE_ERROR);
        if (ok_detected) {
            Log("Response: OK\r\n", 14);
            return RESPONSE_OK;
        }
        if (error_detected) {
            Log("Response: ERROR\r\n", 17);
            return RESPONSE_ERROR;
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
