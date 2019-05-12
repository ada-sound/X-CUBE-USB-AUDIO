/**
  ******************************************************************************
  * @file    stm32f7xx_it.c 
  * @author  MCD Application Team 
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019  STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd;
/* SAI handler declared in "stm32f769i_discovery_audio.c" file */
extern SAI_HandleTypeDef haudio_out_sai;

#if USE_AUDIO_TIMER_VOLUME_CTRL
extern TIM_HandleTypeDef    TimHandle;
#endif /* USE_AUDIO_TIMER_VOLUME_CTRL */

extern DFSDM_Filter_HandleTypeDef       hAudioInTopLeftFilter;
extern DFSDM_Filter_HandleTypeDef       hAudioInTopRightFilter;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f7xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_FS_INTO_HS
void OTG_HS_IRQHandler(void)
#else
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
#endif
{
  HAL_PCD_IRQHandler(&hpcd);
}

#if USE_AUDIO_TIMER_VOLUME_CTRL
/**
  * @brief  This function handlestt TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM_VolumeChangeIRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}
#endif /* USE_AUDIO_TIMER_VOLUME_CTRL */

/**
  * @brief This function handles DMA2 Stream 4 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_OUT_SAIx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}

/**
  * @brief This function handles DMA2 Stream 0 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_DFSDMx_DMAx_TOP_LEFT_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hAudioInTopLeftFilter.hdmaReg);
}

/**
  * @brief This function handles DMA2 Stream 0 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_DFSDMx_DMAx_TOP_RIGHT_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hAudioInTopRightFilter.hdmaReg);
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
