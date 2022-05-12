/*
 * Frontlayer.c
 *
 *  Created on: 29 Apr 2022
 *      Author: Samantha
 */
#include "Frontlayer.h"
#include "usart.h"
#include "main.h"

char command[12];

uint8_t value;
uint8_t trigger;
//input_vars input;

void UartTx()
{
	printf("fakka kill ");
}

char receive()
{
	trigger = 1;
	for(uint8_t a =0; a<input.msglen; a++)
	{
		if(input.line_rx_buffer[a] != ',' && trigger == 1)
			command[a] = input.line_rx_buffer[a];
		if(input.line_rx_buffer[a] == ',' || input.line_rx_buffer[a] == ' ')
			trigger = 0;
	}
return command;
}

