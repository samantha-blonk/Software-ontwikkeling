/*
 * Frontlayer.c
 *
 *  Created on: 29 Apr 2022
 *      Author: Samantha
 */
#include "Frontlayer.h"
#include "usart.h"
#include "main.h"

//init
MX_USART2_UART_Init;



HAL_UART_Receive_IT(&huart2, data, BYTE_SIZE);

