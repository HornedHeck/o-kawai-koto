#include "communications_ex.h"

#include "communications.h"

static void GetNextByte(CommunicationHandle *handle, uint8_t *buffer) {
    ReceiveData(handle, buffer, 1);
}

uint16_t BlockingRead(CommunicationHandle *handle, uint8_t *buffer,
                      uint16_t bufferSize, DataSequence start,
                      DataSequence *ends, uint16_t endsCount) {
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
