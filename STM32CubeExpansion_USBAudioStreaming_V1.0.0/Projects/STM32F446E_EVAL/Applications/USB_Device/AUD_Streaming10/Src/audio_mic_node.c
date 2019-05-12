/**
  ******************************************************************************
  * @file    audio_mic_node.c
  * @author  MCD Application Team 
  * @brief   mic node implementation.
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
#include <string.h>
#include "usbd_audio.h"
#include "audio_mic_node.h"
#include "usb_audio.h"

/* Private defines -----------------------------------------------------------*/
#define MEMS_VOLUME_MIC_RES_DB_256     256 /* 1 db 1 * 256 = 256*/ 
#define MEMS_VOLUME_MIC_MAX_DB_256     8192 /* 32db == 32*256 = 8192*/
#define MEMS_VOLUME_MIC_MIN_DB_256     -8192 /* -32db == -32*256 = -8192*/
#define MIC_CMD_STOP  1
#define MIC_CMD_EXIT  2
#define MIC_CMD_CHANGE_FREQUENCE  4

 /* #define DEBUG_MIC_NODE 1 define if debug required*/
#ifdef DEBUG_MIC_NODE
#define MIC_DEBUG_BUFFER_SIZE 1000
#endif /* DEBUG_MIC_NODE */

/* Private macros -------------------------------------------------------------*/
#define VOLUME_DB_256_TO_PERCENT(volume_db_256) ((uint8_t)((((int)(volume_db_256) - MEMS_VOLUME_MIC_MIN_DB_256)*100)/\
                                                          (MEMS_VOLUME_MIC_MAX_DB_256 - MEMS_VOLUME_MIC_MIN_DB_256)))

/* Private typedef -----------------------------------------------------------*/
#ifdef DEBUG_MIC_NODE
typedef struct
{
  uint32_t time;
  uint16_t write;
  uint16_t read;
} AUDIO_MicDebugStats;
#endif /* DEBUG_MIC_NODE*/

/* externals variables ---------------------------------------------------------*/
#ifdef DEBUG_MIC_NODE
extern __IO uint32_t uwTick;
#endif /* DEBUG_MIC_NODE*/

#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
extern I2S_HandleTypeDef haudio_in_i2s;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/

/* Private function prototypes -----------------------------------------------*/
/* list of Mic Callbacks */
static int8_t  AUDIO_MicDeInit(uint32_t node_handle);
static int8_t  AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer,  uint32_t node_handle);
static int8_t  AUDIO_MicStop( uint32_t node_handle);
static int8_t  AUDIO_MicChangeFrequency( uint32_t node_handle);
static int8_t  AUDIO_MicMute(uint16_t channel_number,  uint8_t mute, uint32_t node_handle);
static int8_t  AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256, uint32_t node_handle);
static int8_t  AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle);
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
static int8_t  AUDIO_MicStartReadCount( uint32_t node_handle);
static uint16_t AUDIO_MicGetLastReadCount( uint32_t node_handle);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
static void AUDIO_MicFillDataToBuffer(uint32_t pdm_offset);
#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)
static void AUDIO_DoPadding(uint8_t* src,  uint8_t *dest,  int size);
#endif /* ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)  */
 
/* Private variables ---------------------------------------------------------*/ 
static AUDIO_MicNode_t *AUDIO_MicHandler = 0;
#ifdef DEBUG_MIC_NODE
static AUDIO_MicDebugStats mic_stats[MIC_DEBUG_BUFFER_SIZE];
static  int AUDIO_MicStatsCount = 0;
static  int AUDIO_MicStatsCounter = 0;
#endif /* DEBUG_MIC_NODE*/

