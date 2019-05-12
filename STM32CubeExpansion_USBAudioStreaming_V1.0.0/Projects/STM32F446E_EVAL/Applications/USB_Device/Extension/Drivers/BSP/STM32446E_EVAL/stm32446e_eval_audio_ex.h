  /**
  ******************************************************************************
  * @file    stm32446e_eval_audio_ex.h
  * @author  MCD Application Team 
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32446e_eval_audio_ex.c driver.
  *          It is extended  from [stm32446e_eval_audio.h, version V2.0.0, published in STM32Cube for STM32F4 firmware 1.14.0].
  *          modified fields : 
  *                                - AUDIODATA_SIZE,AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE
  *                                  & AUDIO_SAIx_DMAx_MEM_DATA_SIZE:  modified to support 24bit (32bit with padding)
  *                                - AUDIO_OUT_IRQ_PREPRIO &AUDIO_IN_IRQ_PREPRIO :prioritize audio interrupts.
  *                                - DEFAULT_AUDIO_IN_FREQ  : emplaced by DEFAULT_AUDIO_IN_MAX_FREQ  to change frequency dynamically
  *                                - INTERNAL_BUFF_SIZE: fixed
  *                                - BSP_AUDIO_IN_ClockConfig: added function to support audio out clock setting
  *                                - BSP_AUDIO_IN_PDMToPCM : changed to support variable frequency (16 khz, or 48 khz))
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
#ifndef __STM32446E_EVAL_AUDIO_EX_H
#define __STM32446E_EVAL_AUDIO_EX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Include audio component Driver */
#include "../Components/wm8994/wm8994_ex.h"
#include "stm32446e_eval.h"
#include "../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc/pdm2pcm_glo.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32446E_EVAL
  * @{
  */
    
/** @addtogroup STM32446E_EVAL_AUDIO 
  * @{
  */

/** @defgroup STM32446E_EVAL_AUDIO_Exported_Types STM32446E EVAL AUDIO Exported Types
  * @{
  */
/**
  * @}
  */ 

/** @defgroup STM32446E_EVAL_AUDIO_Exported_Constants  STM32446E EVAL AUDIO Exported Constants
  * @{
  */
 
/*------------------------------------------------------------------------------
                          USER SAI defines parameters
 -----------------------------------------------------------------------------*/
/** @defgroup CODEC_AudioFrame_SLOT_TDMMode  STM32446E EVAL Audio Slot TDM mode 
  * @brief In W8994 codec the Audio frame contains 4 slots : TDM Mode
  * TDM format :
  * +------------------|------------------|--------------------|-------------------+ 
  * | CODEC_SLOT0 Left | CODEC_SLOT1 Left | CODEC_SLOT0 Right  | CODEC_SLOT1 Right |
  * +------------------------------------------------------------------------------+
  * @{
  */
/* To have 2 separate audio stream in Both headphone and speaker the 4 slot must be activated */
#define CODEC_AUDIOFRAME_SLOT_0123                   SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1 | SAI_SLOTACTIVE_2 | SAI_SLOTACTIVE_3
/* To have an audio stream in headphone only SAI Slot 0 and Slot 2 must be activated */ 
#define CODEC_AUDIOFRAME_SLOT_02                     SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_2 
/* To have an audio stream in speaker only SAI Slot 1 and Slot 3 must be activated */ 
#define CODEC_AUDIOFRAME_SLOT_13                     SAI_SLOTACTIVE_1 | SAI_SLOTACTIVE_3
/**
  * @}
  */

/* SAI peripheral configuration defines */
#define AUDIO_SAIx                           SAI2_Block_B
#define AUDIO_SAIx_CLK_ENABLE()              __HAL_RCC_SAI2_CLK_ENABLE()
#define AUDIO_SAIx_CLK_DISABLE()             __HAL_RCC_SAI2_CLK_DISABLE()
#define AUDIO_SAIx_SCK_AF                    GPIO_AF8_SAI2
#define AUDIO_SAIx_MCLK_SD_FS_AF             GPIO_AF10_SAI2

   
#define AUDIO_SAIx_MCLK_SCK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define AUDIO_SAIx_MCLK_SCK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()
#define AUDIO_SAIx_MCK_PIN                   GPIO_PIN_1 /* PA1 */
#define AUDIO_SAIx_SCK_PIN                   GPIO_PIN_2 /* PA2 */
#define AUDIO_SAIx_MCLK_SCK_GPIO_PORT        GPIOA
#define AUDIO_SAIx_SD_FS_ENABLE()            __HAL_RCC_GPIOG_CLK_ENABLE()
#define AUDIO_SAIx_SD_FS_DISABLE()           __HAL_RCC_GPIOG_CLK_DISABLE()
#define AUDIO_SAIx_FS_PIN                    GPIO_PIN_9 /* PG9 */
#define AUDIO_SAIx_SD_PIN                    GPIO_PIN_10 /* PG10 */
#define AUDIO_SAIx_SD_FS_GPIO_PORT           GPIOG


/* SAI DMA Stream definitions */
#define AUDIO_SAIx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_SAIx_DMAx_CLK_DISABLE()        __HAL_RCC_DMA2_CLK_DISABLE()
#define AUDIO_SAIx_DMAx_STREAM               DMA2_Stream6
#define AUDIO_SAIx_DMAx_CHANNEL              DMA_CHANNEL_3
#define AUDIO_SAIx_DMAx_IRQ                  DMA2_Stream6_IRQn
#define AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE  DMA_PDATAALIGN_HALFWORD
#define AUDIO_SAIx_DMAx_MEM_DATA_SIZE     DMA_MDATAALIGN_HALFWORD

#define AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE_16     DMA_PDATAALIGN_HALFWORD
#define AUDIO_SAIx_DMAx_MEM_DATA_SIZE_16        DMA_MDATAALIGN_HALFWORD
#define AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE_24  DMA_PDATAALIGN_WORD
#define AUDIO_SAIx_DMAx_MEM_DATA_SIZE_24     DMA_MDATAALIGN_WORD
#define AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE_32  DMA_PDATAALIGN_WORD
#define AUDIO_SAIx_DMAx_MEM_DATA_SIZE_32     DMA_MDATAALIGN_WORD

  
#define DMA_MAX_SZE                          0xFFFF
   
#define AUDIO_SAIx_DMAx_IRQHandler           DMA2_Stream6_IRQHandler

/* Select the interrupt preemption priority for the DMA interrupt */
#define AUDIO_OUT_IRQ_PREPRIO           ((uint32_t)2)   /* Select the preemption priority level(0 is the highest) */   

/*------------------------------------------------------------------------------
                        AUDIO IN CONFIGURATION
------------------------------------------------------------------------------*/
/* SPI Configuration defines */
#define AUDIO_I2Sx                          SPI3
#define AUDIO_I2Sx_CLK_ENABLE()             __HAL_RCC_SPI3_CLK_ENABLE()
#define AUDIO_I2Sx_CLK_DISABLE()            __HAL_RCC_SPI3_CLK_DISABLE()
#define AUDIO_I2Sx_SCK_PIN                   GPIO_PIN_3
#define AUDIO_I2Sx_SCK_GPIO_PORT             GPIOB
#define AUDIO_I2Sx_SCK_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_I2Sx_SCK_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define AUDIO_I2Sx_SCK_AF                    GPIO_AF6_SPI3

#define AUDIO_I2Sx_SD_PIN                   GPIO_PIN_1
#define AUDIO_I2Sx_SD_GPIO_PORT             GPIOC
#define AUDIO_I2Sx_SD_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_I2Sx_SD_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define AUDIO_I2Sx_SD_AF                    GPIO_AF5_SPI3

/* I2S DMA Stream Rx definitions */
#define AUDIO_I2Sx_DMAx_CLK_ENABLE()        __HAL_RCC_DMA1_CLK_ENABLE()
#define AUDIO_I2Sx_DMAx_CLK_DISABLE()       __HAL_RCC_DMA1_CLK_DISABLE()
#define AUDIO_I2Sx_DMAx_STREAM              DMA1_Stream2
#define AUDIO_I2Sx_DMAx_CHANNEL             DMA_CHANNEL_0
#define AUDIO_I2Sx_DMAx_IRQ                 DMA1_Stream2_IRQn
#define AUDIO_I2Sx_DMAx_PERIPH_DATA_SIZE    DMA_PDATAALIGN_HALFWORD
#define AUDIO_I2Sx_DMAx_MEM_DATA_SIZE       DMA_MDATAALIGN_HALFWORD
   
#define AUDIO_I2Sx_DMAx_IRQHandler          DMA1_Stream2_IRQHandler
  
/* Select the interrupt preemption priority and subpriority for the IT/DMA interrupt */
#define AUDIO_IN_IRQ_PREPRIO                ((uint32_t)4)   /* Select the preemption priority level(0 is the highest) */


/* Two channels are used:
   - one channel as input which is connected to I2S SCK in stereo mode 
   - one channel as output which divides the frequency on the input
*/

#define AUDIO_TIM_VolumeChangeCLK_ENABLE()             __HAL_RCC_TIM4_CLK_ENABLE()
#define AUDIO_TIM_VolumeChangeCLK_DISABLE()            __HAL_RCC_TIM4_CLK_DISABLE()
#define AUDIO_TIM_VolumeChange                          TIM4
#define AUDIO_TIM_VolumeChangeIN_CHANNEL               TIM_CHANNEL_1
#define AUDIO_TIM_VolumeChangeOUT_CHANNEL              TIM_CHANNEL_2 /* Select channel 2 as output */
#define AUDIO_TIM_VolumeChangeGPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_TIM_VolumeChangeGPIO_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()
#define AUDIO_TIM_VolumeChangeGPIO_PORT                GPIOB
#define AUDIO_TIM_VolumeChangeIN_GPIO_PIN              GPIO_PIN_6
#define AUDIO_TIM_VolumeChangeOUT_GPIO_PIN             GPIO_PIN_7
#define AUDIO_TIM_VolumeChangeAF                       GPIO_AF2_TIM4

/*------------------------------------------------------------------------------
             CONFIGURATION: Audio Driver Configuration parameters
------------------------------------------------------------------------------*/


/* Audio status definition */     
#define AUDIO_OK                            ((uint8_t)0)
#define AUDIO_ERROR                         ((uint8_t)1)
#define AUDIO_TIMEOUT                       ((uint8_t)2)

/* AudioFreq * DataSize (2 bytes) * NumChannels (Stereo: 2) */
#define DEFAULT_AUDIO_IN_MAX_FREQ           I2S_AUDIOFREQ_48K
#define DEFAULT_AUDIO_IN_BIT_RESOLUTION     ((uint8_t)16)
#define DEFAULT_AUDIO_IN_CHANNEL_NBR        ((uint8_t)2) /* Mono = 1, Stereo = 2 */
#define DEFAULT_AUDIO_IN_VOLUME             ((uint16_t)64)

/* PDM buffer input size */
   /*each 64 pdm sample produce 16 PCM sample then required size of buffer in ms is 
   (FREQ*RES*N_CHANNELS/1000)/16*64)*/
#define PDM_BUF_SIZE(freq) ((((int)freq/1000)*64/8)*((DEFAULT_AUDIO_IN_CHANNEL_NBR)))
/* PCM buffer output size */
#define PCM_OUT_SIZE(freq)                        ((freq)/1000*2)
#define CHANNEL_DEMUX_MASK                  ((uint8_t)0x55)
   
/*------------------------------------------------------------------------------
                    OPTIONAL Configuration defines parameters
------------------------------------------------------------------------------*/

/* Delay for the Codec to be correctly reset */
#define CODEC_RESET_DELAY           ((uint8_t)5)
   

/*------------------------------------------------------------------------------
                            OUTPUT DEVICES definition
------------------------------------------------------------------------------*/

/* Alias on existing output devices to adapt for 2 headphones output */
#define OUTPUT_DEVICE_HEADPHONE1 OUTPUT_DEVICE_HEADPHONE
#define OUTPUT_DEVICE_HEADPHONE2 OUTPUT_DEVICE_SPEAKER /* Headphone2 is connected to Speaker output of the wm8994 */
   
/**
  * @}
  */
 
/** @defgroup STM32446E_EVAL_AUDIO_Exported_Variables  STM32446E EVAL AUDIO Exported Variables
  * @{
  */
extern __IO uint16_t AudioInVolume;
 /**
  * @}
  */
   
/** @defgroup STM32446E_EVAL_AUDIO_Exported_Macros  STM32446E EVAL AUDIO Exported Macros
  * @{
  */
#define DMA_MAX(x)           (((x) <= DMA_MAX_SZE)? (x):DMA_MAX_SZE)
/**
  * @}
  */ 

/** @defgroup STM32446E_EVAL_AUDIO_OUT_Exported_Functions  STM32446E EVAL AUDIO OUT Exported Functions
  * @{
  */
uint8_t BSP_AUDIO_OUT_Init_Ext(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq, uint8_t AudioResolution);
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
void    BSP_AUDIO_OUT_DeInit(void);

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
void    BSP_AUDIO_OUT_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t AudioFreq, void *Params);
void    BSP_AUDIO_OUT_MspInit(SAI_HandleTypeDef *hsai, void *Params);
void    BSP_AUDIO_OUT_MspDeInit(SAI_HandleTypeDef *hsai, void *Params);
/**
  * @}
  */ 

