/*
 * GccApplication4.c
 * Author : Dell
 */ 

// the frequency of the Micro controller is 16 MHz. 
#define  F_CPU  16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/****************************Bit Math Macros******************************/
#define 	SET_BIT(VAR,BITNO) 		(VAR) |=  (1 << (BITNO))
#define 	CLR_BIT(VAR,BITNO)		(VAR) &= ~(1 << (BITNO))
#define 	TOG_BIT(VAR,BITNO) 		(VAR) ^=  (1 << (BITNO))
#define 	GET_BIT(VAR,BITNO) 		(((VAR) >> (BITNO)) & 0x01)                                                    
/*************************************************************************/

/******************************7_Segment**********************************/
// array store the right sequence of segments for each number.
unsigned short _7_segment[10] = {
									0b11000000 ,	//0
									0b11111001 ,	//1
									0b10100100 ,	//2
									0b10110000 ,	//3
									0b10011001 ,	//4
									0b10010010 ,	//5
									0b10000011 ,	//6
									0b11111000 ,	//7
									0b10000000 ,	//8
									0b10011000 ,	//9
								}; 

// array to store the separated Digits that came from the Get_Digits function.							         
char		Display_Num[4];
// variable to store the location of the DOT.
char		DOT_Place;

// a prototype for Displaying the Digit on the 7_Segment.
void	_7Segment_Display(char Num);
// a prototype for taking the total number of Millie Seconds and convert it into separated digits. 
char	Get_Digits(unsigned long int Total_Num);                                                           
/************************************************************************/
// a prototype for the function that will run in the While(1) loop.
void	Open_LOOP();
/*************************Timer/Counter**********************************/
// define a variable that store the number of the overflowing
int OverFlow_Counter;                                                 
/************************************************************************/


int main(void)
{	/***********************TIMER0 MAIN CONFIGURATION************************/  
	// 	16 MHz experiment :
	//	Normal Mode
	// 	TCNT0 = 6
	// 	Prescaler = 64
	// 	T = 1 mS

	// we Use the Normal mode ,so we just need to enable the overflow interrupt.
	//  Enable OverFlow Interrupt.
	SET_BIT(TIMSK,TOIE0);          
	/************************************************************************/
	SET_BIT(DDRD,DDD7) ;
	/**********************7_Segments GENERAL PINS***************************/
	// 7_segment Configurations
	DDRA	=    (0xFF) ;    // all pins in PORTA are output pins.
	PORTA	=    (0x00) ;	 //the start state of these pins are (LOW) state
	// 7_segment Enable Pins.
	DDRD   |=   (0x78)	;    // pin 3,4,5,6 in PORTD are output
	CLR_BIT(PORTD,3);		 // the start state of these pins are low state
	CLR_BIT(PORTD,4);		 // the start state of these pins are low state
	CLR_BIT(PORTD,5);		 // the start state of these pins are low state
	CLR_BIT(PORTD,6);		 // the start state of these pins are low state
	/************************************************************************/
	
	
	/*************************EXTERNAL INTERRUPT******************************/	
	// 1- configure the INT0 pin as an input Pull_Up pin.
	CLR_BIT(DDRD,DDD2);
	SET_BIT(PORTD,PORTD2);
	//1- External interrupt (INT0) on Falling Edge.
	SET_BIT(MCUCR,ISC01);
	// 2- External interrupt (INT0) Enable.
	SET_BIT(GICR,INT0);
	/************************************************************************/
	
	/***************************Initializations******************************/
	// initiate the OverFlow Counter at the beginning before anything
	OverFlow_Counter = 0;
	/************************************************************************/
	sei();
	while (1) 
	    {	Open_LOOP();  };
}




