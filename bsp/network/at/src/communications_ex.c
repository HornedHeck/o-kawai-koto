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
    "AT+CWMODE=1,0\r\n",
    "AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PWD "\"\r\n",
    "AT+CIPSTA?\r\n",
};

uint16_t BlockingRead(const CommunicationHandle *handle, uint8_t *buffer,
                      uint16_t bufferSize, DataSequence start,
                      const DataSequence *ends, uint16_t endsCount) {
    uint16_t endSequenceCounters[endsCount];
    uint16_t readCount = 0;

    uint16_t startSequenceCounter = 0;
    uint8_t localBuffer;
    while (startSequenceCounter < start.size &&
           ReadByte(handle, &localBuffer) == 0) {
        if (localBuffer == (uint8_t) start.data[startSequenceCounter]) {
            startSequenceCounter++;
        } else {
            startSequenceCounter = 0;
        }
    }

    while (readCount < bufferSize && ReadByte(handle, &localBuffer) == 0) {
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

uint16_t ExecuteCommand(const CommunicationHandle *handle, uint8_t *buffer,
                        uint16_t bufferSize, ATCommand ATCommand) {
    return ExecuteAtCommand(handle, buffer, bufferSize, commands[ATCommand],
                            strlen(commands[ATCommand]));
}

uint16_t ExecuteAtCommand(const CommunicationHandle *handle, uint8_t *buffer,
                          uint16_t bufferSize, const char *atCommand,
                          const uint16_t atCommandSize) {
    DataSequence commandData = {
        .data = atCommand,
        .size = atCommandSize,
    };
    SendData(handle, (const uint8_t *) commandData.data, commandData.size);
    return BlockingRead(handle, buffer, bufferSize, commandData, ends, 2);
}