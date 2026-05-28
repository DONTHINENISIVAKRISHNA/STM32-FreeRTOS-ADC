# STM32-FreeRTOS-ADC
Designed and Implemented a real-time ADC reading system on STM32F103RB using FreeRTOS.
# STM32 FreeRTOS — Module 7B: ADC with FreeRTOS Task

## Overview
Real-time potentiometer reading using ADC1 and FreeRTOS 
on STM32 Nucleo F103RB.

## Hardware
- STM32 Nucleo F103RB (ARM Cortex-M3 @ 72MHz)
- 10K Potentiometer connected to PA0

## What It Does
- ADCTask reads potentiometer every 500ms
- AlertTask monitors threshold every 600ms  
- HIGH ALERT fires when value crosses 73% (raw > 3000)
- Two FreeRTOS tasks sharing data via volatile global variable

## Key Concepts
- FreeRTOS task creation and scheduling
- ADC1 single conversion polling mode
- vTaskDelay vs HAL_Delay
- Shared global variable between tasks
- volatile keyword on ARM Cortex-M3

## Demo Output
![50% level](media/puttywith_50.png)
![High Alert](media/puttywithhighalert.png)
![Back to 0%](media/puttywith0potrotated_back.png)

## Tools
- STM32CubeIDE 2.1.1
- FreeRTOS CMSIS_V1
- PuTTY @ 115200 baud
