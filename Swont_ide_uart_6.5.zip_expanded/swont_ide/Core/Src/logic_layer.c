/*
 * logic_layer.c
 *
 *  Created on: 12 mei 2022
 *      Author: thrij
 */
#include "Frontlayer.h"
#include "logic_layer.h"
#include "main.h"


char rood[4] = "rood";
char blauw[5] = "blauw";
char zwart[5] = "zwart";
char lblauw[10] = "lichtblauw";
char groen[5] = "groen";
char lgroen[10] = "lichtgroen";
char cyaan[5] = "cyaan";
char lcyaan[10] = "lichtcyaan";
char lrood[9] = "lichtrood";
char magenta[7] = "magenta";
char lmagenta[12] = "lichtmagenta";
char bruin[5] = "bruin";
char geel[4] = "geel";
char grijs[5] = "grijs";
char wit[3] = "wit";

char consolas[] = "consolas", ariel[] = "ariel";
char normaal[] = "normaal" , vet[] = "vet", cursief[] = "cursief";

int color_check(char color[12]);
int style_check(char stl[7]);
int font_check(char fnt[8]);

//--------------------------------------------------------------
// @brief Logic function.
// @details This function conects the frond layer with the io layer.
//
// @param[in] x: The X-coordinate of the top left corner of the bitmap
// @param[in] y: The Y-coordinate of the top left corner of the bitmap
// @param[in] bmNr: The number ID of the bitmap to be loaded in
//--------------------------------------------------------------
void logic()
{

	switch(input.byte_buffer_rx[firstCharacter])
	{
		case line:
			 printf("comando lijn");

			 UB_VGA_SetLine(line_s.x1,
							 line_s.x1,
							 line_s.y1,
							 line_s.x2,
							 line_s.y2,
							 color_check(line_s.color),
							 line_s.weight);
			 break;
		case rectangle:
			printf("comando rechthoek");
			UB_VGA_DrawRectangle(rectangle_s.xlup,
					rectangle_s.ylup,
					rectangle_s.width,
					rectangle_s.height,
					color_check(rectangle_s.color),
					rectangle_s.filled,
					rectangle_s.bordercolor,
					rectangle_s.linewidth);
			break;
		case text:
			printf("comando tekst");
			UB_VGA_writeText(text_s.xlup,
					text_s.ylup,
					color_check(text_s.color),
					text_s.textin,
					font_check(text_s.fontname),
					text_s.fontsize,
					style_check(text_s.fontstyle));
			break;
		case bitmap:
			printf("comando clearscherm");
			UB_VGA_DrawBitmap(bitmap_s.nr,
					bitmap_s.xlup,
					bitmap_s.ylup);
			break;
		case clearscreen:
			printf("comando bitmap");
			UB_VGA_clearScreen(color_check(clearscreen_s.color));
			break;
		default:
			printf("onbekent comando");
			break;
	}
}


//--------------------------------------------------------------
// @brief Function check font type.
// @details This function checks which font is askt.
//
// @param[in] fnt: char whit font type
//
// @return return font type as 1 or 2
//--------------------------------------------------------------
int font_check(char fnt[8])
{
	int font;
	uint8_t result;

	result = strcmp(fnt, ariel);
	if (result == 0)
	{
		font = 1;
	}
	result = strcmp(fnt, consolas);
	if (result == 0)
	{
		font = 2;
	}
	else
	{
		printf("foute kleur");
	}



	return font;
}

//--------------------------------------------------------------
// @brief Function check font style.
// @details This function looks which font style should be displayd (normaal, vet, cursief).
//
// @param[in] the char withe the fond style in it
//
// @return return the style code (1,2,3) 1 = normaal, 2 = vet, 3 = cursief
//--------------------------------------------------------------
int style_check(char stl[7])
{
	int style;
	uint8_t result;

	result = strcmp(stl, normaal);
	if (result == 0)
	{
		style = 1;
	}
	result = strcmp(stl, vet);
	if (result == 0)
	{
		style = 2;
	}
	result = strcmp(stl, cursief);
	if (result == 0)
	{
		style = 3;
	}
	else
	{
		printf("foute kleur");
	}

	return style;
}

//--------------------------------------------------------------
// @brief Function checks color.
// @details this functions checks which color code belongs to which hex value.
//
// @param[in] color: char with the color
//
// @return return the color code
//--------------------------------------------------------------
int color_check(char color[12])
{
		int ret_val;
		uint8_t result;

		switch(color[0])
		{
		case 'z':
			result = strcmp(color, zwart);
			if (result == 0)
			{
			ret_val = VGA_COL_BLACK;
			}
			else
			{
				printf("foute kleur");
			}
		case 'l':
			switch(color[5])
			{
			case 'b':
				result = strcmp(color, lblauw);
				if (result == 0)
				{
				ret_val = VGA_COL_LIGHTBLUE;
				}
				else
				{
					printf("foute kleur");
				}
			case 'g':
				result = strcmp(color, lgroen);
				if (result == 0)
				{
				ret_val = VGA_COL_LIGHTGREEN;
				}
				else
				{
					printf("foute kleur");
				}
			case 'r':
				result = strcmp(color, lrood);
				if (result == 0)
				{
				ret_val = VGA_COL_LIGHTRED;
				}
				else
				{
					printf("foute kleur");
				}
			case 'c':
				result = strcmp(color, lcyaan);
				if (result == 0)
				{
				ret_val = VGA_COL_LIGHTCYAN;
				}
				else
				{
					printf("foute kleur");
				}
			case 'm':
				result = strcmp(color, lmagenta);
				if (result == 0)
				{
				ret_val = VGA_COL_LIGHTMAGENTA;
				}
				else
				{
					printf("foute kleur");
				}
			}
		case 'r':
			result = strcmp(color, rood);
			if (result == 0)
			{
			ret_val = VGA_COL_RED;
			}
			else
			{
				printf("foute kleur");
			}
		case 'w':
			result = strcmp(color, wit);
			if (result == 0)
			{
			ret_val = VGA_COL_WHITE;
			}
			else
			{
				printf("foute kleur");
			}
		case 'g':
			result = strcmp(color, geel);
			if (result == 0)
			{
			ret_val = VGA_COL_YELLOW;
			}
			result = strcmp(color, grijs);
			if (result == 0)
			{
			ret_val = VGA_COL_GRAY;
			}
			result = strcmp(color, groen);
			if (result == 0)
			{
			ret_val = VGA_COL_GREEN;
			}
			else
			{
				printf("foute kleur");
			}
		case 'c':
			result = strcmp(color, cyaan);
			if (result == 0)
			{
			ret_val = VGA_COL_CYAN;
			}
			else
			{
				printf("foute kleur");
			}
		case 'm':
			result = strcmp(color, magenta);
			if (result == 0)
			{
			ret_val = VGA_COL_MAGENTA;
			}
			else
			{
				printf("foute kleur");
			}
		case 'b':
			result = strcmp(color, blauw);
			if (result == 0)
			{
			ret_val = VGA_COL_BLUE;
			}
			result = strcmp(color, bruin);
			if (result == 0)
			{
			ret_val = VGA_COL_BROWN;
			}
			else
			{
				printf("foute kleur");
			}
	//	default: ret_val = VGA_COL_BLACK;
		}

	return ret_val;
}

