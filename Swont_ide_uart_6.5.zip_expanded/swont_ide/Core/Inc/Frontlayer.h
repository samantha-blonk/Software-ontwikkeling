/*
 * Frontlayer.h
 *
 *  Created on: 29 Apr 2022
 *      Author: Samantha
 */

#ifndef INC_FRONTLAYER_H_
#define INC_FRONTLAYER_H_


#include "main.h"

#define maxTextLen 128
#define maxFontnameLen 30
#define maxColorLen 12
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
#define firstCharacter 0

typedef struct
{
	uint8_t nr;
    uint16_t xlup;
    uint16_t ylup;
}BITMAP_S;

typedef struct
{
	char color[maxColorLen];
}CLEARSCREEN_S;

typedef struct
{
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    char color[maxColorLen];
    uint8_t weight;
}LINE_S;

typedef struct
{
    uint16_t xlup;
    uint16_t ylup;
    uint8_t width;
    uint8_t height;
    char color[maxColorLen];
    uint8_t filled;
    char bordercolor[maxColorLen];
    uint8_t linewidth;
} RECTANGLE_S;

typedef struct
{
    uint16_t xlup;
    uint16_t ylup;
    char color[maxColorLen];
    char textin[maxTextLen];
    char fontname[maxFontnameLen];
    uint8_t fontsize;
    char fontstyle[maxColorLen];
}TEXT_S;

void FL_Input(void);


#endif /* INC_FRONTLAYER_H_ */
