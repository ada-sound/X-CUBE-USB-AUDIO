  /**
  ******************************************************************************
  * @file    stm32f769i_discovery_audio_ex.h
  * @author  MCD Application Team 
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32446e_eval_audio_ex.c driver.
  *          It is extended  from [stm32f769i_discovery_audio.h, version V2.0.0, published in STM32Cube for STM32F7 firmware 1.6.0].
  *          modified fields : 
  *                                - AUDIO_OUT_SAIx_DMAx_PERIPH_DATA_SIZE
  *                                  & AUDIO_OUT_SAIx_DMAx_MEM_DATA_SIZE:  modified to support 24bit (32bit with padding)
  *                                - AUDIO_OUT_IRQ_PREPRIO &AUDIO_IN_IRQ_PREPRIO :prioritize audio interrupts.
  *                                - BSP_AUDIO_IN_Get_PcmBuffer : added  to  get PCM sample 24/16 bit.
  *                                
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F769I_DISCOVERY_AUDIO_EX_H
#define __STM32F769I_DISCOVERY_AUDIO_EX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Include audio component Driver */
#include "wm8994_ex.h"
#include "stm32f769i_discovery.h"
#include <stdlib.h>

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F769I_DISCOVERY
  * @{
  */
    
/** @defgroup STM32F769I_DISCOVERY_AUDIO STM32F769I_DISCOVERY_AUDIO
  * @{
  */

/** @defgroup STM32F769I_DISCOVERY_AUDIO_Exported_Types STM32F769I_DISCOVERY_AUDIO Exported Types
  * @{
  */
/**
  * @}
  */ 

/** @defgroup STM32F769I_DISCOVERY_AUDIO_Exported_Constants STM32F769I_DISCOVERY_AUDIO Exported Constants
  * @{
  */

/** @defgroup BSP_Audio_Sample_Rate BSP Audio Sample Rate
  * @{
  */
#define BSP_AUDIO_FREQUENCY_96K         SAI_AUDIO_FREQUENCY_96K
#define BSP_AUDIO_FREQUENCY_48K         SAI_AUDIO_FREQUENCY_48K
#define BSP_AUDIO_FREQUENCY_44K         SAI_AUDIO_FREQUENCY_44K
#define BSP_AUDIO_FREQUENCY_32K         SAI_AUDIO_FREQUENCY_32K
#define BSP_AUDIO_FREQUENCY_22K         SAI_AUDIO_FREQUENCY_22K
#define BSP_AUDIO_FREQUENCY_16K         SAI_AUDIO_FREQUENCY_16K
#define BSP_AUDIO_FREQUENCY_11K         SAI_AUDIO_FREQUENCY_11K
#define BSP_AUDIO_FREQUENCY_8K          SAI_AUDIO_FREQUENCY_8K
/**
  * @}
  */
    
/*------------------------------------------------------------------------------
                          USER SAI defines parameters
 -----------------------------------------------------------------------------*/
/** CODEC_AudioFrame_SLOT_TDMMode In W8994 codec the Audio frame contains 4 slots : TDM Mode
  * TDM format :
  * +------------------|------------------|--------------------|-------------------+ 
  * | CODEC_SLOT0 Left | CODEC_SLOT1 Left | CODEC_SLOT0 Right  | CODEC_SLOT1 Right |
  * +------------------------------------------------------------------------------+
  */
/* To have 2 separate audio stream in Both headphone and speaker the 4 slot must be activated */
#define CODEC_AUDIOFRAME_SLOT_0123                   SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1 | SAI_SLOTACTIVE_2 | SAI_SLOTACTIVE_3

/* To have an audio stream in headphone only SAI Slot 0 and Slot 2 must be activated */ 
#define CODEC_AUDIOFRAME_SLOT_02                     SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_2 
/* To have an audio stream in speaker only SAI Slot 1 and Slot 3 must be activated */ 
#define CODEC_AUDIOFRAME_SLOT_13                     SAI_SLOTACTIVE_1 | SAI_SLOTACTIVE_3


/* SAI peripheral configuration defines */
#define AUDIO_OUT_SAIx                           SAI1_Block_A
#define AUDIO_OUT_SAIx_CLK_ENABLE()              __HAL_RCC_SAI1_CLK_ENABLE()
#define AUDIO_OUT_SAIx_CLK_DISABLE()             __HAL_RCC_SAI1_CLK_DISABLE()
#define AUDIO_OUT_SAIx_AF                        GPIO_AF6_SAI1

#define AUDIO_OUT_SAIx_MCLK_ENABLE()             __HAL_RCC_GPIOG_CLK_ENABLE()
#define AUDIO_OUT_SAIx_MCLK_GPIO_PORT            GPIOG
#define AUDIO_OUT_SAIx_MCLK_PIN                  GPIO_PIN_7
#define AUDIO_OUT_SAIx_SD_FS_CLK_ENABLE()        __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_OUT_SAIx_SD_FS_SCK_GPIO_PORT       GPIOE
#define AUDIO_OUT_SAIx_FS_PIN                    GPIO_PIN_4   
#define AUDIO_OUT_SAIx_SCK_PIN                   GPIO_PIN_5
#define AUDIO_OUT_SAIx_SD_PIN                    GPIO_PIN_6

/* SAI DMA Stream definitions */
#define AUDIO_OUT_SAIx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_OUT_SAIx_DMAx_STREAM               DMA2_Stream1
#define AUDIO_OUT_SAIx_DMAx_CHANNEL              DMA_CHANNEL_0
#define AUDIO_OUT_SAIx_DMAx_IRQ                  DMA2_Stream1_IRQn


#define AUDIO_OUT_SAIx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_OUT_SAIx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD

#define AUDIO_OUT_SAIx_DMAx_PERIPH_DATA_SIZE_16     DMA_PDATAALIGN_HALFWORD
#define AUDIO_OUT_SAIx_DMAx_MEM_DATA_SIZE_16        DMA_MDATAALIGN_HALFWORD
#define AUDIO_OUT_SAIx_DMAx_PERIPH_DATA_SIZE_24  DMA_PDATAALIGN_WORD
#define AUDIO_OUT_SAIx_DMAx_MEM_DATA_SIZE_24     DMA_MDATAALIGN_WORD
#define AUDIO_OUT_SAIx_DMAx_PERIPH_DATA_SIZE_32  DMA_PDATAALIGN_WORD
#define AUDIO_OUT_SAIx_DMAx_MEM_DATA_SIZE_32     DMA_MDATAALIGN_WORD

#define DMA_MAX_SZE                              0xFFFF
   
#define AUDIO_OUT_SAIx_DMAx_IRQHandler           DMA2_Stream1_IRQHandler

/* Select the interrupt preemption priority and subpriority for the DMA interrupt */
#define AUDIO_OUT_IRQ_PREPRIO                    ((uint32_t)0x02) 

/*------------------------------------------------------------------------------
                        AUDIO IN CONFIGURATION
------------------------------------------------------------------------------*/
/* SAI peripheral configuration defines */
#define AUDIO_IN_SAIx                           SAI1_Block_B
#define AUDIO_IN_SAIx_CLK_ENABLE()              __HAL_RCC_SAI1_CLK_ENABLE()
#define AUDIO_IN_SAIx_CLK_DISABLE()             __HAL_RCC_SAI1_CLK_DISABLE()
#define AUDIO_IN_SAIx_AF                        GPIO_AF6_SAI1
#define AUDIO_IN_SAIx_SD_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_IN_SAIx_SD_GPIO_PORT              GPIOE
#define AUDIO_IN_SAIx_SD_PIN                    GPIO_PIN_3

/* SAI DMA Stream definitions */
#define AUDIO_IN_SAIx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_IN_SAIx_DMAx_STREAM               DMA2_Stream4
#define AUDIO_IN_SAIx_DMAx_CHANNEL              DMA_CHANNEL_1
#define AUDIO_IN_SAIx_DMAx_IRQ                  DMA2_Stream4_IRQn
#define AUDIO_IN_SAIx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_IN_SAIx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD

#define AUDIO_IN_INT_GPIO_ENABLE()               __HAL_RCC_GPIOJ_CLK_ENABLE()
#define AUDIO_IN_INT_GPIO_PORT                   GPIOJ
#define AUDIO_IN_INT_GPIO_PIN                    GPIO_PIN_12
#define AUDIO_IN_INT_IRQ                         EXTI15_10_IRQn

/* DFSDM Configuration defines */
#define AUDIO_DFSDMx_TOP_RIGHT_CHANNEL                  DFSDM_CHANNEL_0
#define AUDIO_DFSDMx_TOP_LEFT_CHANNEL                   DFSDM_CHANNEL_1
#define AUDIO_DFSDMx_BUTTOM_RIGHT_CHANNEL               DFSDM_CHANNEL_4
#define AUDIO_DFSDMx_BUTTOM_LEFT_CHANNEL                DFSDM_CHANNEL_5

#define AUDIO_DFSDMx_TOP_LEFT_FILTER                    DFSDM1_Filter0
#define AUDIO_DFSDMx_TOP_RIGHT_FILTER                   DFSDM1_Filter1
#define AUDIO_DFSDMx_BUTTOM_LEFT_FILTER                 DFSDM1_Filter2
#define AUDIO_DFSDMx_BUTTOM_RIGHT_FILTER                DFSDM1_Filter3
   
#define AUDIO_DFSDMx_CLK_ENABLE()                       __HAL_RCC_DFSDM1_CLK_ENABLE()
#define AUDIO_DFSDMx_CKOUT_PIN                          GPIO_PIN_3
#define AUDIO_DFSDMx_CKOUT_DMIC_GPIO_PORT               GPIOD    
#define AUDIO_DFSDMx_CKOUT_DMIC_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define AUDIO_DFSDMx_DMIC_DATIN1_PIN                    GPIO_PIN_3
#define AUDIO_DFSDMx_DMIC_DATIN5_PIN                    GPIO_PIN_11   
#define AUDIO_DFSDMx_DMIC_DATIN_GPIO_PORT               GPIOC
#define AUDIO_DFSDMx_DMIC_DATIN_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_DFSDMx_DMIC_DATIN_AF                      GPIO_AF3_DFSDM1
#define AUDIO_DFSDMx_CKOUT_AF                           GPIO_AF3_DFSDM1
    
/* DFSDM DMA Right and Left channels definitions */
#define AUDIO_DFSDMx_DMAx_CLK_ENABLE()                  __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_DFSDMx_DMAx_CHANNEL                       DMA_CHANNEL_8
#define AUDIO_DFSDMx_DMAx_PERIPH_DATA_SIZE              DMA_PDATAALIGN_WORD
#define AUDIO_DFSDMx_DMAx_MEM_DATA_SIZE                 DMA_MDATAALIGN_WORD

#define AUDIO_DFSDMx_DMAx_TOP_LEFT_STREAM               DMA2_Stream0
#define AUDIO_DFSDMx_DMAx_TOP_LEFT_IRQ                  DMA2_Stream0_IRQn   
#define AUDIO_DFSDMx_DMAx_TOP_LEFT_IRQHandler           DMA2_Stream0_IRQHandler

#define AUDIO_DFSDMx_DMAx_TOP_RIGHT_STREAM              DMA2_Stream5
#define AUDIO_DFSDMx_DMAx_TOP_RIGHT_IRQ                 DMA2_Stream5_IRQn
#define AUDIO_DFSDMx_DMAx_TOP_RIGHT_IRQHandler          DMA2_Stream5_IRQHandler

#define AUDIO_DFSDMx_DMAx_BUTTOM_LEFT_STREAM            DMA2_Stream6
#define AUDIO_DFSDMx_DMAx_BUTTOM_LEFT_IRQ               DMA2_Stream6_IRQn   
#define AUDIO_DFSDMx_DMAx_BUTTOM_LEFT_IRQHandler        DMA2_Stream6_IRQHandler

#define AUDIO_DFSDMx_DMAx_BUTTOM_RIGHT_STREAM           DMA2_Stream7
#define AUDIO_DFSDMx_DMAx_BUTTOM_RIGHT_IRQ              DMA2_Stream7_IRQn
#define AUDIO_DFSDMx_DMAx_BUTTOM_RIGHT_IRQHandler       DMA2_Stream7_IRQHandler
 
/* Select the interrupt preemption priority and subpriority for the DMA interrupt */
#define AUDIO_IN_IRQ_PREPRIO                ((uint32_t)0x04)


/*------------------------------------------------------------------------------
             CONFIGURATION: Audio Driver Configuration parameters
------------------------------------------------------------------------------*/



/* Audio status definition */     
#define AUDIO_OK                            ((uint8_t)0)
#define AUDIO_ERROR                         ((uint8_t)1)
#define AUDIO_TIMEOUT                       ((uint8_t)2)

/* Audio In default settings */
#define DEFAULT_AUDIO_IN_FREQ               BSP_AUDIO_FREQUENCY_16K
#define DEFAULT_AUDIO_IN_BIT_RESOLUTION     ((uint8_t)16)
#define DEFAULT_AUDIO_IN_CHANNEL_NBR        ((uint8_t)2)
#define DEFAULT_AUDIO_IN_VOLUME             ((uint16_t)64)

/*------------------------------------------------------------------------------
                            OUTPUT DEVICES definition
------------------------------------------------------------------------------*/
/* Alias on existing output devices to adapt for 2 headphones output */
#define OUTPUT_DEVICE_HEADPHONE1 OUTPUT_DEVICE_HEADPHONE
#define OUTPUT_DEVICE_HEADPHONE2 OUTPUT_DEVICE_SPEAKER /* Headphone2 is connected to Speaker output of the wm8994 */

/*------------------------------------------------------------------------------
                           INPUT DEVICES definition
------------------------------------------------------------------------------*/
/* MP34DT01TR digital microphone on PCB top side */
#define INPUT_DEVICE_DIGITAL_MIC       ((uint16_t)0)
/* Analog microphone input from 3.5 audio jack connector */    
#define INPUT_DEVICE_ANALOG_MIC        INPUT_DEVICE_INPUT_LINE_1  

/**
  * @}
  */
   
/** @defgroup STM32F769I_DISCOVERY_AUDIO_Exported_Macros STM32F769I_DISCOVERY_AUDIO Exported Macros
  * @{
  */
#define DMA_MAX(x)           (((x) <= DMA_MAX_SZE)? (x):DMA_MAX_SZE)
/**
  * @}
  */ 

/** @defgroup STM32F769I_DISCOVERY_AUDIO_OUT_Exported_Functions STM32F769I_DISCOVERY_AUDIO_OUT Exported Functions
  * @{
  */
uint8_t BSP_AUDIO_OUT_Init_Ext(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq, uint8_t AudioResolution);
void    BSP_AUDIO_OUT_DeInit(void);
uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size);
void    BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size);
uint8_t BSP_AUDIO_OUT_Pause(void);
uint8_t BSP_AUDIO_OUT_Resume(void);
uint8_t BSP_AUDIO_OUT_Stop(uint32_t Option);
uint8_t BSP_AUDIO_OUT_SetVolume(uint8_t Volume);
void    BSP_AUDIO_OUT_SetFrequency(uint32_t AudioFreq);
void    BSP_AUDIO_OUT_SetAudioFrameSlot(uint32_t AudioFrameSlot);
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd);
uint8_t BSP_AUDIO_OUT_SetOutputMode(uint8_t Output);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function is called when the requested data has been completely transferred.*/
void    BSP_AUDIO_OUT_TransferComplete_CallBack(void);

