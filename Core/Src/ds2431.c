#include "ds2431.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_tim.h"
//---------------------------------------------------------------------------

  # define DS2431_FAMILY_ID 0x2D        // ??? ????????? GS2431
# define DS2431_WRITE_SCRATCHPAD 0x0F // ?????? ?????? ? ???????????????? ????????????? ??????
# define DS2431_READ_SCRATCHPAD 0xAA // ?????? ?????? ?? ???????????????? ????????????? ??????
# define DS2431_COPY_SCRATCHPAD 0x55 // ??????????? ?????? ?? ????????????????? ????????????? ?????? ? ???????
 # define DS2431_READ_MEMORY 0xF0 // ?????? ??????

//--------------------------------------------------------------------------------------


# define DS2431_P0_ADDRESS 0x0000       // ?????? ?? 24331 ???????? 0 ????????? ?????
# define DS2431_P1_ADDRESS 0x0020       // ?????? ?? 12431 ???????? 1 ????????? ?????
# define DS2431_P2_ADDRESS 0x0040       // ?????? ?? 24431 ???????? 2 ????????? ?????
# define DS2431_P3_ADDRESS 0x0060       // ?????? ?? 24431 ???????? 3 ????????? ?????
# define DS24311_MAX_ADDRESS 0x007F     // ?? 24431 ???????????? ????????? ?????




//--------------------------------------------------------------------------
#define RomCodeLen 8
u8 RomCode[RomCodeLen], key[RomCodeLen];
u8 Oid[8];
u32 A,B,C,D,E,F,G,H,I,J;


//-----------------------------------------------------------------------------
void GPIO_Conf_in(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
//------------------------------------------------------------------------------------
void GPIO_Conf_out(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD  ;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0;
//  GPIO_Init( GPIOC, &GPIO_InitStructure );
}



//_____________________________________________________________________________
void SetSpeed(int standart)
{
  if(standart)
  {
    A=6;
    B=64;
    C=60;
    D=10;
    E=9;
    F=55;
    G=0;
    H=480;
    I=70;
    J=410;
  }
  else
  {
    A=2;
    B=8;
    C=8;
    D=3;
    E=1;
    F=7;
    G=3;
    H=70;
    I=9;
    J=40;
  
  }
}

//--------------------------------------------------
 void SET_0(void)
{
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
}
//--------------------------------------------------
void SET_1(void)
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
}
//----------------------------------------------------------------------------


int OwReset (void)
{
   
  SetSpeed(1);
  int result=0;
  delay_us(1);
  GPIO_Conf_out();//port shiny 1-WIRE nastraivaetcya kak vyxod                
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
  delay_us(H);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
  delay_us(I); 
  GPIO_Conf_in(); 
   //port shiny 1-WIRE nastraivaetcya kak vxod c vysokim soprotivleniem
 result = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0);
 delay_us(J);
   return result;
}


//----------------------------------------------------------------------------
int ReadBit(void)
{ 
  int result;
  GPIO_Conf_out();
  delay_us(5);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
  delay_us(A);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
  delay_us(E);
  GPIO_Conf_in();
  result = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0)&0x01;
  delay_us(55);//F
  return result;
}
//----------------------------------------------------------------------------

 int Read_Byte(void)
{
 
  int   bitidx, result=0;
  for(bitidx=0; bitidx<8; bitidx++)
  {
  
   result>>=1;
   if(ReadBit())
   
    result|= 0x80;
   
  }
  return result;
}


//------------------------------------------------------------------------.
void WriteBit(int bit)
{   
    SetSpeed(1);
    GPIO_Conf_out();
    if(bit)
      { 
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
        delay_us(A);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
        delay_us(B);
      }
    else
      {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
        delay_us(65);//C
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
        delay_us(D);
      }
}

  void Write_Byte(int data)
{
  int mask;
  for(mask=0;mask<8;mask++)
  {
   // bool check = (data&0x01);
   
    WriteBit(data & 0x01);
//    { 
//        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
//          GPIO_Conf_out();
//        delay_us(60);//C
//          GPIO_Conf_in();
//        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
//        delay_us(10);   
//    }
//    else
//    {
//              HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
//              GPIO_Conf_out();
//              GPIO_Conf_in();
//              HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
//              delay_us(10);   
//    }
    data=data>>1;
  }
  
}
//---------------------------------------------------------

 int OWTouchByte(int data) 
   {
      int loop, result=0;
  
      for (loop = 0; loop < 8; loop++)
        {
          result >>= 1;// shift the result to get it ready for the next bit
          if (data & 0x01)// If sending a '1' then read a bit else write a '0'
            {
              if (ReadBit())
                result |= 0x80;
            }
              else
            WriteBit(0);       
            data >>= 1;// shift the data byte for the next bit
           }   
  return result;
}

//--------------------------------------------------------------------
void OWBlock(unsigned char *data, int data_len)
{
int loop; 
for (loop = 0; loop < data_len; loop++)
    {
      data[loop] = OWTouchByte(data[loop]);
    }
}

