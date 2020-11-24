//=========================================================
//PatSimulator.c
//Функции и глобальные переменные для генерации испытательных сигналов
//@title       
//@author      Valery Varabyou
//@version     1.0.0.1
//@date        30.12.2019
//=========================================================
//Includes -------------------------------------------------------------------
#include "PatSimulator.h"
#include "main.h"
#include "dac.h"
//#include "dma.h"
//#include "tim.h"
#include "gpio.h"
#include <math.h>
//Defines -------------------------------------------------------------------
//Variables-------------------------------------------------------------------
uint32_t inum = 0;//номер итерации
uint32_t ValDAC1 = 0;
uint32_t ValDAC2 = 0;
uint32_t buf;

uint8_t counterREO = 0;

uint32_t uiCounterBT_PACE = 0;
uint32_t uiCounterBT_NORMAL = 0;
uint32_t uiCounterBT_PILA = 0;
uint32_t uiCounterBT_SINUS = 0;
uint32_t uiCounterBT_MEANDR = 0;

enum TModeWork ModeWork = MODE_NORMAL;

enum TModePQRST WorkModePQRST = MODE_P;
//=========================================================
//Functions
//-----------------------------------------------------------
void PatSimDeviceInit(void)
{
   //HAL_TIM_Base_Start(&htim6);  
   //HAL_TIM_Base_Start(&htim7);   
   //HAL_DAC_Start_DMA (&hdac, DAC_CHANNEL_1, (uint32_t*)buf_tr, SIZE_BUFTR, DAC_ALIGN_12B_R);   
   //HAL_DAC_Start_DMA (&hdac, DAC_CHANNEL_2, (uint32_t*)buf_tr, SIZE_BUFTR, DAC_ALIGN_12B_R);
  
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1); 
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
}

void WorkingWelle(uint32_t a, uint32_t t, uint32_t z, enum TModePQRST m)
{//a - амплитуда, t - длительность, z - уровень нуля, m - следующий шаг
 //Welle, аппроксимация параболой 
  //double buf;
  inum++;   
  
  ValDAC1 = z + (uint32_t)round(a*sin(inum*3.1416/t));
    
  if (inum >= t)
  {
    WorkModePQRST = m;//MODE_PQ;
    inum = 0;
  }  
}

void WorkingStrecke(uint32_t a, uint32_t t, uint32_t z, enum TModePQRST m)
{//a - амплитуда, t - длительность, z - уровень нуля, m - следующий шаг
  inum++;     
  ValDAC1 = a + z;  
  if (inum >= t)
  {
    WorkModePQRST = m;
    inum = 0;
  }  
}

void WorkingStreckeWithPace(uint32_t a, uint32_t t, uint32_t z, uint32_t tp, uint32_t ap, enum TModePQRST m)
{//a - амплитуда, t - длительность, z - уровень нуля, m - следующий шаг
  inum++;     
  ValDAC1 = a + z;  
  if (HAL_GPIO_ReadPin(BT_PACE_GPIO_Port, BT_PACE_Pin) == 0)
    if (inum >= t - tp) ValDAC1 = ap;
  if (inum >= t)
  {
    WorkModePQRST = m;
    inum = 0;
  }  
}

void WorkingQ(uint32_t a, uint32_t t, uint32_t z)
{//a - амплитуда, t - длительность, z - уровень нуля
//падение от уровня PQ до уровня Q  
  inum++;     
  //ValDAC1 = (uint32_t)round(z - inum*a/t);  
  if (z >=  (uint32_t)round(inum*a/t))
    ValDAC1 = (uint32_t)round(z - inum*a/t);  
  else ValDAC1 = 0;
  if (inum >= t)
  {
    WorkModePQRST = MODE_QR;
    inum = 0;
  }
}

void WorkingQR(uint32_t a, uint32_t t, uint32_t z)
{//a - амплитуда, t - длительность, z - уровень нуля
//подъём от уровня Q до уровня R  
  inum++;     
  ValDAC1 = (uint32_t)round(z + inum*a/t);  
  if (inum >= t)
  {
    WorkModePQRST = MODE_RS;
    inum = 0;
  }
}

void WorkingRS(uint32_t a, uint32_t t, uint32_t z)
{//a - амплитуда, t - длительность, z - уровень нуля
//падение от уровня R до уровня S
  inum++;   
  if (z >=  (uint32_t)round(inum*a/t))
    ValDAC1 = (uint32_t)round(z - inum*a/t);  
  else ValDAC1 = 0;
  
  if (inum >= t)
  {
    WorkModePQRST = MODE_S;
    inum = 0;
  }
}

void WorkingS(uint32_t a, uint32_t t, uint32_t z)
{//a - амплитуда, t - длительность, z - уровень нуля
//подъём от уровня S до уровня ST  
  inum++;     
  ValDAC1 = (uint32_t)round(z + inum*a/t);  
  if (inum >= t)
  {
    WorkModePQRST = MODE_ST;
    inum = 0;
  }
}

