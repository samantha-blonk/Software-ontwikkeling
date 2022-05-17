/*
 * Frontlayer.c
 *
 *  Created on: 29 Apr 2022
 *      Author: Samantha
 */
#include "Frontlayer.h"
#include "usart.h"
#include "main.h"

#define line 'l'
#define lineLenght 4
#define lineArguments 6

#define text 't'
#define textLenght 4
#define textArguments 7

#define rectangle 'r'
#define rectangleLenght 9
#define rectangleArguments 8

#define bitmap 'b'
#define bitmapLenght 6
#define bitmapArguments 3

#define clearscreen 'c'
#define clearscreenLenght 11
#define clearscreenArguments 1

char command[12];

uint8_t value;
uint8_t trigger;


void UartTx()
{
	printf("fakka kill ");
}

char receive()
{
	/*
	trigger = 1;
	for(uint8_t a =0; a<input.msglen; a++)
	{
		if(input.line_rx_buffer[a] != ',' && trigger == 1)
			command[a] = input.line_rx_buffer[a];
		if(input.line_rx_buffer[a] == ',' || input.line_rx_buffer[a] == ' ')
			trigger = 0;
	}
	*/

	switch(input.line_rx_buffer[0])
	{
		case line:
			line.x1 = argument(1);
			line.y1 = argument(2);
			line.x2 = argument(3);
			line.y2 = argument(4);
			line.color = argument(5);
			line.weight = argument(6);
			break;
		case text:

			break;
		case rectangle:
			break;
		case bitmap:
			break;
		case clearscreen:
			break;
		default:
			break;
	}


return command;
}


char argument(uint_8 argument)
{
	uint_8 i = 0;
	char stored_args = 0;
	uint_8 arg_counter = 0;
	uint_8 arg_character_counter = 0;
	char string_container[input.msglen];
	while(i <= input.msglen)
			{
				if(input.line_rx_buffer[i] == ',')
				{
					arg_counter++;
					i++;
				}
				else if(input.line_rx_buffer[i] == ' ')
				i++;
				else if(arg_counter == argument)
				{
					string_container[arg_character_counter++] = line_rx_buffer[i++];
				}
				else i++;
			}
	return string_container;
}




