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
#define text 't'
#define rectangle 'r'
#define bitmap 'b'
#define clearscreen 'c'
#define NOTEXT 0
#define YESTEXT 1
#define argumentZERO 0
#define argumentONE 1
#define argumentTWO 2
#define argumentTHREE 3
#define argumentFOUR 4
#define argumentFIVE 5
#define argumentSIX 6
#define argumentSEVEN 7
#define argumentEIGHT 8


void arg(uint8_t, char*, uint8_t);


uint8_t value;
uint8_t trigger;
char string_container[128];

LINE_S line_s;
BITMAP_S bitmap_s;
CLEARSCREEN_S clearscreen_s;
LINE_S line_s;
RECTANGLE_S rectangle_s;
TEXT_S text_s;


//--------------------------------------------------------------
// @brief Function load input into structs
// @details This function loads the different arguments into the assigned structs
//--------------------------------------------------------------
void FL_Input()
{
	switch(input.line_rx_buffer[0])
	{
		case line:	//if the function line is called
			arg(argumentONE, string_container, NOTEXT);	//collects the information from argument 1
			line_s.x1 = atoi(string_container);	//stores the information from argument 1 into line struct
			arg(argumentTWO, string_container, NOTEXT);	//etc
			line_s.y1 = atoi(string_container);
			arg(argumentTHREE, string_container, NOTEXT);
			line_s.x2 = atoi(string_container);
			arg(argumentFOUR, string_container, NOTEXT);
			line_s.y2 = atoi(string_container);
			arg(argumentFIVE, string_container, NOTEXT);
			strcpy(line_s.color, string_container);
			arg(argumentSIX, string_container, NOTEXT);
			line_s.weight = atoi(string_container);
			break;
		case text:	//if the function text is called
			arg(argumentONE, string_container, NOTEXT);	//collects the information from argument 1
			text_s.xlup = atoi(string_container);	//stores the information from argument 1 into text struct
			arg(argumentTWO, string_container, NOTEXT);	//etc
			text_s.ylup = atoi(string_container);
			arg(argumentTHREE, string_container, NOTEXT);
			strcpy(text_s.color, string_container);
			arg(argumentFOUR, string_container, YESTEXT);
			strcpy(text_s.textin, string_container);
			arg(argumentFIVE, string_container, YESTEXT);
			strcpy(text_s.fontname, string_container);
			arg(argumentSIX, string_container, NOTEXT);
			text_s.fontsize = atoi(string_container);
			arg(argumentSEVEN, string_container, NOTEXT);
			strcpy(text_s.fontstyle, string_container);
			break;
		case rectangle:	//if the function rectangle is called
			arg(argumentONE, string_container, NOTEXT);	//collects the information from argument 1
			rectangle_s.xlup = atoi(string_container);	//stores the information from argument 1 into rectangle struct
			arg(argumentTWO, string_container, NOTEXT);
			rectangle_s.ylup = atoi(string_container);
			arg(argumentTHREE, string_container, NOTEXT);
			rectangle_s.width = atoi(string_container);
			arg(argumentFOUR, string_container, NOTEXT);
			rectangle_s.height = atoi(string_container);
			arg(argumentFIVE, string_container, NOTEXT);
			strcpy(rectangle_s.color, string_container);
			arg(argumentSIX, string_container, NOTEXT);
			rectangle_s.filled = atoi(string_container);
			arg(argumentSEVEN, string_container, NOTEXT);
			strcpy(rectangle_s.bordercolor, string_container);
			arg(argumentEIGHT, string_container, NOTEXT);
			rectangle_s.linewidth = atoi(string_container);
			break;
		case bitmap:	//if the function bitmap is called
			arg(argumentONE, string_container, NOTEXT);	//collects the information from argument 1
			bitmap_s.nr = atoi(string_container);	//stores the information from argument 1 into bitmap struct
			arg(argumentTWO, string_container, NOTEXT);
			bitmap_s.xlup = atoi(string_container);
			arg(argumentTHREE, string_container, NOTEXT);
			bitmap_s.ylup = atoi(string_container);
			break;
		case clearscreen:	//if the function clearscreen is called
			arg(argumentONE, string_container, NOTEXT);	//collects the information from argument 1
			strcpy(clearscreen_s.color, string_container);	//stores the information from argument 1 into line struct
			break;
		default:
			break;
	}


return 1;
}


void arg(uint8_t a,char *string_container ,uint8_t text_trigger)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t arg_counter = 0;
	uint8_t arg_character_counter = 0;
	for (j=0; j<input.msglen; j++)
	string_container[j] = 0;
	while(i <= input.msglen)
			{
				if(input.line_rx_buffer[i] == ',')
				{
					arg_counter++;
					i++;
				}
				else if(input.line_rx_buffer[i] == ' ' && text_trigger == 0)
				i++;
				else if(input.line_rx_buffer[i] == ' ' && text_trigger == 1 && arg_counter == a)
				{
					string_container[arg_character_counter++] = input.line_rx_buffer[i++];
				}
				else if(arg_counter == a)
				string_container[arg_character_counter++] = input.line_rx_buffer[i++];
				else i++;
			}
}




