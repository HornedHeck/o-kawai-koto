#include "communications_ex.h"

#include <string.h>

#include "communications.h"
#include "stddef.h"

static const DataSequence ends[] = {
    {
        .data = "OK\r\n",
        .size = 4,
    },
    {
        .data = "ERROR\r\n",
        .size = 7,
    },
};

static const char commands[3][50] = {
    "AT+CWMODE=1,0", "AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PWD "\"",
    "AT+CIPSTA?"};

static void GetNextByte(const CommunicationHandle *handle, uint8_t *buffer) {
    ReceiveData(handle, buffer, 1);
}

uint16_t BlockingRead(const CommunicationHandle *handle, uint8_t *buffer,
                      uint16_t bufferSize, DataSequence start,
                      const DataSequence *ends, uint16_t endsCount) {
    uint16_t endSequenceCounters[endsCount];
    uint16_t readCount = 0;

    uint16_t startSequenceCounter = 0;
    uint8_t localBuffer;
    while (startSequenceCounter < start.size) {
        GetNextByte(handle, &localBuffer);
        if (localBuffer == (uint8_t) start.data[startSequenceCounter]) {
            startSequenceCounter++;
        } else {
            startSequenceCounter = 0;
        }
    }

    while (readCount < bufferSize) {
        GetNextByte(handle, &localBuffer);
        buffer[readCount] = localBuffer;
        readCount++;
        for (uint16_t j = 0; j < endsCount; j++) {
            if (localBuffer == (uint8_t) ends[j].data[endSequenceCounters[j]]) {
                endSequenceCounters[j]++;
                if (endSequenceCounters[j] == ends[j].size) {
                    return readCount - endSequenceCounters[j];
                }
            } else {
                endSequenceCounters[j] = 0;
            }
        }
    }
    return 0;
}

uint16_t ExecuteAtCommand(const CommunicationHandle *handle, uint8_t *buffer,
                          uint16_t bufferSize, ATCommand ATCommand) {
    DataSequence commandData = {
        .data = commands[ATCommand],
        .size = strlen(commands[ATCommand]),
    };
    SendData(handle, (const uint8_t *) commandData.data, commandData.size);
    return BlockingRead(handle, buffer, bufferSize, commandData, ends, 2);
}
