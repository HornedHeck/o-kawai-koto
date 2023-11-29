#include "basic_operations.h"

#include "stdio.h"
#include "unistd.h"
#include "sys/time.h"

void TogglePin(Pin pin) {
    printf("Toggle pin %d\n", pin);
}

void Delay(uint32_t milis) {
    usleep(milis * 1000);  // NOLINT
}

uint32_t GetTick(){
    struct timeval t_val;
    gettimeofday(&t_val, NULL);
    return t_val.tv_sec * 1000 + t_val.tv_usec / 1000;
}