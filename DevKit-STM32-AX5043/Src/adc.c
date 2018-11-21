#include "adc.h"
#include "main.h"


#define V_30C           520             //���������� (� ��) �� ������� ��� ����������� 30 �C.
#define V_Slope         4.3f            //��������� ���������� (� ��) ��� ��������� ����������� �� ������.
#define VDDA            3300            //���������� ������� ��� (� ��).
#define ADC_TIMEOUT     100


static ADC_HandleTypeDef 	AdcHandle;
static ADC_ChannelConfTypeDef 	sConfig;
int32_t ADC_VDDA;
int32_t ADC_temp;


void ADC_init(void)
{
  __HAL_RCC_ADC1_CLK_ENABLE();
  
  AdcHandle.Instance = ADC1;
  
  AdcHandle.Init.OversamplingMode      = DISABLE;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
  AdcHandle.Init.LowPowerAutoPowerOff  = ENABLE;
  AdcHandle.Init.LowPowerFrequencyMode = DISABLE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_160CYCLES_5;
  
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ContinuousConvMode    = DISABLE;
  AdcHandle.Init.DiscontinuousConvMode = ENABLE;
  AdcHandle.Init.ExternalTrigConv		 = ADC_SOFTWARE_START;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_SEQ_CONV;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  
  HAL_ADC_Init(&AdcHandle);
  HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED);
  
  sConfig.Channel = ADC_CHANNEL_VREFINT | ADC_CHANNEL_TEMPSENSOR;    
  HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
}

void ADC_deinit(void)
{
  HAL_ADC_DeInit(&AdcHandle);
  ADC->CCR = 0;
}

int8_t ADC_get(void)
{
  volatile uint16_t timeout = ADC_TIMEOUT;
  
  timeout = ADC_TIMEOUT;
  AdcHandle.Instance->CR |= ADC_CR_ADSTART;
  while(!__HAL_ADC_GET_FLAG(&AdcHandle, ADC_FLAG_EOC) && --timeout);
  if (!timeout)
    return -1;
  AdcHandle.Instance->ISR = 0xFFFF;
                                                                                
  ADC_VDDA = (AdcHandle.Instance->DR)*VDDA/4096;                //���������� � �� �� VREFINT STM32.
  
  timeout = ADC_TIMEOUT;
  AdcHandle.Instance->CR |= ADC_CR_ADSTART;
  while(!__HAL_ADC_GET_FLAG(&AdcHandle, ADC_FLAG_EOC) && --timeout);
  if (!timeout)
    return -1;
  AdcHandle.Instance->ISR = 0xFFFF;	
  ADC_temp = (AdcHandle.Instance->DR)*VDDA/4096;                //���������� � �� �� �������.
  ADC_temp = (int32_t)((ADC_temp-V_30C)/V_Slope + 30);          //����������� � ��������.
  
  return 0;
}