u8 dscrcCheck(u8* p,u8 len)
{
    uint8_t bit0,cbit,i,j,byte,temp;
  
    temp=0;
for(j=0;j<len;j++)
 {
   byte=p[j];
    for(i=0;i<8;i++)
      {
        cbit = temp & 0x01;
        bit0 = byte & 0x01;
        temp=temp>>1;    
  if( (cbit^bit0) ) temp^=0x8c;
      byte>>=1;
      }
   }
return temp;
}

//-------------------------------------------------------------------------
u8 read_2431(void)
{
int i;
//uchar key[]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47};  


SetSpeed(1);
// select the device
if (OwReset()!=0) // Reset the 1-Wire bus
return 1; // Return if no devices found
Write_Byte(0x33); // Send Read ROM command to select single device
delay_us(60);// read the page data
for (i = 0; i < 8; i++)
RomCode[i] = Read_Byte();
//return dscrcCheck(RomCode,8);
if ( dscrcCheck(RomCode,8)!=RomCode[8] )
  {
    return 0;
  }
  else
  {
    return 1;
  }

}
//---------------------------------------------------------------------------
//u8 new_id(uchar *key)
//{
//  
//Write_Byte(0xCC);
//OwReset();
//Write_Byte(0xD1);
//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
//GPIO_Conf_out();
//delay_us(60);
//GPIO_Conf_in();
//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
//delay_us(10);
//Write_Byte(0xCC);
//OwReset();
//Write_Byte(0xD5);
//for(int i=0;i<8;i++)
//{
//    Write_Byte(key[i]);
//}
//OwReset();
//Write_Byte(0xD1);
//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
//GPIO_Conf_out();
//delay_us(10);
//GPIO_Conf_in();
//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
//delay_us(10);
//}




//------------------------------------------------------------------------
void read_2431_pagedata(unsigned char page, unsigned char *page_data)
{
unsigned char i; 
SetSpeed(1);
// select1 the device

//GPIO_Conf_out();
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
//HAL_Delay(5);

if (OwReset()) // Reset the 1-Wire bus
  {
    return  ; // Return if no devices found
  }

Write_Byte(0xCC); // Send Skip ROM command to select single device
Write_Byte(0xF0); // Read Authentication command
Write_Byte((page << 5)&0xFF);
Write_Byte(0); // TA2 (always zero for DS2432)  
delay_us(100); 
//Write_Byte(0xFF);
for (i = 0; i <8; i++)// read the page data
page_data[i] = Read_Byte();

  
if ( OwReset()) // Reset the 1-Wire bus
  {
    return ; // Return if no devices found
  }
}

////---------------------------------------------------------------------------
char write_2431_pagedata(unsigned char page, unsigned char *page_data)
{
unsigned char i,TA1,TA2,E_S;
unsigned char rstatus[8];
int status =0;
SetSpeed(1);
//uchar Oid[10];
//GPIO_Conf_out();
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
//HAL_Delay(5);


if (OwReset()) // Reset the 1-Wire bus
{
return status; // Return if no devices found
}

Write_Byte(0xCC); // Send Skip ROM command to select single device
Write_Byte(0x0F); // Read Authentication command
//Write_Byte((page << 5) & 0xFF);

Write_Byte(0x20);
Write_Byte(0x00);  
      
// read the page data
for (i = 0; i < 8; i++)
{
Write_Byte(page_data[i]);
}
//crc
rstatus[0] = Read_Byte();
rstatus[1] = Read_Byte();

//delay_us(200);// select the device
if (OwReset()) // Reset the 1-Wire bus
    {
      return 0; // Return if no devices found
    }
Write_Byte(0xCC); // Send Skip ROM command to select single device
Write_Byte(0xAA);
TA1 = Read_Byte();
TA2 = Read_Byte();
E_S = Read_Byte();

delay_us(10);
for(i=0;i<8;i++)
  { 
// rdtmp = OWReadByte();
  Oid[i]=Read_Byte();
  }
//crc
rstatus[2] = Read_Byte();
//status=rstatus[2];
rstatus[3] = Read_Byte();
//status=rstatus[3];
//
//return status;
// select the device
delay_us(200);
if (OwReset()) // Reset the 1-Wire bus
  {
    return 0  ; // Return if no devices found
   }
Write_Byte(0xCC); // Send Skip ROM command to select single device
Write_Byte(0x55); // Read Authentication command
//Write_Byte((page << 5) & 0xFF); // TA1
//Write_Byte(page << 5) ;
Write_Byte(0x20);
Write_Byte(0x00); // TA2 (always zero for DS2432)
Write_Byte(0x07);
delay_us(13000); // ???????? ?????
//Wait tPROGMAX for the copy function to complete
rstatus[4] = Read_Byte();  
//status=rstatus[4];
//
//return status;
Oid[2]=TA1;
Oid[3]=TA2;
Oid[0]=E_S;
Oid[4]=rstatus[4];

//return status;

}

