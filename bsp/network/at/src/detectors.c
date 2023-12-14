#include "detectors.h"

#define SQ_COUNT         4
#define SQ_ERROR         "ERROR\r\n"
#define SQ_OK            "OK\r\n"
#define SQ_READ_FOR_DATA ">"
#define SQ_INCOMING_DATA "+IPD,"

static uint8_t counters[SQ_COUNT] = {0};
static const uint8_t *sqs[SQ_COUNT] = {
    SQ_OK,
    SQ_ERROR,
    SQ_READ_FOR_DATA,
    SQ_INCOMING_DATA,
};
static const uint16_t sqs_size[SQ_COUNT] = {
    sizeof SQ_OK - 1,
    sizeof SQ_ERROR - 1,
    sizeof SQ_READ_FOR_DATA - 1,
    sizeof SQ_INCOMING_DATA - 1,
};

bool Detect(uint8_t symbol, Detectable expected) {
    const uint8_t *sequence = sqs[expected];
    const uint16_t sequence_size = sqs_size[expected];
    if (sequence[counters[expected]] == symbol) {
        counters[expected]++;
        if (counters[expected] == sequence_size) {
            counters[expected] = 0;
            return true;
        }
    } else {
        counters[expected] = 0;
    }
    return false;
}

int8_t MassDetect(uint8_t symbol, Detectable expected[],
                  int8_t expected_count) {
    int8_t detected = -1;
    for (int8_t i = 0; i < expected_count; i++) {
        if (Detect(symbol, expected[i]) && detected != -1) {
            detected = i;
            // Need to pass all to reset counters;
        }
    }
    return detected;
}