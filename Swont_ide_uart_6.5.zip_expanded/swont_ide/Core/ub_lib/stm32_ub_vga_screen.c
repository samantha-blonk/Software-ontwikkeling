//--------------------------------------------------------------
// @author
//
// @brief
// @details
//
// @version
// @date
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
// @brief
// @details
//
// @param[in]
// @param[out]
// @param[in,out]
//
// @return
//
// @retval
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
// @brief
// @details
//
// @param[in]
// @param[out]
// @param[in,out]
//
// @return
//
// @retval
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
// @brief
// @details
//
// @param[in]
// @param[out]
// @param[in,out]
//
// @return
//
// @retval
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