/* exported functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_MicInit
  *         Initializes the audio mic node 
  * @param  audio_description:   audio parameters
  * @param  session_handle:     session handle
  * @param  node_handle:        mic node handle must be allocated
  * @retval 0 if no error
  */
 int8_t  AUDIO_MEMS_MicInit(AUDIO_Description_t* audio_description,  AUDIO_Session_t* session_handle,
                       uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;

  mic   = (AUDIO_MicNode_t*)node_handle;
  memset(mic, 0, sizeof(AUDIO_MicNode_t));
  mic->node.type                = AUDIO_INPUT;
  mic->node.state               = AUDIO_NODE_INITIALIZED;
  mic->node.session_handle      = session_handle;
  mic->node.audio_description   = audio_description;
  mic->MicDeInit                = AUDIO_MicDeInit;
  mic->MicStart                 = AUDIO_MicStart;
  mic->MicStop                  = AUDIO_MicStop;
  mic->MicChangeFrequency       = AUDIO_MicChangeFrequency;
  mic->MicMute                  = AUDIO_MicMute;
  mic->MicSetVolume             = AUDIO_MicSetVolume;
  mic->MicGetVolumeDefaultsValues = AUDIO_MicGetVolumeDefaultsValues;
#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
  mic->MicStartReadCount        = AUDIO_MicStartReadCount;
  mic->MicGetReadCount          = AUDIO_MicGetLastReadCount;
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
  mic->volume                           = VOLUME_DB_256_TO_PERCENT(audio_description->audio_volume_db_256);
  mic->packet_length                    = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(audio_description);
  mic->specific.pdm_packet_size    = PDM_BUF_SIZE(audio_description->frequency);
  BSP_AUDIO_IN_Init((audio_description->frequency/1000)*1000, /* PDM Lib doesn't support 44100 freq */
                    audio_description->resolution,
                    audio_description->channels_count);

  AUDIO_MicHandler = mic;
  BSP_AUDIO_IN_Record((uint16_t*)&mic->specific.pdm_buff[0], mic->specific.pdm_packet_size); /* x2 for double buffering */
  return 0;
}
/**
  * @brief  BSP_AUDIO_IN_HalfTransfer_CallBack
            Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  /* PDM to PCM data convert */
  if((AUDIO_MicHandler)&&(AUDIO_MicHandler->node.state==AUDIO_NODE_STARTED))
  {
      AUDIO_MicFillDataToBuffer(0);
  }
}

/**
  * @brief  BSP_AUDIO_IN_TransferComplete_CallBack
  *         Manages the DMA Transfer complete interrupt    
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  /* PDM to PCM data convert */
  if(AUDIO_MicHandler)
  {
      AUDIO_MicFillDataToBuffer((AUDIO_MicHandler->specific.pdm_packet_size>>1));
  }
}

/* private functions ---------------------------------------------------------*/
/**
  * @brief  AUDIO_MicDeInit
  *         De-Initializes the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicDeInit(uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  
  mic = (AUDIO_MicNode_t*)node_handle;
  
  if(mic->node.state != AUDIO_NODE_OFF)
  {
    if(mic->node.state == AUDIO_NODE_STARTED)
    {
      AUDIO_MicStop(node_handle);
    }
    BSP_AUDIO_IN_Stop();
    BSP_AUDIO_IN_DeInit();  
    mic->node.state = AUDIO_NODE_OFF;
  }
  
    return 0;
}

/**
  * @brief  AUDIO_MicStart
  *         Start the audio mic node 
  * @param  buffer:      Audio data buffer       
  * @param  node_handle: mic node handle must be initialized
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicStart(AUDIO_CircularBuffer_t* buffer ,  uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  mic=(AUDIO_MicNode_t*)node_handle;

  if(mic->node.state != AUDIO_NODE_STARTED)
  {
    mic->node.state = AUDIO_NODE_STARTED;
    mic->buf        = buffer;
  }
    return 0;
}

/**
  * @brief  AUDIO_MicStop
  *         stop the audio mic node 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicStop( uint32_t node_handle)
{
    
  AUDIO_MicNode_t* mic;
  mic = (AUDIO_MicNode_t*)node_handle;

  if(mic->node.state == AUDIO_NODE_STARTED)
  {
    mic->node.state = AUDIO_NODE_STOPPED;
  }
    return 0;
}

/**
  * @brief  AUDIO_MicChangeFrequency
  *         change mic frequency 
  * @param  node_handle: mic node handle must be initialized
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicChangeFrequency( uint32_t node_handle)
{
    
  AUDIO_MicNode_t* mic;

  mic = (AUDIO_MicNode_t*)node_handle;
  mic->specific.cmd|= MIC_CMD_CHANGE_FREQUENCE;
  
    return 0;
}

/**
  * @brief  AUDIO_MicMute
  *         mute  mic 
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicMute(uint16_t channel_number,  uint8_t mute , uint32_t node_handle)
{
  uint8_t volume;
  
  volume=(mute)?0:((AUDIO_MicNode_t*)node_handle)->volume;
  BSP_AUDIO_IN_SetVolume(volume);
  
  return 0;
}

/**
  * @brief  AUDIO_MicSetVolume
  *         set  mic volume 
  * @param  channel_number : which channel to set the volume (not used currently)
  * @param  volume_db_256  : description is missing
  * @param  node_handle      mic  node handle
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicSetVolume( uint16_t channel_number,  int volume_db_256 ,  uint32_t node_handle)
{
  ((AUDIO_MicNode_t*)node_handle)->volume = VOLUME_DB_256_TO_PERCENT(volume_db_256);
  BSP_AUDIO_IN_SetVolume(((AUDIO_MicNode_t*)node_handle)->volume);
  
  return 0;
}
/**
  * @brief  AUDIO_MicGetVolumeDefaultsValues
  *         get  mic volume max, min & resolution value  in db
  * @param  vol_max            
  * @param  vol_min            
  * @param  vol_res             
  * @param  node_handle         
  * @retval 0 if no error
  */
