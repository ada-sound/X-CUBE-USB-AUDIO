/**
  ******************************************************************************
  * @file    audio_user_devices.h
  * @author  MCD Application Team 
  * @brief   Abstraction of boared specific devices.
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
#ifndef __AUDIO_USER_DEVICES_H
#define __AUDIO_USER_DEVICES_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32446e_eval_audio_ex.h"
#include "usb_audio.h"
   
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#if  USE_USB_AUDIO_RECORDING
#if USE_AUDIO_MEMS_MIC
typedef struct
{
  uint16_t pdm_buff[PDM_BUF_SIZE(USB_AUDIO_CONFIG_RECORD_FREQ_MAX)];
  uint8_t pdm_tmp_buff[PDM_BUF_SIZE(USB_AUDIO_CONFIG_RECORD_FREQ_MAX)];
  uint16_t writing_step;
  uint32_t pdm_packet_size;
  uint8_t bos; /* begin of play */
  uint8_t cmd; /* cmd to execute in interruption routine */
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
  uint32_t dma_remaining; /* The number of remaining bytes in the current DMA Stream transfer*/
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
}AUDIO_MicrophoneSpecificParams_t;
#endif /* USE_AUDIO_MEMS_MIC */
#define AUDIO_USER_MicInit AUDIO_MEMS_MicInit
#endif /* USE_USB_AUDIO_RECORDING */
#if USE_USB_AUDIO_PLAYBACK
#if !USE_AUDIO_SPEAKER_DUMMY
typedef struct
{
  uint16_t               injection_size;         /* the nominal size of the unit packet sent using DMA to SAI*/
  uint8_t*               data;                   /* a pointer to data, which is going to transmit using DMA to SAI */
  uint16_t               data_size;              /* a size of data, which is going to transmit using DMA to SAI */
  uint8_t*               alt_buffer;             /* an alternative buffer used  when underrun is produced(no enough data to inject) or when padding should be added*/
  uint16_t               alt_buf_half_size;      /* the half size of the alternative buffer*/
  uint8_t                double_buff;            /* when the padding is needed the double buffering are required. It means that the alt_buff will contain two packet*/
  uint8_t                offset ;                /* a binary flag. used to indicate if next packet is in the first half of alternate buffer or in the second half*/
  __IO uint8_t           cmd;                    /* this field contains commands to execute within next transfer complete call(or in next Volume change interrupt) */
  uint16_t               dma_remaining;  /* used for synchronization, it helps to provide the counter of played samples */
} AUDIO_SpeakerSpecificParms_t;
#endif /*USE_AUDIO_SPEAKER_DUMMY*/
#endif /* USE_USB_AUDIO_PLAYBACK */
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
}
#endif
#endif  /* __AUDIO_USER_DEVICES_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
