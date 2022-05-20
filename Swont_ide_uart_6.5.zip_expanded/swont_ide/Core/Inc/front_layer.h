//--------------------------------------------------------------
// File     : front_layer.h
//--------------------------------------------------------------

#ifndef INC_FRONT_LAYER_H_
#define INC_FRONT_LAYER_H_

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------
// Define max values
//--------------------------------------------------------------
#define maxTextLen 128
#define maxFontnameLen 30
#define maxColorLen 12

//--------------------------------------------------------------
// Define first letter of argument
//--------------------------------------------------------------
#define line 'l'
#define text 't'
#define rectangle 'r'
#define bitmap 'b'
#define clearscreen 'c'

//--------------------------------------------------------------
// Define the argument length
//--------------------------------------------------------------
#define lineLen 6
#define textLen 7
#define rectangleLen 8
#define bitmapLen 3
#define clearscreenLen 1

//--------------------------------------------------------------
// Define bool value
//--------------------------------------------------------------
#define NOTEXT 0
#define YESTEXT 1

//--------------------------------------------------------------
// Define argument id
//--------------------------------------------------------------
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

//--------------------------------------------------------------
// Bitmap Structure
//--------------------------------------------------------------
typedef struct
{
	uint8_t nr;
    uint16_t xlup;
    uint16_t ylup;
}BITMAP_S;

//--------------------------------------------------------------
// Clearscreen Structure
//--------------------------------------------------------------
typedef struct
{
	char color[maxColorLen];
}CLEARSCREEN_S;

//--------------------------------------------------------------
// Line Structure
//--------------------------------------------------------------
typedef struct
{
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    char color[maxColorLen];
    uint8_t weight;
}LINE_S;

//--------------------------------------------------------------
// Rectangle Structure
//--------------------------------------------------------------
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

//--------------------------------------------------------------
// Text Structure
//--------------------------------------------------------------
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

//--------------------------------------------------------------
// Global Structures
//--------------------------------------------------------------
extern LINE_S line_s;
extern BITMAP_S bitmap_s;
extern CLEARSCREEN_S clearscreen_s;
extern LINE_S line_s;
extern RECTANGLE_S rectangle_s;
extern TEXT_S text_s;

//--------------------------------------------------------------
// Global Function call
//--------------------------------------------------------------
void FL_Input(void);


#endif /* INC_FRONT_LAYER_H_ */
