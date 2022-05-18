//--------------------------------------------------------------
// @author Franc van der Bent & Niels Vollebregt
//
// @brief The API to drive the VGA-output
// @details The API can draw a single pixel and fill the entire screen with a particular color to a VGA-monitor
//
// @version 1.0
// @date 29/04/2022
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_vga_screen.h"
#include "bitmap.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim1_up;


uint8_t VGA_RAM1[(VGA_DISPLAY_X+1)*VGA_DISPLAY_Y];
VGA_t VGA;

//--------------------------------------------------------------
// @brief The initialisation of the VGA driver
// @details The function initialises the timer registers of timer 1 & 2.
//			It also initialises the DMA register.
//--------------------------------------------------------------
void UB_VGA_Screen_Init(void)
{
  VGA.hsync_cnt = 0;
  VGA.start_adr = 0;
  VGA.dma2_cr_reg = 0;

  GPIOB->BSRR = VGA_VSYNC_Pin;

  // TIM2
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3);

  // TIM1
  __HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_UPDATE);
  __HAL_TIM_ENABLE(&htim1);
  HAL_DMA_Start_IT(&hdma_tim1_up, (uint32_t)&VGA_RAM1[0], VGA_GPIOE_ODR_ADDRESS, VGA_DISPLAY_X + 1);

  HAL_DMA_Init(&hdma_tim1_up);
  __HAL_DMA_ENABLE_IT(&hdma_tim1_up, DMA_IT_TC);

  //-----------------------
  // Register swap and safe
  //-----------------------
  // content of CR-Register read and save
  VGA.dma2_cr_reg = DMA2_Stream5->CR;
}


//--------------------------------------------------------------
// @brief Function to fill the whole screen with a single color.
// @details This function writes for all pixels a single color to the ram.
//
// @param[in] color The 8-bit color-code to display
//--------------------------------------------------------------
void UB_VGA_FillScreen(uint8_t color)
{
  uint16_t xp,yp;

  for(yp = 0; yp < VGA_DISPLAY_Y; yp++) {
    for(xp = 0; xp < VGA_DISPLAY_X; xp++) {
      UB_VGA_SetPixel(xp, yp, color);
    }
  }
}


//--------------------------------------------------------------
// @brief Function to color a single pixel
// @details This function writes a single color of a single pixel to the ram.
//
// @param[in] xp The X-coordinate of the pixel
// @param[in] yp The Y-coordinate of the pixel
// @param[in] color The 8-bit color-code to display
//--------------------------------------------------------------
void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color)
{
  if(xp >= VGA_DISPLAY_X)
    xp = 0;
  if(yp >= VGA_DISPLAY_Y)
    yp = 0;

  // Write pixel to ram
  VGA_RAM1[(yp * (VGA_DISPLAY_X + 1)) + xp] = color;
}


