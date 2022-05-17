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


uint8_t Line_s[6];
uint8_t bitmap_s[3];
uint8_t rectangle_s[6];
uint8_t clearscreen_s;

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
			Line_s[0] = arg(1);
			Line_s[1] = arg(2);
			Line_s[2] = arg(3);
			Line_s[3] = arg(4);
			Line_s[4] = arg(5);
			Line_s[5] = arg(6);
			break;
		case text:
			printf("sexy met belly");
			break;
		case rectangle:
			rectangle_s[0] = arg(1);
			rectangle_s[1] = arg(2);
			rectangle_s[2] = arg(3);
			rectangle_s[3] = arg(4);
			rectangle_s[4] = arg(5);
			rectangle_s[5] = arg(6);
			break;
		case bitmap:
			bitmap_s[0] = arg(1);
			bitmap_s[1] = arg(2);
			bitmap_s[2] = arg(3);
			break;
		case clearscreen:
			clearscreen_s = arg(1);
			break;
		default:
			break;
	}


return 1;
}


int arg(int a)
{
	uint8_t i = 0;
	char stored_args = 0;
	uint8_t arg_counter = 0;
	uint8_t arg_character_counter = 0;
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
				else if(arg_counter == a)
				{
					string_container[arg_character_counter++] = input.line_rx_buffer[i++];
				}
				else i++;
			}
	return atoi(string_container);
}




