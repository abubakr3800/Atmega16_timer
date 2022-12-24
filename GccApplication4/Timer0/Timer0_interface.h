/*
 * Timer0_interface.h
 *
 * Created: 12/19/2022 10:16:21 AM
 *  Author: Dell
 */ 


#ifndef TIMER0_INTERFACE_H_
#define TIMER0_INTERFACE_H_



/*				  configurations					*/

// Operation Modes
#define		Normal_Mode				0
#define		Compare_Mode			1
#define		PWM_PhaseCorrect_Mode	2
#define		Fast_PWM_Mode			3

#define		Operation_Mode		Normal_Mode

// Timer0 Scaler
#define		TimerCounter_stopped		0
#define		clkI_0						1
#define		clkI_8						2
#define		clkI_64						3
#define		clkI_256					4
#define		clkI_1024					5

#define		Timer0_Scaler		clkI_64

//	OC0 Modes
#define		OC0_Disconnected			0		// No Control on any I/O pin.
#define		OC0_Toggle					1		// just in Non_PWM modes.
#define		OC0_Clear					2		/* In Non_PWN : (Clear OC0 on compare match) , In Fast_PWM : (non-inverting mode)
												 * In Phase_PWM : (Clear OC0 up-counting. Set OC0 down-counting). 
												 */ 
#define		OC0_Set						3		 /* In Non_PWN : (Set OC0 on compare match) , In Fast_PWM : (inverting mode)
												 * In Phase_PWM : (Set OC0 up-counting. Clear OC0 down-counting). 
												 */

#define		OC0_Operation_Mode			OC0_Disconnected	 

// Interrupt Cases
#define		No_Interrupt						0	// when both TOIE0 and OCIE0 = 0
#define		OutputCompare_Interrupt				1	// when TOIE0 = 0 and OCIE0 = 1
#define		OverFlow_Interrupt					2   // when OCIE0 = 0 and TOIE0 = 1
#define		OverFlow_OutputCompare_Interrupt	3   // when both TOIE0 and OCIE0 = 1

#define		Interrupt_mode				OverFlow_Interrupt

/*				Function Prototypes					*/


void	Timer0_voidInt(void);

void	Timer0_Start();
void	Timer0_Stop();

void	Timer0_OutputCompareValue(char Analog_value);
void	Timer0_AnalogWrite(char Analog_value);
#endif /* TIMER0_INTERFACE_H_ */