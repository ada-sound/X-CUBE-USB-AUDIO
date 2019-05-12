/**
  @page AUD_Streaming10 USB Device AUDIO expansion application
  
  @verbatim
  ******************** (C) COPYRIGHT 2019 STMicroelectronics *******************
  * @file    USB_Device/AUD_Streaming10/readme.txt 
  * @author  MCD Application Team 
  * @brief   Description of the USB Device AUDIO streaming application.
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

This application is a part  of the USB audio streaming expansion package using STM32Cube firmware. It describes how to 
use USB device application based on the AUDIO Class implementation of an audio streaming 
(Out: Speaker/Headset , In: mic)  capability on the STM32F446xx devices.

It follows the "Universal Serial Bus Device Class Definition for Audio Devices Release 1.0 March 18, 
1998" defined by the USB Implementers Forum for reprogramming an application through USB-FS-Device. 
Following this specification, it is possible to manage only Full Speed USB mode (High Speed is not supported).
This class is natively supported by most Operating Systems: no need for specific driver setup.

This is an advanced  application on how to use the STM32F446xx USB OTG Device peripheral and SAI peripheral to 
stream audio data from USB Host to the audio codec implemented on the STM32446E-EVAL board.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 168MHz. This value is chosen to drive USB clock from the main PLL.  


Main supported features:
- Playback Audio 
- Recording Audio
- Playback sampling rate: 96Khz (for hi-fi audio),  48KHz and 44.1Khz.
- Playback audio resolution: 24 bits (for hi-fi audio) and 16 bits.
- Playback synchronization using feedback.
- Recording synchronization using add/remove(implicit synchronization).
- Recording sampling rate:  96Khz (for hi-fi audio),  48KHz,  44.1Khz and 16 Khz.

- Recording audio resolution: 24 bits (for hi-fi audio) and 16 bits.
- Both recording and playback support multi-sampling rate: switch between sampling rate on runtime by host request.
- Both recording and playback support volume and  mute control.


      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".


usb_audio_user.h provides USB audio configuration following the application requirements. 
Compilation flag listed bellow allow activation or deactivation of USB audio features      
- USE_USB_FS
- USE_USB_AUDIO_CLASS_10
- USE_USB_AUDIO_PLAYBACK : to use playback
- USE_USB_AUDIO_RECORDING  : to use recording
- USE_AUDIO_MEMS_MIC: to use PDM MEMS MIC(maximal supported frequencies 48KHZ)
- USE_AUDIO_DUMMY_MIC: to use a dummy mic instead of mems mic , it sends zero packets , it emulates mic which supports 96KHZ.
  In order to send a dummy data with 96khz frequency, please make sure that USB_AUDIO_CONFIG_RECORD_USE_FREQ_96_K is set to 1 and
    USB_AUDIO_CONFIG_RECORD_FREQ_MAX is set to 96K instead of 48K
In addition you may change usb_audio_user_cfg.h file for extra option: .
- USB_AUDIO_CONFIG_PLAY_RES_BIT/USB_AUDIO_CONFIG_PLAY_RES_BYTE :    to support 24 or 16 bit audio.
- USE_AUDIO_PLAYBACK_USB_FEEDBACK  : to activate feedback  in playback
- USE_AUDIO_TIMER_VOLUME_CTRL: Handle volume change in playback by  a timer interrupt with low priority, it reduces glitches when changing volume

- USB_AUDIO_CONFIG_RECORD_RES_BIT/USB_AUDIO_CONFIG_RECORD_RES_BYTE  :  to support 24 bit audio in recording 
- USE_AUDIO_RECORDING_USB_IMPLICIT_SYNCHRO: to use implicit synchro in MEMS MIC
- USE_AUDIO_RECORDING_USB_NO_REMOVE: Enhance recorded audio quality, when codec sampling rate is higher than USB rate an extra sample is sent to host (avoiding remove of samples)
 

Four project configurations are provided:
- STM32446E-EVAL_USB_PLAYER: supports playback 24 bit with feedback synchronization.
- STM32446E-EVAL_USB_RECORDER: MEMS RECORDER, which supports audio 16KHZ, 48KHZ. Other frequencies under 48KHZ may be added. 
- STM32446E-EVAL_USB_AUD_DUM_RECORDER: USB Audio dummy recorder which supports audio 16KHZ, 44.1KHZ, 48KHZ and 96KHZ.
- STM32446E-EVAL_USB_AUD_ADVANCED: supports playback and recording both 24 bit, playback with feedback synchronization.    
@note Please make sure that " USB_AUDIO_CONFIG_RECORD_USE_FREQ_96_K" is set to 0 when record and playback are activated simultaneously.         
@par Keywords

Connectivity, USB Device, AUDIO, , Full Speed
       
       
@par Directory contents
  - USB_Device/AUD_Streaming10/Inc/main.h                            main program header file
  - USB_Device/AUD_Streaming10/Inc/stm32f4xx_hal_conf.h              HAL configuration file
  - USB_Device/AUD_Streaming10/Inc/stm32f4xx_it.h                    interrupt handlers header file
  - USB_Device/AUD_Streaming10/Inc/usb_audio_user_cfg.h                  user options for the project
  - USB_Device/AUD_Streaming10/Inc/usb_audio.h                           Macros for the project
  - USB_Device/AUD_Streaming10/Inc/usb_audio_constants.h                  list useful constants
   - USB_Device/AUD_Streaming10/Inc/audio_user_devices.h                  file to modify if user change the speaker or microphone
  - USB_Device/AUD_Streaming10/Inc/usbd_conf.h                       USB device driver Configuration file
  - USB_Device/AUD_Streaming10/Inc/usbd_desc.h                       USB device AUDIO descriptor header file
  - USB_Device/AUD_Streaming10/Src/audio_mic_node.c                  Mic node implementation
  - USB_Device/AUD_Streaming10/Src/audio_speaker_node.c              Speaker node implementation
  - USB_Device/AUD_Streaming10/Src/main.c                            Main program
  - USB_Device/AUD_Streaming10/Src/stm32f4xx_it.c                    Interrupt handlers
  - USB_Device/AUD_Streaming10/Src/system_stm32f4xx.c                STM32F4xx system clock configuration file
  - USB_Device/AUD_Streaming10/Src/usbd_conf.c                       General low level driver configuration
  - USB_Device/AUD_Streaming10/Src/usbd_desc.c                       USB device AUDIO descriptor 
  - USB_Device/AUD_Streaming10/Src/stm32f4xx_hal_msp.c               Timer for playback volume change handling msp init 
  - USB_Device\Extension\Drivers\BSP\Components\wm8994\wm8994_ex.c                      Customized wm8994.c 
  - USB_Device\Extension\Drivers\BSP\Components\wm8994\wm8994_ex.h                      Customized wm8994.h 
  - USB_Device\Extension\Drivers\BSP\STM32446E_EVAL\stm32446e_eval_audio_ex.h           Customized stm32446e_eval_audio.h
  - USB_Device\Extension\Drivers\BSP\STM32446E_EVAL\stm32446e_eval_audio_ex.c           Customized stm32446e_eval_audio.c


@par Hardware and Software environment
                                                                                          
  - This application runs on STM32F446xx devices.
    
  - This application has been tested with STMicroelectronics STM32446E-EVAL
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
	
  - STM32446E-EVAL RevB Set-up
    - Connect the STM32446E-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector CN9 to use USB Full Speed (FS)
	@note Make sure that :
	- jumper JP4 is on FS position (2-3)
	- jumper JP7 is on FS position (2-3)  
    - Use CN22 connector to connect the board to external headset
    - Please ensure that jumpers JP19 and JP20 are fitted in position 2-3 (audio)
    - Please ensure that jumpers JP13 is fitted in position 1-2(recording audio)
    
	@note The compiler optimization level should be set to High/Size giving a good performance.
	
@par How to use it ? 

In order to make the program work, you must do the following :
 1- Open your preferred toolchain
 2- Choose your configuration :
      a- STM32F446E-EVAL_UAC10-PLAY :  to use STM32 board as USB speaker
      b- STM32F446E-EVAL_UAC10-REC  :  to use STM32 board as USB microphone
      c- STM32F446E-EVAL_UAC10-ADV :  :  to use STM32 board as USB speaker and USB microphone
      d- STM32F446E-EVAL_UAC10-DUM :  to use STM32 board as USB microphone , that returns only zero padded packets. however it supports wide range of frequencies(96KHZ).
 3- Set options in usb_audio_user_cfg.h, not mandotary as pre-set option will work for you
 4- Rebuild all files and load your image into target memory
 5- Run the application
 6- Open an audio player application (Audacity for example) and play music or record speech (switch of chosen config)
 *@note: when you connect the USB two times or more with different configs to the Windows 7 host , windows may have conflict with device role.
         As solution , you may change hardcoded PID after changing config.
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
