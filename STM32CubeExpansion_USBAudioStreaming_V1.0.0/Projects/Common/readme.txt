/**
  @page USB Device AUDIO expansion extended files
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    USB_Device/Extension/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Extension folder content.
  *******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                               www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This Folder Contains extended files from middleware &  streaming library that suppports playback & recording.
A suffix "_ex" is added to the name of customized files.
       
@par Directory contents
  - Common\Middlewares\ST\STM32_USB_Device_Library\Class\AUDIO_10\Inc\usbd_audio.h New implementation of USB audio class 1.0
  - Common\Middlewares\ST\STM32_USB_Device_Library\Class\AUDIO_10\Src\usbd_audio.c New implementation of USB audio class 1.0
  - Common\Middlewares\ST\STM32_USB_Device_Library\Core\Src\usbd_core_ex.c Customized usbd_core.c
  - Common\Streaming\inc\audio_node.h                      generic node structures
  - Common\Streaming\inc\audio_usb_nodes.h                 USB nodes header 
  - Common\Streaming\inc\audio_speaker_node.h              speaker node header
  - Common\Streaming\inc\audio_mic_node.h                  microphone node header
  - Common\Streaming\inc\audio_sessions_usb.h              USB sessions header
  - Common\Streaming\inc\usbd_audio_if.h                   USBD Audio interface header file
  - Common\Streaming\inc\audio_user_devices_template.h     audio specific devices node header template
  - Common\Streaming\src\audio_usb_nodes.c                 USB nodes implementation
  - Common\Streaming\Src\audio_dummymic_node.c             Dummy MIC implementation
  - Common\Streaming\Src\audio_dummyspeaker_node.c             Dummy SPEAKER implementation
  - Common\Streaming\src\audio_usb_playback_session.c      playback session implementation
  - Common\Streaming\src\audio_usb_recording_session.c     recording session implementation
  - Common\Streaming\src\usbd_audio_10_config_descriptors.c Configuration descriptor file.
  - Common\Streaming\Src\usbd_audio_if.c                   USBD Audio class interface
  - Common\Streaming\Src\audio_speaker_node_template.c     Speaker node implementation template
  - Common\Streaming\Src\audio_mic_node_template.c         Micnode implementation template
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
