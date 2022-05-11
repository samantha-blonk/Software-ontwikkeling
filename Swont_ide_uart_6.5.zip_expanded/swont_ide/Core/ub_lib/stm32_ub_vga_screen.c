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

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim1_up;


uint8_t VGA_RAM1[(VGA_DISPLAY_X+1)*VGA_DISPLAY_Y];
VGA_t VGA;

//--------------------------------------------------------------
// @brief The SGN function
// @details Adds the mathematical SGN function
//
// @param[in] x The value to be converted to sign
//
// @ return sgn When x is positive returns 1, if x is negative returns -1. Otherwise returns 0.
//--------------------------------------------------------------
int8_t sgn(int8_t x)
{
	if(x > 0) return 1;
	if(x < 0) return -1;
	return 0;
}

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
  uint16_t i;
  uint16_t j;
  uint8_t neg;
  uint16_t amPerSide;
  float slope;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs=abs(dx);
  dyabs=abs(dy);
  amPerSide = width / 2;
  neg = width % 2;

  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    slope=(float)dy / (float)dx;
    for(i=0;i!=dx;i++)
    {
      px=i+x1;
      py=slope*i+y1;
      UB_VGA_SetPixel(px,py,color);

      for(j = 0; j <= amPerSide; j++)
      {
          py = slope * i + y1 + j;
          UB_VGA_SetPixel(px,py,color);

          if(!(neg) && (j == amPerSide))
        	  break;

          py = slope * i + y1 - j;
          UB_VGA_SetPixel(px,py,color);
      }
    }
  }
  else /* the line is more vertical than horizontal */
  {
    slope=(float)dx / (float)dy;
    for(i=0;i!=dy;i++)
    {
      px=slope*i+x1;
      py=i+y1;

      UB_VGA_SetPixel(px,py,color);

      for(j = 0; j <= amPerSide; j++)
      {
          px = slope * i + x1 + j;
    	  UB_VGA_SetPixel(px,py,color);

    	  if(!(neg) && (j == amPerSide))
    		  break;

          px = slope * i + x1 - j;
    	  UB_VGA_SetPixel(px,py,color);
      }
    }
  }
}