//--------------------------------------------------------------
// @brief Function to draw a line.
// @details This function draws a single color to a line with a customisable width from a
//			custom coordinate (point 1) to another custom coordinate (point 2).
//
// @param[in] x1 The X-coordinate of point 1 of the line
// @param[in] y1 The Y-coordinate of point 1 of the line
// @param[in] x2 The X-coordinate of point 2 of the line
// @param[in] y2 The Y-coordinate of point 2 of the line
// @param[in] color The 8-bit color-code to display
// @param[in] width The width of the line in pixels
//--------------------------------------------------------------
void UB_VGA_SetLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color, uint16_t width)
{
  int16_t dx;
  int16_t dy;
  uint16_t px;
  uint16_t py;
  uint16_t dxabs;
  uint16_t dyabs;
  uint16_t i; 								//Count the x or y pixels
  uint16_t j; 								//Count amount of width pixels
  uint8_t neg;
  uint16_t amPerSide;
  float slope;

  dx=x2-x1;      							//the horizontal distance of the line
  dy=y2-y1;      							//the vertical distance of the line
  dxabs=abs(dx);
  dyabs=abs(dy);
  amPerSide = width / 2;
  neg = width % 2;

  if (dxabs>=dyabs) 						//the line is more horizontal than vertical
  {
    slope=(float)dy / (float)dx;
    for(i=0;i!=dx;i++)
    {
      px=i+x1;
      py=slope*i+y1;						//Calculate Y-value for the current Y-value
      UB_VGA_SetPixel(px,py,color);

      for(j = 0; j <= amPerSide; j++)
      {
          py = slope * i + y1 + j;			//Calculate extra width-pixels on the positive side
          UB_VGA_SetPixel(px,py,color);

          if(!(neg) && (j == amPerSide))
        	  break;						//Break when positive and last width pixel

          py = slope * i + y1 - j;			//Calculate extra width-pixels on the negative side
          UB_VGA_SetPixel(px,py,color);
      }
    }
  }
  else 										//the line is more vertical than horizontal
  {
    slope=(float)dx / (float)dy;
    for(i=0;i!=dy;i++)
    {
      px=slope*i+x1; 						//Calculate X-value for the current Y-value
      py=i+y1;

      UB_VGA_SetPixel(px,py,color);

      for(j = 0; j <= amPerSide; j++)
      {
          px = slope * i + x1 + j;			//Calculate extra width-pixels on the positive side
    	  UB_VGA_SetPixel(px,py,color);

    	  if(!(neg) && (j == amPerSide))
    		  break;						//Break when positive and last width pixel

          px = slope * i + x1 - j;			//Calculate extra width-pixels on the negative side
    	  UB_VGA_SetPixel(px,py,color);
      }
    }
  }
}

//--------------------------------------------------------------
// @brief Function to draw a rectangle.
// @details This function draws a rectangle starting with the coördinates of the top left
//			corner of the rectangle. It also has the potential to fill the rectangle with a color.
//
// @param[in] xp: The X-coordinate of the top left corner of the rectangle
// @param[in] yp: The Y-coordinate of the top left corner of the rectangle
// @param[in] width: The width of the entire rectangle
// @param[in] height: The height of the entire rectangle
// @param[in] color: The 8-bit color-code to display as filling of the rectangle
// @param[in] filled: The value that determines if the rectangle is filled or not
// @param[in] bordercolor: The 8-bit color-code to display as the border of the rectangle
// @param[in] lineWidth: The width of the line that determines the border of the rectangle
//--------------------------------------------------------------

void UB_VGA_DrawRectangle(uint16_t xp, uint16_t yp, uint8_t width, uint8_t height, uint8_t color, uint8_t filled,
		uint8_t bordercolor, uint8_t lineWidth)
{
	uint16_t px;
	uint16_t py;
	uint16_t i;
	uint16_t j;

	if(xp >= VGA_DISPLAY_X)
	  xp = 0;
	if(yp >= VGA_DISPLAY_Y)
	  yp = 0;

	/* the vertical part of the rectangle */
	for(j=0; j < lineWidth; j++)
	{
		for(i=0; i <= height; i++)
		{
			py = yp + i;
			px = xp + j;
			UB_VGA_SetPixel(px,py,bordercolor);
		}
		for(i=0; i <= height; i++)
		{
			py = yp + i;
			px = xp + width - j;
			UB_VGA_SetPixel(px,py,bordercolor);
		}
	}

    /* the horizontal part of the rectangle */
	for(j=0; j < lineWidth; j++)
	{
		for(i=0; i <= width-lineWidth; i++)
		{
			py = yp + j;
			px = xp + i + lineWidth;
			UB_VGA_SetPixel(px,py,bordercolor);
		}
		for(i=0; i <= width-lineWidth; i++)
		{
			py = yp + height - j;
			px = xp + i + lineWidth;
			UB_VGA_SetPixel(px,py,bordercolor);
		}
	}

    /* filling in the rectangle */
    if(filled == 1)
    {
    	for(i=0; i <= width-lineWidth*2; i++)
    	{
    		px = xp + i + lineWidth;

        	for(j=0; j <= height-(lineWidth*2); j++)
        	{
        		py = yp + j + lineWidth;
        		UB_VGA_SetPixel(px,py,color);
        	}
    	}
    }

}

