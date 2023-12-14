#ifndef BSP_NETWORK_AT_INCLUDE_DETECTORS
#define BSP_NETWORK_AT_INCLUDE_DETECTORS

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DETECTABLE_OK,
    DETECTABLE_ERROR,
    DETECTABLE_READY_FOR_DATA,
    DETECTABLE_INCOMING_DATA,
} Detectable;

bool Detect(uint8_t, Detectable);

int8_t MassDetect(uint8_t, Detectable[], int8_t);

#endif /* BSP_NETWORK_AT_INCLUDE_DETECTORS */
