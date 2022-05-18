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