//--------------------------------------------------------------
// @brief Function to clear the whole screen.
// @details This function writes for all pixels the color white to the ram.
//--------------------------------------------------------------
void UB_VGA_clearScreen()
{
  uint16_t xp,yp;

  for(yp = 0; yp < VGA_DISPLAY_Y; yp++) {
    for(xp = 0; xp < VGA_DISPLAY_X; xp++) {
      UB_VGA_SetPixel(xp, yp, VGA_COL_WHITE);
    }
  }
}

//--------------------------------------------------------------
// @brief Function load in a bitmap.
// @details This function loads a designated bitmap on designated coordinates.
//
// @param[in] x: The X-coordinate of the top left corner of the bitmap
// @param[in] y: The Y-coordinate of the top left corner of the bitmap
// @param[in] bmNr: The number ID of the bitmap to be loaded in
//--------------------------------------------------------------
void UB_VGA_DrawBitmap(uint16_t x, uint16_t y, uint16_t bmNr, uint8_t color, uint8_t double_size)
{
  	uint16_t i; //Counts y coordinate VGA
  	uint16_t j; //Counts x coordinate VGA
  	uint8_t xBm; //Counts x of the bitmap
  	uint8_t yBm; //Counts y of the bitmap
  	uint16_t bmCoord;
  	uint8_t doubleXFlag;
  	uint8_t doubleYFlag;

  	doubleXFlag = 0;
  	doubleYFlag = 0;
  	bmCoord = 0;
  	yBm = 0;
  	xBm = 0;

  	for(i = 0; i < BM_AMOUNT; i++) //Look for bm ID
  	{
  		if(bmNr == bmLookup[i][ID])
  		{
  			bmNr = i;
  			break;
  		}
  	}

  	bmCoord = bmLookup[bmNr][OFFSET];

  	for(i = 0; i < VGA_DISPLAY_Y; i++)
    {
  		if(i >= y)
  		{
  			if(yBm == bmLookup[bmNr][Y_LEN]) //Check for Y length of bm
  				break;
  			for(j = 0; j < VGA_DISPLAY_X; j++)
  			{
  				if(j >= x)
  				{
  					if(xBm == bmLookup[bmNr][X_LEN]) //Check for X length of bm
  						break;

  					if (color)
  					{
  						if (bitmap_test[bmCoord] == 0XFF)
  						{
  							UB_VGA_SetPixel(j, i, color);
  						}
  						else
  							UB_VGA_SetPixel(j, i, 0xFF);
  					}
  					else
  						UB_VGA_SetPixel(j, i, bitmap_test[bmCoord]);

  					if(double_size && !doubleXFlag)
  						doubleXFlag++;
  					else
  					{
  						doubleXFlag--;
  	  					xBm++;
  	  					bmCoord++;
  					}
  				}
	    	}
  			xBm = 0;


			if(double_size && !doubleYFlag)
			{
				doubleYFlag++;
				bmCoord -= bmLookup[bmNr][X_LEN];
			}
			else
			{
				doubleYFlag--;
				yBm++;
			}
  		}
    }
}