static int8_t  AUDIO_MicGetVolumeDefaultsValues( int* vol_max, int* vol_min, int* vol_res, uint32_t node_handle)
{
  *vol_max = MEMS_VOLUME_MIC_MAX_DB_256;
  *vol_min = MEMS_VOLUME_MIC_MIN_DB_256;
  *vol_res = MEMS_VOLUME_MIC_RES_DB_256;
  return 0;
}


/**
  * @brief  AUDIO_MicFillDataToBuffer
   *        convert data to pdm then check if padding needed
  * @param  pdm_offset        
  * @retval None
  */

static void AUDIO_MicFillDataToBuffer(uint32_t pdm_offset)
{
  uint32_t buffer_filled_size ;
#ifdef DEBUG_MIC_NODE
  uint32_t counter;
  mic_stats[AUDIO_MicStatsCount].time = uwTick;
  counter = ++AUDIO_MicStatsCounter;
#endif /*DEBUG_MIC_NODE*/

  if(AUDIO_MicHandler->specific.cmd & MIC_CMD_CHANGE_FREQUENCE)
  {  /* first stop the Microphone */
     BSP_AUDIO_IN_Stop();
     BSP_AUDIO_IN_DeInit();
     /* recalculate the packet length*/
     AUDIO_MicHandler->packet_length = AUDIO_MS_PACKET_SIZE_FROM_AUD_DESC(AUDIO_MicHandler->node.audio_description);
     AUDIO_MicHandler->specific.pdm_packet_size = PDM_BUF_SIZE(AUDIO_MicHandler->node.audio_description->frequency);
     /* Start the Microphone*/
     BSP_AUDIO_IN_Init(AUDIO_MicHandler->node.audio_description->frequency,
                    AUDIO_MicHandler->node.audio_description->resolution,
                    AUDIO_MicHandler->node.audio_description->channels_count);
     BSP_AUDIO_IN_Record((uint16_t*)&AUDIO_MicHandler->specific.pdm_buff[0], AUDIO_MicHandler->specific.pdm_packet_size); /* x2 for double buffering */
     /* remove the change frequency command */
     AUDIO_MicHandler->specific.cmd &= ~MIC_CMD_CHANGE_FREQUENCE;
  }
  else
  {
    if(AUDIO_MicHandler->node.state == AUDIO_NODE_STARTED)
    {
      
    buffer_filled_size = AUDIO_BUFFER_FREE_SIZE(AUDIO_MicHandler->buf);
    if(buffer_filled_size<=AUDIO_MicHandler->packet_length)
    {
      AUDIO_MicHandler->node.session_handle->SessionCallback(AUDIO_OVERRUN, (AUDIO_Node_t*)AUDIO_MicHandler,
                                                        AUDIO_MicHandler->node.session_handle);
    }
    BSP_AUDIO_IN_PDMToPCM((uint16_t*)&AUDIO_MicHandler->specific.pdm_buff[pdm_offset], 
                        (uint16_t*)(AUDIO_MicHandler->buf->data+AUDIO_MicHandler->buf->wr_ptr), AUDIO_MicHandler->specific.pdm_tmp_buff, AUDIO_MicHandler->specific.pdm_packet_size);
  /* to change to support other resolution */
  /* check for overflow */
#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)
    AUDIO_DoPadding(AUDIO_MicHandler->buf->data+AUDIO_MicHandler->buf->wr_ptr,
                          AUDIO_MicHandler->buf->data+AUDIO_MicHandler->buf->wr_ptr, AUDIO_MicHandler->packet_length);
    AUDIO_MicHandler->buf->wr_ptr+=AUDIO_MicHandler->packet_length;
#else  /* #if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) == 16)*/
    AUDIO_MicHandler->buf->wr_ptr += AUDIO_MicHandler->packet_length;
#endif /* #if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16) */
   #if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO 
  AUDIO_MicHandler->node.session_handle->SessionCallback(AUDIO_PACKET_RECEIVED, (AUDIO_Node_t*)AUDIO_MicHandler,
                                                        AUDIO_MicHandler->node.session_handle);
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
    if(AUDIO_MicHandler->buf->wr_ptr == AUDIO_MicHandler->buf->size)
    {
      AUDIO_MicHandler->buf->wr_ptr = 0;
    }
#ifdef DEBUG_MIC_NODE
    if( counter !=AUDIO_MicStatsCounter)
    {
      Error_Handler();
    }

    mic_stats[AUDIO_MicStatsCount].read = AUDIO_MicHandler->buf->rd_ptr;
    mic_stats[AUDIO_MicStatsCount].write = AUDIO_MicHandler->buf->wr_ptr;
    
    if(++AUDIO_MicStatsCount == MIC_DEBUG_BUFFER_SIZE)
    {
        AUDIO_MicStatsCount = 0;
    }
#endif /*DEBUG_MIC_NODE*/
    }
  }
  
}
/**
  * @brief  AUDIO_DoPadding
  *        Pad 24bit  sample to 32 sample by adding zeros .
  * @param  src(IN)                
  * @param  dest(OUT)               
  * @param  size(IN)                
  * @retval None
  */
