#ifndef BSP_NETWORK_AT_INCLUDE_COMMUNICATIONS_EX
#define BSP_NETWORK_AT_INCLUDE_COMMUNICATIONS_EX

#include "communications.h"
#include "stdint.h"



ResponseStatus Execute(const CommunicationHandle *, const uint8_t *, uint16_t,
                       uint8_t *, uint16_t, uint16_t *);

#endif /* BSP_NETWORK_AT_INCLUDE_COMMUNICATIONS_EX */
