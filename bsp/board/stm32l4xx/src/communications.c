#include "communications.h"

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"

UART_HandleTypeDef huart1;
static CommunicationHandle *h_comm_uart1;
UART_HandleTypeDef huart2;
static CommunicationHandle *h_comm_uart2;

static inline ResponseStatus MapHALResponse(HAL_StatusTypeDef response) {
    switch (response) {
        case HAL_OK:
            return RESPONSE_OK;
            break;
        case HAL_BUSY:
        case HAL_ERROR:
            return RESPONSE_ERROR;
            break;
        case HAL_TIMEOUT:
            return RESPONSE_TIMEOUT;
            break;
    }
    return RESPONSE_ERROR;
}

static inline void ClearITReceiveData(CommunicationHandle *handle) {
    handle->buffer = NULL;
    handle->buffer_size = 0;
    handle->it_read_callback = NULL;
}

void InitCommunication(CommunicationHandle *handle) {
    if (handle->protocol == UART && handle->portNum == 2) {
        huart2.Instance = USART2;
        huart2.Init.BaudRate = 115200;  // NOLINT
        huart2.Init.WordLength = UART_WORDLENGTH_8B;
        huart2.Init.StopBits = UART_STOPBITS_1;
        huart2.Init.Parity = UART_PARITY_NONE;
        huart2.Init.Mode = UART_MODE_TX_RX;
        huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart2.Init.OverSampling = UART_OVERSAMPLING_16;
        huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart2) == HAL_OK) {
            handle->hwHandle = &huart2;
            h_comm_uart2 = handle;
        }
    } else if (handle->protocol == UART && handle->portNum == 1) {
        huart1.Instance = USART1;
        huart1.Init.BaudRate = 115200;
        huart1.Init.WordLength = UART_WORDLENGTH_8B;
        huart1.Init.StopBits = UART_STOPBITS_1;
        huart1.Init.Parity = UART_PARITY_NONE;
        huart1.Init.Mode = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling = UART_OVERSAMPLING_16;
        huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart1) == HAL_OK) {
            handle->hwHandle = &huart1;
            h_comm_uart1 = handle;
        }
    }
}

ResponseStatus SendData(const CommunicationHandle *handle, const uint8_t *data,
                        uint16_t size) {
    return MapHALResponse(HAL_UART_Transmit(handle->hwHandle, data, size, 100));
}

ResponseStatus ReceiveData(const CommunicationHandle *handle, uint8_t *buffer,
                           uint16_t bufferSize) {
    return MapHALResponse(
        HAL_UART_Receive(handle->hwHandle, buffer, bufferSize, 100));
}

ResponseStatus ReadByte(const CommunicationHandle *handle, uint8_t *dst) {
    return MapHALResponse(HAL_UART_Receive(handle->hwHandle, dst, 1, 5000));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    /*
    TODO
    Array of huart handles
    Array of pointers to CommHandle

    Knowing start of the huart array, addres of param and sizeof exact pos can
    be found After that we can take item from the same pos in Comm Handle array
    */
    CommunicationHandle *h_comm;
    if (huart == &huart1) {
        h_comm = h_comm_uart1;
    } else if (huart == &huart2) {
        h_comm = h_comm_uart2;
    } else{
        return;
    }
    if (h_comm->it_read_callback == NULL) {
        return;
    }
    if (h_comm->it_read_callback()) {
        HAL_UART_Receive_IT(huart, h_comm->buffer, h_comm->buffer_size);
    } else {
        ClearITReceiveData(h_comm);
    }
}

void EnableITReceive(CommunicationHandle *handle, uint8_t *buffer,
                     uint16_t buffer_size, bool (*it_read_callback)()) {
    // TODO(HornedHeck) check existing and error
    handle->buffer = buffer;
    handle->buffer_size = buffer_size;
    handle->it_read_callback = it_read_callback;
    HAL_UART_Receive_IT(handle->hwHandle, buffer, buffer_size);
}

void DisableITReceive(CommunicationHandle *handle) {
    HAL_UART_AbortReceive_IT(handle->hwHandle);
    ClearITReceiveData(handle);
}
