#include "stm32f0xx.h"        // Device header


uint8_t i,flag=0;
uint16_t TimingDelay,led_count,adc_data;

uint16_t buf[32];
uint8_t buf_l;

void TimingDelayDec(void) {
 if (TimingDelay			!=0x00) TimingDelay--;
 if (!led_count) {led_count=500;}// GPIOB->ODR ^=1;}
 led_count--;
 }

void TIM17_IRQHandler(void){
		if (TIM17->SR & TIM_SR_UIF) {
					TimingDelayDec();
  				TIM17->SR &=(~TIM_SR_UIF);
		}
}	
 void delay_ms (uint16_t DelTime) {
    TimingDelay=DelTime;
  while(TimingDelay!= 0x00);
}

void initial (void)
{
	/*timer config*/
  RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;    						//HSI 8 MHz - 1 msek
  TIM17->PSC = 8000-1;
  TIM17->ARR = 1;
  TIM17->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR | TIM_CR1_CEN; // 
	TIM17->DIER |=TIM_DIER_UIE;
				
	NVIC_EnableIRQ (TIM17_IRQn);
	NVIC_SetPriority(TIM17_IRQn,0x05);	
	
	/*GPIOB PIN 0 */ 
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN; 								//
	GPIOB->MODER |= GPIO_MODER_MODER0_1;								//Pb0-Alternative 
	GPIOB->AFR[0] |= 0x01 << GPIO_AFRL_AFRL0_Pos;  			// Pb0 - AFR1 (TIM3_CH3)

	
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; 								//
	GPIOA->MODER |= GPIO_MODER_MODER7;									// Analog mode PA7


	RCC->APB2ENR |=RCC_APB2ENR_ADC1EN;
	ADC1->CHSELR |=ADC_CHSELR_CHSEL7;										//Channel 7 of ADC
	ADC1->CFGR1 |=0x01<<ADC_CFGR1_RES_Pos; //10 bit resolution
	ADC1->CR |=ADC_CR_ADCAL;
	while(ADC1->CR & ADC_CR_ADCAL){};
	ADC1->CR |=ADC_CR_ADEN;


	RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
	
	TIM3->ARR = 100; 																			// 
	
	TIM3->CCR3 = 0;  
	TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
	TIM3->CCER |= TIM_CCER_CC3E; 
 	TIM3->CR1 |= TIM_CR1_CEN;
	TIM3->EGR |= TIM_EGR_UG;
} 

int main(void)
{
initial();

//-----------------------------initial data----------------------------------

while (1)  /* Main loop */
{
while (!(ADC1->ISR & ADC_ISR_ADRDY)){};	
ADC1->CR |=ADC_CR_ADSTART;
while (!(ADC1->ISR & ADC_ISR_EOC)){};
adc_data=ADC1->DR/10;
ADC1->CR |=ADC_CR_ADSTP;	
	
TIM3->CCR3 = adc_data;        
			
delay_ms(10);
	
} // end - main loop 
} // end - Main  
	
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ while (1)  {  } }
#endif
