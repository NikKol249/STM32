//==============================================================================
//PatSimulator.h
//Заголовочный файл для PatSimulator.c
//Структуры и функции для генерации испытательных сигналов
//Предназначен для STM32 c драйвером HAL
//@title       PatSimulator.h
//@author      Valery Varabyou
//@version     1.0.0.1
//@date        26.06.2019
//==============================================================================
// Define to prevent recursive inclusion ---------------------------------------
#ifndef __PATSIMULATOR_H
#define __PATSIMULATOR_H
//Includes ---------------------------------------------------------------------
//#include "stm32f4xx_hal.h"
#include <stdint.h>
//DEFINES-----------------------------------------------------------------------
//режимы работы симулятора
enum TModeWork {
  MODE_NORMAL,
  MODE_SINUS,
  MODE_PILA,
  MODE_MEANDR
};


//этапы генерации нормального цикла PQRSTU
enum TModePQRST {
  //PR Interval---
  MODE_P,//P Welle
  MODE_PQ,//PQ Strecke
  
  //QRS Complex
  MODE_Q,//падение от уровня PQ до уровня Q
  MODE_QR,//подъём от уровня Q до уровня R
  MODE_RS,//падение от уровня R до уровня S
  MODE_S,//подъём от уровня S до уровня ST
  
  MODE_ST,//ST Strecke  
  MODE_T,//T Welle
  MODE_U,//U Welle
  MODE_PAUSE//пауза между двумя циклами
};

#define MAX_DAC_VAL             4095
//define MY_ZERO_LEVEL             ((MAX_DAC_VAL+1)/4) - 1 
//#define MY_ZERO_LEVEL           1536
#define MY_ZERO_LEVEL           2400
#define MY_AMP_S                250
#define MY_ZERO_MIDI            2000
#define MY_MAX_COUNT_REO        3

#define MAX_COUNTER_KEY         100


//временные интервалы (в циклах выполнения)
#define MY_TIME_MIN_INT 2500
//#define MY_TIME_MIN_INT 75
#define MY_TIME_P       MY_TIME_MIN_INT*2   
#define MY_TIME_PQ      MY_TIME_MIN_INT*20
#define MY_TIME_Q       MY_TIME_MIN_INT*6
#define MY_TIME_QR      MY_TIME_MIN_INT*6
#define MY_TIME_RS      MY_TIME_MIN_INT*6
#define MY_TIME_S       MY_TIME_MIN_INT*6
#define MY_TIME_ST      MY_TIME_MIN_INT*20
#define MY_TIME_T       MY_TIME_MIN_INT*2
#define MY_TIME_U       MY_TIME_MIN_INT
#define MY_TIME_PAUSE   MY_TIME_MIN_INT*32

#define MY_PACE_LENGTH  125

//светодиоды
#define ON_LED_NORMAL  HAL_GPIO_WritePin(LED_NORMAL_GPIO_Port, LED_NORMAL_Pin, GPIO_PIN_RESET)
#define OFF_LED_NORMAL  HAL_GPIO_WritePin(LED_NORMAL_GPIO_Port, LED_NORMAL_Pin, GPIO_PIN_SET)
#define ON_LED_SINUS  HAL_GPIO_WritePin(LED_SINUS_GPIO_Port, LED_SINUS_Pin, GPIO_PIN_RESET) 
#define OFF_LED_SINUS  HAL_GPIO_WritePin(LED_SINUS_GPIO_Port, LED_SINUS_Pin, GPIO_PIN_SET)
#define ON_LED_PILA  HAL_GPIO_WritePin(LED_PILA_GPIO_Port, LED_PILA_Pin, GPIO_PIN_RESET)
#define OFF_LED_PILA  HAL_GPIO_WritePin(LED_PILA_GPIO_Port, LED_PILA_Pin, GPIO_PIN_SET)
#define ON_LED_MEANDR  HAL_GPIO_WritePin(LED_MEANDR_GPIO_Port, LED_MEANDR_Pin, GPIO_PIN_RESET)
#define OFF_LED_MEANDR  HAL_GPIO_WritePin(LED_MEANDR_GPIO_Port, LED_MEANDR_Pin, GPIO_PIN_SET)


//EXTERNAL GLOBAL VARIABLES-----------------------------------------------------
//extern uint16_t buf_tr[SIZE_BUFTR];
extern uint32_t inum;//номер итерации
extern uint32_t ValDAC1;
extern uint32_t ValDAC2;
//==============================================================================
//FUNCTIONS---------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
void PatSimDeviceInit(void);//инициализация аппаратной части (зависит от камушка)
void WorkTriangle(uint32_t m, uint32_t t, uint32_t m0, uint32_t d);//односторонняя пила
void WorkSinus(uint32_t a, uint32_t t, uint32_t m0, uint32_t d);//синусоида
void WorkNormalCircle(uint32_t d);//нормальный сердечный цикл
void WorkMain(void);//основная функция для главного цикла
//void WorkNormalCircleDebug(uint32_t d);//debug

#ifdef __cplusplus
}
#endif

#endif  //#ifndef __PATSIMULATOR_H