#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)
 static void AUDIO_DoPadding(uint8_t* src,  uint8_t *dest ,  int size)
 {
   int j = size-1;
   
   for(int i = (size*2)/3-1;i>=0;)
   {
     dest[j--] = src[i--];
     dest[j--] = src[i--];
     dest[j--] = 0;
   }
 }
#endif /* #if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)*/

#if USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO
/**
  * @brief  AUDIO_MicStartReadCount
  *         Start a count to compute read bytes from mic each ms 
  * @param  node_handle: mic node handle must be started
  * @retval  : 0 if no error
  */
static int8_t  AUDIO_MicStartReadCount( uint32_t node_handle)
{
    AUDIO_MicNode_t* mic;
  
    mic = (AUDIO_MicNode_t*)node_handle;
  
    if(mic->node.state == AUDIO_NODE_STARTED)
    {
           /* read remained value in dma buffer */
      mic->specific.dma_remaining = (mic->specific.pdm_packet_size - __HAL_DMA_GET_COUNTER(haudio_in_i2s.hdmarx))>>1;
#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)
       mic->specific.dma_remaining|=0x1; 
#endif
      return 0;
    }
   return -1;     
} 
/**
  * @brief  AUDIO_MicGetLastReadCount
  *         read the number of bytes that was read in the last 1ms 
  * @param  node_handle(IN): mic node handle must be started
  * @retval number of read bytes, 0 if an error
  */    
static uint16_t  AUDIO_MicGetLastReadCount( uint32_t node_handle)
{
  AUDIO_MicNode_t* mic;
  uint32_t cur_waiting_bytes, read_bytes;
  mic = (AUDIO_MicNode_t*)node_handle;
  
  if(mic->node.state == AUDIO_NODE_STARTED)
  {
         /* read remained value in dma buffer */
    cur_waiting_bytes = (mic->specific.pdm_packet_size - __HAL_DMA_GET_COUNTER(haudio_in_i2s.hdmarx))>>1;
#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)
       cur_waiting_bytes|=0x1; 
#endif /* #if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16) */
    read_bytes = (cur_waiting_bytes> mic->specific.dma_remaining)?cur_waiting_bytes -  mic->specific.dma_remaining:
                 (((mic->specific.pdm_packet_size)>>1) - mic->specific.dma_remaining)+cur_waiting_bytes ;
#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)
    read_bytes >>= 1;
    read_bytes *= 3;
#endif /*#if ((USB_AUDIO_CONFIG_RECORD_RES_BIT) != 16)*/
    mic->specific.dma_remaining = cur_waiting_bytes;
    
    return read_bytes;
  }
    return 0;
}
#endif /* USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
