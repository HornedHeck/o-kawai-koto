#ifndef APP_ENCODER
#define APP_ENCODER

#include <stdint.h>

#define TIMESPAN        20
#define FS              48000
#define CHANNELS        2
#define FRAME_SIZE      (FS * TIMESPAN / 1000)
#define BUFFER_SIZE     (FRAME_SIZE * CHANNELS)
#define MAX_FRAME_SIZE  FRAME_SIZE
#define BUFFER_OUT_SIZE (MAX_FRAME_SIZE * CHANNELS)
#define OUTPUT_MAX_SIZE 4000

void InitEncoder();

int16_t EncodePacket(const uint16_t *pcm, uint8_t *encoded_data);

int16_t DecodePacket(const uint8_t *encoded_data, uint16_t encoded_size,
                     uint16_t *pcm);

bool IsEncodeResultOk(int16_t);

bool IsDecodeResultOk(int16_t);

bool IsNeedToSend(int16_t);

#endif /* APP_ENCODER */
