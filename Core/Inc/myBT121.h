//==============================================================================
//myBT121.h
//Заголовочный файл для myBT12.c
//Макросы, структуры и функции контроля и управления BT121
//Предназначен для STM32 c драйвером HAL
//@title       Monit BT121
//@author      Samailovich Yaraslau
//@version     1.0.0.1
//@date        15.05.2019
//==============================================================================
// Define to prevent recursive inclusion ---------------------------------------
#ifndef __MYBT121_H
#define __MYBT121_H
//Includes ---------------------------------------------------------------------
#include "stm32f4xx_hal.h"
#include "main.h"
//DEFINES-----------------------------------------------------------------------
#define DOWN_BT_BOOT    HAL_GPIO_WritePin(BT_BOOT_GPIO_Port, BT_BOOT_Pin, GPIO_PIN_RESET)
#define UP_BT_BOOT      HAL_GPIO_WritePin(BT_BOOT_GPIO_Port, BT_BOOT_Pin, GPIO_PIN_SET)

#define DOWN_BT_ENABLE    HAL_GPIO_WritePin(BT_ENABLE_GPIO_Port, BT_ENABLE_Pin, GPIO_PIN_RESET)
#define UP_BT_ENABLE    HAL_GPIO_WritePin(BT_ENABLE_GPIO_Port, BT_ENABLE_Pin, GPIO_PIN_SET) 

#define DOWN_BT_LSTAT    HAL_GPIO_WritePin(BT_LSTAT_GPIO_Port, BT_LSTAT_Pin, GPIO_PIN_RESET)
#define UP_BT_LSTAT    HAL_GPIO_WritePin(BT_LSTAT_GPIO_Port, BT_LSTAT_Pin, GPIO_PIN_SET)

#define DOWN_BT_RST     HAL_GPIO_WritePin(BT_RST_GPIO_Port, BT_RST_Pin, GPIO_PIN_RESET)
#define UP_BT_RST       HAL_GPIO_WritePin(BT_RST_GPIO_Port, BT_RST_Pin, GPIO_PIN_SET)
//void MyInitBT121(void);
//FUNCTIONS---------------------------------------------------------------------
void resetBT121(void);
#ifdef __cplusplus
extern "C" {
#endif
void MyInitBT121(void);//инициализация BT121 для нормальной работы
#ifdef __cplusplus
}
#endif

#endif  //#ifndef __MYBT212_H
