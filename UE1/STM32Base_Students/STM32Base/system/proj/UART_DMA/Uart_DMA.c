/**
  ******************************************************************************
  * @file    Uart_DMA.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    18-November-2025
  * @brief   Project File for the Exercise 7 in MPT3
  ******************************************************************************
*/
 
#include "board_uart_dma.h"
#include "board_led.h"
#include "Uart_DMA.h"
#include "sysdelay.h"
#include "stm32f0xx_dma.h"


// Defines

#define ms *1
#define LOOP_DELAY 1000 ms


/**
 * @brief Interrupt Handler for DMA Channel 7 (UART3 TX)
 * 
 */
void DMA1_Channel4_5_6_7_IRQHandler(void) {
  // Check if Transfer Complete Interrupt Flag is set for Channel 7 (UART3 TX)
  if(DMA1->ISR & DMA_ISR_TCIF7){
    // Clear the Interrupt Flag
    DMA1->IFCR = DMA_IFCR_CTCIF7;
    
    // Disable the DMA Channel after Transfer is complete
    DMA_Cmd(DMA1_Channel7, DISABLE);

    LED_On(LED5); // Indicate Transfer Complete with LED5
    LED_Toggle(LED4); // Toggle LED4 to show activity
  }
}

// Global Buffer to prevent out of scope issues
// cannot be const otherwise it is stored in flash memory (excersize requirement: buffer in RAM)
static char * const helloStr = "MPT3 Hello World from UART DMA VCOM APUS Board \n";

/**
 * @brief Test Programm for UART DMA VCOM
 * 
 */
void main_Uart_DMA_VCOM(){
  // Initialize Peripheral
  UART_DMA_VCOM_Init();
  LED_Initialize();
  
  while(1){

    // Transmit String via DMA
    VCOM_DMA_TX_String(helloStr);

    // delay for edge detection -> to supress button bouncing
    SysDelay_Delay(LOOP_DELAY);
  }
} 