void WorkNormalCircle(uint32_t d)
{
  uint32_t iii;
   
  switch (WorkModePQRST)
  {
    
  case MODE_P://P Welle
    WorkingWelle(200, MY_TIME_P, MY_ZERO_LEVEL, MODE_PQ);
    break;
    
  case MODE_PQ://PQ Strecke 
    //WorkingStrecke( 0, MY_TIME_PQ, MY_ZERO_LEVEL, MODE_Q);
    WorkingStreckeWithPace(0, MY_TIME_PQ, MY_ZERO_LEVEL, MY_PACE_LENGTH, MAX_DAC_VAL - 500,   MODE_Q);
    break;
    
  case  MODE_Q: //падение от уровня PQ до уровня Q
    WorkingQ(220, MY_TIME_Q, MY_ZERO_LEVEL);
    break;
    
  case  MODE_QR://подъём от уровня Q до уровня R 
    WorkingQR(MAX_DAC_VAL-MY_ZERO_LEVEL+220, MY_TIME_QR, MY_ZERO_LEVEL-220);
    break;
    
  case  MODE_RS://падение от уровня R до уровня S
    WorkingRS(MAX_DAC_VAL - MY_ZERO_LEVEL + MY_AMP_S, MY_TIME_RS, MAX_DAC_VAL-30);
    //WorkingStrecke( 0, MY_TIME_RS, MY_ZERO_LEVEL, MODE_S);
    break;  
    
  case  MODE_S://подъём от уровня S до уровня ST 
    WorkingS(MY_AMP_S, MY_TIME_S, MY_ZERO_LEVEL - MY_AMP_S);
    break; 
    
  case  MODE_ST://ST Strecke
    WorkingStrecke(0, MY_TIME_ST, MY_ZERO_LEVEL, MODE_T);
    //WorkingStrecke(MAX_DAC_VAL-MY_ZERO_LEVEL, MY_TIME_ST, MY_ZERO_LEVEL, MODE_T);//debug
    break;   
    
  case  MODE_T://T Welle
    WorkingWelle(400, MY_TIME_T, MY_ZERO_LEVEL, MODE_U);
    break;  
    
  case  MODE_U://U Welle
    WorkingWelle(100, MY_TIME_U, MY_ZERO_LEVEL, MODE_PAUSE);
    break;  
    
  case  MODE_PAUSE://пауза между двумя циклами
    //WorkingStrecke(0, MY_TIME_PAUSE, MY_ZERO_LEVEL, MODE_P);
    WorkingStreckeWithPace(0, MY_TIME_PAUSE, MY_ZERO_LEVEL, MY_PACE_LENGTH*2, MAX_DAC_VAL,   MODE_P);
    if (WorkModePQRST == MODE_P) 
    {
      HAL_GPIO_TogglePin(LED_D2_GPIO_Port, LED_D2_Pin);
      
      if (counterREO >= MY_MAX_COUNT_REO)
      {//имитируем рео (замыкаем или размыкаем)
        HAL_GPIO_TogglePin(REO_OUT0_GPIO_Port, REO_OUT0_Pin);
        counterREO = 0;
      }//if (counterREO >= MY_MAX_COUNT_REO)
      else counterREO++;
    }//if (WorkModePQRST == MODE_P) 
    break;        
    
  default:
    WorkModePQRST = MODE_P;
    inum = 0;
  };//switch (WorkModePQRST)
  
  
  if (ValDAC1 > MAX_DAC_VAL) ValDAC1 = MAX_DAC_VAL;
  
  //ValDAC2 = (uint32_t)round(MY_ZERO_LEVEL - MY_AMP_S - 200 - (ValDAC1/5));
  if (MY_ZERO_MIDI >= ValDAC1 - MY_ZERO_MIDI)
     ValDAC2 = MY_ZERO_MIDI - (ValDAC1 - MY_ZERO_MIDI);
  else ValDAC2 = 0;

  
  
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, ValDAC1);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, ValDAC2);
  for (iii = 0; iii < d; iii++); 
}


void WorkTriangle(uint32_t m, uint32_t t, uint32_t m0, uint32_t d)
{
  uint32_t iii;
  inum++;
  if (inum > t) inum = 0;

  ValDAC1 = m0 + (uint32_t)round(inum*m/t);
  
  if (ValDAC1 > MAX_DAC_VAL) ValDAC1 = MAX_DAC_VAL;
  
  ValDAC2 = MAX_DAC_VAL - ValDAC1;    
  
  
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, ValDAC1);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, ValDAC2);
  
  for (iii = 0; iii<d; iii++);  
}

void WorkSinus(uint32_t a, uint32_t t, uint32_t m0, uint32_t d)
{
  uint32_t iii;
  inum++;
  if (inum > t) inum = 0;
  
  ValDAC1 = (uint32_t)round(m0 + a*sin(inum*3.1416*2/t));
    
  if (ValDAC1 > MAX_DAC_VAL) ValDAC1 = MAX_DAC_VAL; 
  ValDAC2 = MAX_DAC_VAL - ValDAC1;  
    
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, ValDAC1);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, ValDAC2);
  
  for (iii = 0; iii<d; iii++); 
}