/* This function is called when half of the requested buffer has been transferred. */
void    BSP_AUDIO_OUT_HalfTransfer_CallBack(void);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void    BSP_AUDIO_OUT_Error_CallBack(void);

/* These function can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void  BSP_AUDIO_OUT_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t AudioFreq, void *Params);
void  BSP_AUDIO_OUT_MspInit(SAI_HandleTypeDef *hsai, void *Params);
void  BSP_AUDIO_OUT_MspDeInit(SAI_HandleTypeDef *hsai, void *Params);

/**
  * @}
  */ 

/** @defgroup STM32F769I_DISCOVERY_AUDIO_IN_Exported_Functions STM32F769I_DISCOVERY_AUDIO_IN Exported Functions
  * @{
  */
uint8_t BSP_AUDIO_IN_Init(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
uint8_t BSP_AUDIO_IN_InitEx(uint16_t InputDevice, uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
uint8_t BSP_AUDIO_IN_AllocScratch (int32_t *pScratch, uint32_t size);
uint8_t BSP_AUDIO_IN_GetChannelNumber(void);
void    BSP_AUDIO_IN_DeInit(void);
uint8_t BSP_AUDIO_IN_Record(uint16_t *pData, uint32_t Size);
uint8_t BSP_AUDIO_IN_Stop(void);
uint8_t BSP_AUDIO_IN_Pause(void);
uint8_t BSP_AUDIO_IN_Resume(void);
uint8_t BSP_AUDIO_IN_Get_PcmBuffer(uint8_t* pbuf, uint16_t sample_count,
                                   uint16_t ScratchOffset, uint8_t res);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function should be implemented by the user application.
   It is called into this driver when the current buffer is filled to prepare the next
   buffer pointer and its size. */
void    BSP_AUDIO_IN_TransferComplete_CallBack(void);
void    BSP_AUDIO_IN_HalfTransfer_CallBack(void);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void    BSP_AUDIO_IN_Error_CallBack(void);
     
/* These function can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void BSP_AUDIO_IN_ClockConfig(DFSDM_Filter_HandleTypeDef *hdfsdm_filter, uint32_t AudioFreq, void *Params);
void BSP_AUDIO_IN_MspInit(void);
void BSP_AUDIO_IN_MspDeInit(void);

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F769I_DISCOVERY_AUDIO_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