//--------------------------------------------------------------
// @brief Function write text on screen
// @details This function checks the fontname, fontsize and fontstyle and prints the appropriate
//			letter that fits the given parameters
// @param[in] x_lup: The X-coordinate of the top left corner of the printed letter
// @param[in] y_lup: The Y-coordinate of the top left corner of the printed letter
// @param[in] color: The color code which the letter is printed in
// @param[in] text: The text that has the be written on the screen
// @param[in] fontname: The fontname that the text is printed in
// @param[in] fontsize: The fontsize that the text is printed in
// @param[in] fontstyle: The fontstyle that the text is printed in
//--------------------------------------------------------------
void UB_VGA_writeText(uint16_t x_lup, uint16_t y_lup, uint8_t color, char text[20], char fontname, uint8_t fontsize, uint8_t fontstyle)
{
    uint16_t px = 0;
    uint8_t i = 0;
    uint8_t letterWidth = 0;
    if(fontsize == 1)	//normal fontsize is selected
    {
        letterWidth = 8;
    }
    else if(fontsize == 2)	//large fontsize is selected
    {
        letterWidth = 16;
    }


    for(i=0; i < 20; i++)	//maximum text size of 20 letters
    {
        px = x_lup + (i * letterWidth);	//changes the position of x
        switch(text[i])
        {
            case 'a':	//if the letter is 'a'
            	switch(fontname)	//checks what the fontname is
            	{
            		case ARIAL_FONT:	//arial font
            			switch(fontstyle)	//checks what fontname is
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, a, color, fontsize); break;	//normal fontstyle
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, A, color, fontsize); break;	//bold fontstyle
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 0, color, fontsize); break;	//cursive fontstyle
            				default: break;
            			}
                	case CONSOLAS_FONT:	//consolas font
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 27, color, fontsize); break;	//normal fontstyle
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 500, color, fontsize); break;	//bold fontstyle
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 527, color, fontsize); break;	//cursive fontstyle
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'b':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, b, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, B, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 1, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 28, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 501, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 528, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'c':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, c, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, C, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 2, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 29, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 502, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 529, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'd':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, d, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, D, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 3, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 30, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 503, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 530, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'e':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, e, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, E, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 4, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 31, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 504, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 531, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'f':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, f, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, F, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 5, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 32, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 505, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 532, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'g':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, g, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, G, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 6, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 33, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 506, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 533, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'h':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, h, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, H, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 7, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 34, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 507, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 534, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'i':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, i, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, I, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 8, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 35, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 508, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 535, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'j':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, j, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, J, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 9, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 36, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 509, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 536, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'k':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, k, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, K, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 10, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 37, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 510, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 537, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'l':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, l, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, L, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 11, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 38, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 511, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 538, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'm':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, m, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, M, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 12, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 39, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 512, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 539, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'n':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, n, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, N, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 13, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 40, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 513, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 540, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'o':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, o, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, O, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 14, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 41, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 514, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 541, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'p':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, p, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, P, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 15, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 42, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 515, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 542, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'q':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, q, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, Q, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 16, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 43, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 516, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 543, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'r':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, r, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, R, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 17, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 44, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 517, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 544, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 's':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, s, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, S, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 18, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 45, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 518, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 545, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
            	break;
            case 't':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, t, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, T, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 19, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 46, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 519, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 546, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'u':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, u, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, U, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 20, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 47, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 520, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 547, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'v':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, v, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, V, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 21, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 48, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 521, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 548, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'w':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, w, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, W, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 22, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 49, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 522, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 549, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'x':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, x, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, X, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 23, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 50, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 523, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 550, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'y':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, y, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, Y, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 24, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 51, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 524, color, fontsize); break;
                			case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 551, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            case 'z':
            	switch(fontname)
            	{
            		case ARIAL_FONT:
            			switch(fontstyle)
            			{
            				case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, z, color, fontsize); break;
            				case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, Z, color, fontsize); break;
            				case CURSIVE_STYLE: UB_VGA_DrawBitmap(px, y_lup, 25, color, fontsize); break;
            				default: break;
            			}
                	case CONSOLAS_FONT:
                		switch(fontstyle)
                		{
                			case NORMAL_STYLE: UB_VGA_DrawBitmap(px, y_lup, 52, color, fontsize); break;
                			case BOLD_STYLE: UB_VGA_DrawBitmap(px, y_lup, 525, color, fontsize); break;
                			case CURSIVE_STYLE UB_VGA_DrawBitmap(px, y_lup, 552, color, fontsize); break;
                			default: break;
                		}
                	default: break;
            	}
                break;
            default:
                break;


        }
    }
}

