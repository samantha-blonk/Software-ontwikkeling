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

//typedef struct
//{
//	uint8_t nr;
//    uint16_t xlup;
//    uint16_t ylup;
//}bitmap;
//
//typedef struct
//{
//	uint8_t color;
//}clearscreen;
//
//typedef struct
//{
//    uint16_t x1;
//    uint16_t y1;
//    uint16_t x2;
//    uint16_t y2;
//    uint8_t color;
//    uint8_t weight;
//}Line_s;
//extern Line_s line_s;
//
//typedef struct
//{
//    uint16_t xlup;
//    uint16_t ylup;
//    uint8_t width;
//    uint8_t height;
//    uint8_t color;
//    uint8_t filled;
//    uint8_t bordercolor;
//    uint8_t linewidth;
//} rectangle;
//
//typedef struct
//{
//    uint16_t xlup;
//    uint16_t ylup;
//    uint8_t color;
//    char text[maxTextLen];
//    char fontname[maxFontnameLen];
//    uint8_t fontsize;
//    uint8_t fontstyle;
//}text;
//



void UartTx(void);
char receive(void);


#endif /* INC_FRONTLAYER_H_ */
