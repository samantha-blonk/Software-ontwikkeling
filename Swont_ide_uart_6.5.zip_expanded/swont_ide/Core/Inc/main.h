/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"

#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#include "stm32_ub_vga_screen.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VGA_BLUE0_Pin GPIO_PIN_8
#define VGA_BLUE0_GPIO_Port GPIOE
#define VGA_BLUE1_Pin GPIO_PIN_9
#define VGA_BLUE1_GPIO_Port GPIOE
#define VGA_GREEN0_Pin GPIO_PIN_10
#define VGA_GREEN0_GPIO_Port GPIOE
#define VGA_GREEN1_Pin GPIO_PIN_11
#define VGA_GREEN1_GPIO_Port GPIOE
#define VGA_GREEN2_Pin GPIO_PIN_12
#define VGA_GREEN2_GPIO_Port GPIOE
#define VGA_RED0_Pin GPIO_PIN_13
#define VGA_RED0_GPIO_Port GPIOE
#define VGA_RED1_Pin GPIO_PIN_14
#define VGA_RED1_GPIO_Port GPIOE
#define VGA_RED2_Pin GPIO_PIN_15
#define VGA_RED2_GPIO_Port GPIOE
#define VGA_HSYNC_Pin GPIO_PIN_11
#define VGA_HSYNC_GPIO_Port GPIOB
#define VGA_VSYNC_Pin GPIO_PIN_12
#define VGA_VSYNC_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define BYTE_BUFLEN 	 1
#define LINE_BUFLEN 	 1024
#define CARRIAGE_RETURN  13 /* carriage return char \r */
#define LINE_FEED 		 10 /* linefeed char \n		   */

#define FALSE 	0x00
#define TRUE 	0xFF

/* Struct's ------------------------------------------------------------------*/
typedef struct
{
	uint8_t byte_buffer_rx[BYTE_BUFLEN];	// Store the rx byte from the USART2
	char line_rx_buffer[LINE_BUFLEN];		// Buffer to hold all the bytes from rx USART2
	int msglen;
	volatile int char_counter;				// Counter for line_rx_buffer
	char command_execute_flag;				/* Set = whole transmission is received, ready for processing \
											   Reset = still receiving*/
}input_vars;
extern input_vars input;

/* Globals -------------------------------------------------------------------*/
extern volatile char container[1024];
extern volatile int temp;
extern volatile int key;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
