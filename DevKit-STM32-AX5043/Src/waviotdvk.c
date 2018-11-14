#include "waviotdvk.h"
#include "stdbool.h"

#define PRESS_SB 5

static uint8_t CntPressSB1 = 0;
static uint8_t CntPressSB2 = 0;
static uint8_t CntPressSB3 = 0;
static uint8_t CntPressSB4 = 0;
static uint8_t button_events = 0;
static uint8_t button_state_change = 0;
//struct wtimer_desc buttons_desc;

bool GetButton1()
{
  if(button_events & SW1) 
  {
    //button_events &= ~SW1;
    //button_events = 0;
    return true;
  }
  else return false;
}

bool GetButton2()
{
  if(button_events & SW2)
  {
    //button_events &= ~SW2;
    //button_events = 0;
    return true;
  }
  else return false;
}

bool GetButton3()
{
  if(button_events & SW3)
  {
    //button_events &= ~SW3;
    //button_events = 0;
    return true;
  }
  else return false;
}

bool GetButton4()
{
  if(button_events & SW4)
  {
    //button_events &= ~SW4;
    //button_events = 0;
    return true;
  }
  else return false;
}

bool GetButtonStateChange()
{
  button_state_change = button_events;                                  //��������� ���������� ������������ ������
  
  if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(SB1_GPIO_Port, SB1_Pin))       //���� ������ SB1 ���� ������
  {
    CntPressSB1++;		                                        //����������� ������� ������ �������� ������� ������
    HAL_Delay(10);						        //�������� 10 ��
    
    if(CntPressSB1>PRESS_SB)                                            //���� ������ 5 ������
    {
      button_events |= SW1;                                             //������������� ���� ������� ������
    }
  }
  else
  {
    button_events &= ~SW1;                                              //���������� ���� ������� ������
    CntPressSB1=0;		                                        //�������� �������
  }

  if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(SB2_GPIO_Port, SB2_Pin))       //���� ������ SB2 ���� ������
  {
    CntPressSB2++;		                                        //����������� ������� ������ �������� ������� ������
    HAL_Delay(10);						        //�������� 10 ��
    
    if(CntPressSB2>PRESS_SB)                                            //���� ������ 5 ������
    {
      button_events |= SW2;                                             //������������� ���� ������� ������
    }
  }
  else
  {
    button_events &= ~SW2;                                              //���������� ���� ������� ������
    CntPressSB2=0;	                                                //�������� �������
  }

  if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(SB3_GPIO_Port, SB3_Pin))       //���� ������ SB3 ���� ������
  {
    CntPressSB3++;		                                        //����������� ������� ������ �������� ������� ������
    HAL_Delay(10);						        //�������� 10 ��
    
    if(CntPressSB3>PRESS_SB)	                                        //���� ������ 5 ������
    {
      button_events |= SW3;                                             //������������� ���� ������� ������
    }
  }
  else
  {
    button_events &= ~SW3;                                              //���������� ���� ������� ������
    CntPressSB3=0;	                                                //�������� �������
  }
  
  if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(SB4_GPIO_Port, SB4_Pin))       //���� ������ SB4 ���� ������
  {
    CntPressSB4++;		                                        //����������� ������� ������ �������� ������� ������
    HAL_Delay(10);						        //�������� 10 ��
    
    if(CntPressSB4>PRESS_SB)	                                        //���� ������ 5 ������
    {
      button_events |= SW4;                                             //������������� ���� ������� ������
    }
  }
  else
  {
    button_events &= ~SW4;                                              //���������� ���� ������� ������
    CntPressSB4=0;	                                                //�������� �������
  }
  
  if(button_state_change != button_events)
  {
    button_state_change=button_events;
    return true;
  }
  else
    return false;
}

void Backlight(uint8_t enable)
{
  if(enable)
  {
    HAL_GPIO_WritePin(BACKLIGHT_GPIO_Port, BACKLIGHT_Pin, GPIO_PIN_RESET);      //BACKLIGHT ON
  }
  else
  {
    HAL_GPIO_WritePin(BACKLIGHT_GPIO_Port, BACKLIGHT_Pin, GPIO_PIN_SET);        //BACKLIGHT OFF
  }
}