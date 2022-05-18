/*
 * Frontlayer.c
 *
 *  Created on: 29 Apr 2022
 *      Author: Samantha
 */
#include "Frontlayer.h"
#include "usart.h"
#include "main.h"
#include <stdbool.h>

void arg(uint8_t, char*, bool);

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
	switch(input.line_rx_buffer[firstCharacter])
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

}

//--------------------------------------------------------------
// @brief Function reads argument from uart
// @details This function reads a certain argument from the uart and puts this in a string
//
// @param[in] a: The variable that determines which argument has to be read
// @param[in] *string_container: A pointer where the string will be put into
// @param[in] text_trigger: A boolean that determines if the argument is a text or not
//--------------------------------------------------------------
void arg(uint8_t a,char *string_container ,bool text_trigger)
{
	uint8_t i = 0;	//counter
	uint8_t j = 0;	//counter
	uint8_t arg_counter = 0;	//is needed to check what argument is currently beeing read
	uint8_t arg_character_counter = 0;	//checks what the current character from the argument is
	for (j=0; j<input.msglen; j++)	//cleans string container
	string_container[j] = 0;
	while(i <= input.msglen)
			{
				if(input.line_rx_buffer[i] == ',')	//if current character is a ',' the argument counter goes up
				{
					arg_counter++;
					i++;
				}
				else if(input.line_rx_buffer[i] == ' ' && text_trigger == false)	//if its a space it doesnt get read
				i++;
				else if(input.line_rx_buffer[i] == ' ' && text_trigger == true && arg_counter == a)	//if its a space but its in a text argument, it gets read
				{
					string_container[arg_character_counter++] = input.line_rx_buffer[i++];
				}
				else if(arg_counter == a)	//if its a character in the correct argument it gets read
				string_container[arg_character_counter++] = input.line_rx_buffer[i++];
				else i++;	//else skips
			}
}