void WorkMeandre(uint32_t a, uint32_t t, uint32_t m0, uint32_t d)
{
  uint32_t iii;
  inum++;
  if (inum > t) inum = 0;
  
  if (inum < t/2) ValDAC1 = m0 + a;
  else ValDAC1 = m0 - a;
    
  if (ValDAC1 > MAX_DAC_VAL) ValDAC1 = MAX_DAC_VAL; 
  ValDAC2 = MAX_DAC_VAL - ValDAC1;  
    
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, ValDAC1);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, ValDAC2);
  
  for (iii = 0; iii<d; iii++); 
}

void WorkMain(void)
{//эта функция вызывается в главном цикле main.c
  
      //
  
   switch (ModeWork)
   {     
   case MODE_NORMAL:
     WorkNormalCircle(0);
     break;
     
   case MODE_SINUS:
     WorkSinus((MAX_DAC_VAL-181)/2, MY_TIME_MIN_INT*8, (MAX_DAC_VAL-1)/2, 0);
     break;
     
   case MODE_PILA:
     WorkTriangle(MAX_DAC_VAL-1, MY_TIME_MIN_INT*16, 0, 0);
     break;
     
   case MODE_MEANDR:
     WorkMeandre((MAX_DAC_VAL-181)/2, MY_TIME_MIN_INT*32, (MAX_DAC_VAL-1)/2, 0);
     break;
   };//switch (ModeWork)
   
   //ON_LED_NORMAL
   
   /*
uint32_t uiCounterBT_PACE = 0;
uint32_t uiCounterBT_NORMAL = 0;
uint32_t uiCounterBT_PILA = 0;
uint32_t uiCounterBT_SINUS = 0;
uint32_t uiCounterBT_MEANDR = 0;
*/
   //кнопки смены режимов
   if (ModeWork != MODE_NORMAL)
   {
     if (HAL_GPIO_ReadPin(BT_NORMAL_GPIO_Port, BT_NORMAL_Pin) == 0)
     {
       uiCounterBT_NORMAL++;
       if (uiCounterBT_NORMAL >= MAX_COUNTER_KEY)
       {
         ModeWork = MODE_NORMAL;
         uiCounterBT_NORMAL = 0;
         ON_LED_NORMAL;
         OFF_LED_SINUS;         
         OFF_LED_PILA;
         OFF_LED_MEANDR;
       };//if
     }//if (HAL_GPIO_ReadPin
     else
     {
      uiCounterBT_NORMAL = 0;
     };//else  
   };//if (ModeWork != MODE_NORMAL)
   
   if (ModeWork != MODE_SINUS)
   {
     if (HAL_GPIO_ReadPin(BT_SINUS_GPIO_Port, BT_SINUS_Pin) == 0)
     {
       uiCounterBT_SINUS++;
       if (uiCounterBT_SINUS >= MAX_COUNTER_KEY)
       {
         ModeWork = MODE_SINUS;
         uiCounterBT_SINUS = 0;
         OFF_LED_NORMAL;
         ON_LED_SINUS;         
         OFF_LED_PILA;
         OFF_LED_MEANDR;
       };//if (uiCounterBT_SINUS >= MAX_COUNTER_KEY)
     }//if (HAL_GPIO_ReadPin(BT_SINUS_GPIO_Port, BT_SINUS_Pin) == 0)
     else
     {
       uiCounterBT_SINUS = 0;
     };//else
   };//if (ModeWork != MODE_SINUS)
   
   if (ModeWork != MODE_PILA)
   {
     if (HAL_GPIO_ReadPin(BT_PILA_GPIO_Port, BT_PILA_Pin) == 0)
     {
       uiCounterBT_PILA++;
       if (uiCounterBT_PILA >= MAX_COUNTER_KEY)
       {
         ModeWork = MODE_PILA;
         uiCounterBT_PILA = 0;
         OFF_LED_NORMAL;
         OFF_LED_SINUS;         
         ON_LED_PILA;
         OFF_LED_MEANDR;
       };//if (uiCounterBT_PILA >= MAX_COUNTER_KEY)
     }//if (HAL_GPIO_ReadPin(BT_PILA_GPIO_Port, BT_PILA_Pin) == 0)
     else
     {
       uiCounterBT_PILA = 0;
     };//else
   };//if (ModeWork != MODE_PILA)
   
   if (ModeWork != MODE_MEANDR)
   {
     if (HAL_GPIO_ReadPin(BT_MEANDR_GPIO_Port, BT_MEANDR_Pin) == 0)
     {
       uiCounterBT_MEANDR++;
       if (uiCounterBT_MEANDR >= MAX_COUNTER_KEY)
       {
         ModeWork = MODE_MEANDR;
         uiCounterBT_MEANDR = 0;
         OFF_LED_NORMAL;
         OFF_LED_SINUS;         
         OFF_LED_PILA;
         ON_LED_MEANDR;
       };//if (uiCounterBT_MEANDR >= MAX_COUNTER_KEY)       
     }//if (HAL_GPIO_ReadPin(BT_MEANDR_GPIO_Port, BT_MEANDR_Pin) == 0)
     else
     {
       uiCounterBT_MEANDR = 0;
     };//else
   };//if (ModeWork != MODE_MEANDR)
   
}
//=========================================================