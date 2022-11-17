#define D0 eS_PORTB5
#define D1 eS_PORTB4
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4			//Defining all the pins for LCD screen 
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTB0
#define EN eS_PORTB1


#define F_CPU 16000000UL


#include "lcd.h"					//adding LCD library
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>


#define BAUDRATE 9600												//Setting the Baudrate for the data visuliser
#define BAUD_PRESCALLER ((( F_CPU / (BAUDRATE * 16UL))) -1)			// setting baud presscaller


unsigned char ReceiveChar;											// initialising ReceiveChar to store values received from USART

void USART_init(void){
	UBRR0H=(uint8_t)(BAUD_PRESCALLER>>8); //set baud rate
	UBRR0L=(uint8_t)(BAUD_PRESCALLER);
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);//enable receive
	UCSR0C=(3<<UCSZ00);//set 8-bit (default)
}

unsigned char USART_receive(void)
{
	while(!(UCSR0A&(1<<RXC0)));  // Wait to receive data
	return UDR0;// Read data from UDR
}





int main(void)
{
	
	Lcd8_Init();
	
	USART_init();
	DDRD = 0xFF;				//Set PortD as output
	DDRB = 0xFF;				//Set PortB as output
	Lcd8_Clear();				//clear screen
	char string_array[65];		//Max length for strings
	int count = 0;				//intitialise count as integer equals 0
	while(1)
	{	
		
		
		int i = 0;	//intialise i for array
		ReceiveChar = USART_receive(); // Wait until data is received
		
		
		if(ReceiveChar != '\n')		// keep storing the value until \n is pressed 
		{
			string_array[count] = ReceiveChar;			//store char value in string array
			count++;
		}
		
		else if (ReceiveChar == '\n')			// when \n is pressed display the string on LCD screen 
		{
			while(i<count)
			{
				Lcd8_Write_Char(string_array[i]);
				i++;
			}
			count = 0;							//reset the counter
			_delay_ms(10000);			//clear screen after 10 sec
			Lcd8_Clear();
		}

	}
}
	