/** @defgroup STM32446E_EVAL_AUDIO_IN_Exported_Functions  STM32446E EVAL AUDIO IN Exported Functions
  * @{
  */
uint8_t BSP_AUDIO_IN_Init(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
uint8_t BSP_AUDIO_IN_Record(uint16_t *pData, uint32_t Size);
uint8_t BSP_AUDIO_IN_Stop(void);
uint8_t BSP_AUDIO_IN_Pause(void);
uint8_t BSP_AUDIO_IN_Resume(void);
uint8_t BSP_AUDIO_IN_SetVolume(uint8_t Volume);
void    BSP_AUDIO_IN_DeInit(void);
uint8_t BSP_AUDIO_IN_PDMToPCM(uint16_t* PDMBuf, uint16_t* PCMBuf, uint8_t* temp_pdm, uint16_t pdm_buf_size );
/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function should be implemented by the user application.
   It is called into this driver when the current buffer is filled to prepare the next
   buffer pointer and its size. */
void    BSP_AUDIO_IN_TransferComplete_CallBack(void);
void    BSP_AUDIO_IN_HalfTransfer_CallBack(void);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void    BSP_AUDIO_IN_Error_Callback(void);

/* These function can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void    BSP_AUDIO_IN_ClockConfig(uint32_t AudioFreq, void *Params);
void    BSP_AUDIO_IN_MspInit(I2S_HandleTypeDef *hi2s, void *Params);
void    BSP_AUDIO_IN_MspDeInit(I2S_HandleTypeDef *hi2s, void *Params);
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

#endif /*__STM32446E_EVAL_AUDIO_EX_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
