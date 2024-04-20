/*
 * ultrasonic.c
 *
 * Created: 09/08/2022 12:17:59
 *  Author: Mahmoud Abdelmoniem
 */
#include "ultrasonic.h"

static volatile uint16 t1,t2,flag=0,c=0;
void tmr1_ov_interput();
void interrupt_icu();

timer1_t tmr={
	.mode=TMR1_PWM_FAST_MODE_OCR1A,
	.prescaler=TMR1_8_PRESCALER,
	.channle=TIMER1_OCR1b,
	.fast_pwm=OC1_NON_INVERTING,
	.TMR1_InterriptOveFlow=tmr1_ov_interput,
	.TMR1_InterriptCAPT=interrupt_icu
};
Std_ReturnType UltraSonic_Init(const ultrasonic_t* ultra){
		Std_ReturnType ret=E_OK;
		if(((void*)0) == ultra)
		{
			ret = E_NOT_OK;
		}
		else{
			ret = gpio_pin_init(&(ultra->trigger));
			OCR1A=39999;
			ret = TMR1_Init(&tmr);

			ret = TMR1_Interrupt_CAPT_enable(interrupt_icu);


		}
		return ret;
}
Std_ReturnType Ultrasonic_GetDistance(const ultrasonic_t* ultra,uint16 *_distance)
{
		Std_ReturnType ret=E_OK;
		if(((void*)0) == _distance)
		{
			ret = E_NOT_OK;
		}
		else{
			uint32 time;
			c=0;
			flag=0;
			ret = gpio_write_logic(&(ultra->trigger),GPIO_HIGH);
			_delay_us(10);
			ret = gpio_write_logic(&(ultra->trigger),GPIO_LOW);

			ret = TMR1_Set_CaptureEdge(ICU_RISING_EDGE);
			ret = TMR1_Interrupt_CAPT_enable(interrupt_icu);
			TMR1_Interrupt_Over_Flow_enable(&tmr);
			while(flag <2);

			time =((uint32) (t2-t1)+ ((uint32)(c*65535)))/2;
			*_distance= time/58;

			ret = TMR1_Interrupt_CAPT_Disable();
		}
		return ret;
}


void interrupt_icu()
{
	if(flag == 0)
	{
		c=0;
		t1=ICR1;
		flag=1;
		TMR1_Set_CaptureEdge(ICU_FALLING_EDGE);
	}
	else if(flag == 1)
	{
		t2=ICR1;
		flag=2;
		TMR1_Set_CaptureEdge(ICU_RISING_EDGE);
		TMR1_Interrupt_CAPT_Disable();
		TMR1_Interrupt_Over_Flow_Disable();
	}
}
void tmr1_ov_interput()
{
	c++;
}
