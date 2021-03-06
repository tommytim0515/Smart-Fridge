/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tos_k.h"
#include "tos_at.h"
#include "pwm_control.h"
#include "task.h"
#include "ff.h"
#include "lcd_tft.h"
#include "write_bmp.h"
#include "display_content.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
uint8_t recv_buffer[1] = {0};
int name_count = 0;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern SD_HandleTypeDef hsd;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  if (tos_knl_is_running())
  {
    tos_knl_irq_enter();
    tos_tick_handler();
    tos_knl_irq_leave();
  }
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
  tos_knl_irq_enter();
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
  {
    if (flag_take_photo)
    {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
      FATFS fs;
      FRESULT res_sd;
      char name[40];
      sprintf(name, "0:photo_%d.bmp", name_count);
      name_count++;
      res_sd = f_mount(&fs, "0:", 1);
      // LCD_GramScan(3);

      if (res_sd == FR_OK)
      {
        if (screen_shot(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH, name) == 0)
        {
          printf("Photo taken\r\n");
          // flag_take_photo = 0;
        }
      }
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    }

    flag_server_connect = 0;
    printf("Key 1 pressed\r\n");
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    HAL_GPIO_EXTI_Callback(GPIO_PIN_0);
  }
  tos_knl_irq_leave();
  /* USER CODE END EXTI0_IRQn 0 */
  // HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
  tos_knl_irq_enter();
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET)
  {
    flag_take_photo = 0;
    flag_server_connect = 0;
    temp_label_state = 1;
    printf("Key cap pressed\r\n");
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
    HAL_GPIO_EXTI_Callback(GPIO_PIN_1);
  }
  tos_knl_irq_leave();
  /* USER CODE END EXTI1_IRQn 0 */
  // HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
  tos_knl_irq_enter();
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
  tos_knl_irq_leave();
  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
  tos_knl_irq_enter();
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */
  tos_knl_irq_leave();
  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles SDIO global interrupt.
  */
void SDIO_IRQHandler(void)
{
  /* USER CODE BEGIN SDIO_IRQn 0 */
  tos_knl_irq_enter();
  /* USER CODE END SDIO_IRQn 0 */
  HAL_SD_IRQHandler(&hsd);
  /* USER CODE BEGIN SDIO_IRQn 1 */
  tos_knl_irq_leave();
  /* USER CODE END SDIO_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void UART_RxAgain(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
  // use the code from HAL_UART_Receive_IT() to repost interrupt
  huart->pRxBuffPtr = pData;
  huart->RxXferSize = Size;
  huart->RxXferCount = Size;
  huart->ErrorCode = HAL_UART_ERROR_NONE;

  /* Check if a transmit process is ongoing or not */
  if (huart->RxState == HAL_UART_STATE_BUSY_TX)
  {

    huart->RxState = HAL_UART_STATE_BUSY_TX_RX;
  }
  else
  {

    huart->RxState = HAL_UART_STATE_BUSY_RX;
  }

  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(huart, UART_IT_PE);

  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

  /* Process Unlocked */
  //__HAL_UNLOCK(huart);
  /* Enable the UART Data Register not empty Interrupt */

  __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // uint8_t data;
  if (huart->Instance == USART3)
  {
    // HAL_UART_Receive_IT(&huart3, (uint8_t *)recv_buffer, sizeof(recv_buffer));
    UART_RxAgain(&huart3, (uint8_t *)recv_buffer, sizeof(recv_buffer));
    tos_at_uart_input_byte(recv_buffer[0]);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3)
  {
    UART_RxAgain(&huart3, (uint8_t *)recv_buffer, sizeof(recv_buffer));
  }
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