/**************************7_Segment Functions***************************/
char	Get_Digits(unsigned long int Total_Num)
{	// define a variable that store the Right Location of the DOT.
	char Local =0 ;
	// if the Number of Millie seconds is less than (1 S).
	if (Total_Num <= 999)
	{	
		// display (0) on the last Digit. 
		Display_Num[3] = 0;
		
		// display (Total_Num /100) on the third Digit.
		Display_Num[2] = (Total_Num /100);
		
		// display ((Total_Num-(Display_Num[2]*100))/10) on the second Digit.
		Display_Num[1] = ((Total_Num-(Display_Num[2]*100))/10);
		
		// display ((Total_Num) % 10) on the First Digit.
		Display_Num[0] = ((Total_Num) % 10);
		
		// Return (3) the location of the DOT meaning that the value is less than (1 S), and exit from the function.
		return 3;
	}
	else if (Total_Num <= 9999)
	{
		// the location of the DOT is in (3) meaning that the value is less than (10 S), and complete the function.
		Local= 3;
	}
	else if (Total_Num <= 99999)
	{
		// display just the last four Digits. 
		Total_Num /= 10;
		// the location of the DOT is in (2) meaning that the value is less than (100 S), and complete the function.
		Local= 2;
	}else if (Total_Num <= 999999)
	{
		// display just the last four Digits.
		Total_Num /= 100;
		// the location of the DOT is in (2) meaning that the value is less than (1000 S), and complete the function.
		Local= 1;
	}else if (Total_Num <= 9999999)
	{
		// display just the last four Digits.
		Total_Num /= 1000;
		// the location of the DOT is in (2) meaning that the value is less than (10000 S), and complete the function.
		Local= 0;
	}
	// display ((Total_Num)/1000) on the last Digit
	Display_Num[3] = ((Total_Num)/1000);
	// display ((Total_Num - ( (Display_Num[3])*1000) ) / 100) on the third Digit.
	Display_Num[2] = ((Total_Num - ( (Display_Num[3])*1000) ) / 100);
	// display (((Total_Num-(Display_Num[3]*1000))-(Display_Num[2]*100))/10) on the second Digit.
	Display_Num[1] = (((Total_Num-(Display_Num[3]*1000))-(Display_Num[2]*100))/10);
	// display ((Total_Num) % 10) on the First Digit.
	Display_Num[0] = ((Total_Num) % 10);
	
	// return the location of the DOT.
	return Local;
}
//////////////////////////////////////////////////////////////////////////
void	_7Segment_Display(char Display_Value)
{	// make all Pins in PORTA HIGH ---> Disable All Segments.
	PORTA	=  (0xFF);
	// Write the PORTA to a specific value the represent the given number. 
	PORTA	=  _7_segment[Display_Value];
}                                                                    
/************************************************************************/

                                  
/************************************************************************/
ISR(INT0_vect)
{	// Calculate the number of Millie seconds.
	DOT_Place=Get_Digits(OverFlow_Counter);
	
	// change the State of the TIMER/COUNTER peripheral (START/STOP) by changing its prescaler value.
	// (CS01,CS00 = 0,0 -----> Stop the Peripheral) , (CS01,CS00 = 1,1 -----> Start the Peripheral , and the prescaler is (Fclk/64) ).
	TCCR0	^=  ( 1 << CS01) ;
	TCCR0	^=  ( 1 << CS00) ;
	// change the State of the LED (START/STOP) , to start with the Starting of the Timer and Stop with it.
	PORTD   ^=	( 1 << PORTD7);
	// initiate the TCNT0 register (the counter at the Timer Peripheral)
	TCNT0 = 6;
	// initiate the OverFlow counter to (0) for each time we press the button.
	OverFlow_Counter = 0;

}
//////////////////////////////////////////////////////////////////////////
ISR(TIMER0_OVF_vect)
{	// increasing the OverFlow Counter By (1) each Time.
	OverFlow_Counter++;
	// initiate the TCNT0 register (the counter at the Timer Peripheral) .
	TCNT0 = 6;
}
/************************************************************************/



/*******************The Function of Dispaying****************************/
void	Open_LOOP()
{	// the counter of the Loop.
	char local_counter ;
	while (1)
	{	// Loop on the 7_segment matrix Digits.
		for (local_counter = 0; local_counter < 4; local_counter++)
		{	// Enable the wanted Digit.
			SET_BIT(PORTD,local_counter+3);					
			_7Segment_Display(Display_Num[local_counter]);
			// Put the DOT on its place.
			if (DOT_Place == local_counter)					
			{ CLR_BIT(PORTA,PORTA7); }
			// Delay for the presentation.
			_delay_ms(5);		
			// Disable the Number on its Digit.									
			CLR_BIT(PORTD,local_counter+3);					
		}
	};
}
/************************************************************************/