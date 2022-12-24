/*
 * Timer0_program.c
 *
 * Created: 12/19/2022 10:16:57 AM
 *  Author: Dell
 */ 
#include <avr/io.h>
#include "Timer0_interface.h"

void	Timer0_voidInt(void)
{
	// 1- Mode of operation :
//		Operation_Mode == Normal_Mode
		TCCR0	&=	~(1<<WGM00);
		TCCR0	&=	~(1<<WGM01);
/*	
#elif	Operation_Mode == Compare_Mode
		TCCR0	&=	~(1<<WGM00);
		TCCR0	|=	 (1<<WGM01);
	
#elif	Operation_Mode == PWM_PhaseCorrect_Mode
		TCCR0	|=	 (1<<WGM00);
		TCCR0	&=	~(1<<WGM01);
	
#elif	Operation_Mode == Fast_PWM_Mode
		TCCR0	|=	 (1<<WGM00);
		TCCR0	|=	 (1<<WGM01);
#endif
*/
	
	// 2- Compare Match Output Mode .
/*	( Operation_Mode == Normal_Mode ) || ( Operation_Mode == Compare_Mode )     // Non_PWM modes
	    TCCR0	&=  (0xCF);
	    TCCR0	|=  ( OC0_Operation_Mode << COM00);
		
#elif	Operation_Mode == Fast_PWM_Mode   // Fast PWM mode
	    TCCR0	&=  (0xCF);
		if( OC0_Operation_Mode != 1) {TCCR0	|=  ( OC0_Operation_Mode << COM00); }

#elif	Operation_Mode == PWM_PhaseCorrect_Mode
	    TCCR0	&=  (0xCF);
		if( OC0_Operation_Mode != 1) {TCCR0	|=  ( OC0_Operation_Mode << COM00); }

*/

/*
	// 3- Interrupt Type.
#if		Interrupt_Type == No_Interrupt
		TIMSK	&=		~(1<<TOIE0);
		TIMSK	&=		~(1<<OCIE0);
		
#elif	Interrupt_Type == OutputCompare_Interrupt
		TIMSK	&=		~(1<<TOIE0);
		TIMSK	|=		 (1<<OCIE0);
*/	
//		Interrupt_Type == OverFlow_Interrupt
		TIMSK	&=		~(1<<OCIE0); 
		TIMSK	|=		 (1<<TOIE0);
	/*	
#elif	Interrupt_Type == OverFlow_OutputCompare_Interrupt
		TIMSK	|=		 (1<<OCIE0);
		TIMSK	|=		 (1<<TOIE0);	
#endif*/
}



void	Timer0_Start()
{
	//SFIOR	|=	  (1<<PSR10);		// When this bit is written to one, the (Timer/Counter1) and (Timer/Counter0) prescaler will be reset.
	TCCR0	|=	  (Timer0_Scaler);	// set the CS-bits with the defined value in the interface file.
}


void	Timer0_Stop(void)
{
	//SFIOR	|=	  (1<<PSR10);				// When this bit is written to one, the (Timer/Counter1) and (Timer/Counter0) prescaler will be reset.
	TCCR0	|=	  (TimerCounter_stopped);	// set the CS-bits with the defined value in the interface file.
}


void	Timer0_OutputCompareValue(char OutputCompare_value)
{
	Timer0_Stop();
	if ((Operation_Mode != Fast_PWM_Mode ) && (Operation_Mode != PWM_PhaseCorrect_Mode))
	{
		OCR0 = OutputCompare_value;
		Timer0_Start();
	}
}

void	Timer0_AnalogWrite(char Analog_value)
{
	Timer0_Stop();
	if ((Operation_Mode == Fast_PWM_Mode ) || (Operation_Mode == PWM_PhaseCorrect_Mode))
	{
		OCR0 = Analog_value;
		Timer0_Start();
	}
}










