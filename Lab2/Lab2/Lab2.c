#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
int main(void)
{
//	unsigned char i;
	unsigned char butcount=0;
	DDRD = 0xFF;
	DDRB = 0x00;
	PORTD = 0b00000000;
	PORTB = 0b00000001;
	bool State=false;
while(1)
{							
		if (!(PINB&0b00000001))
		{			
			if(butcount < 5)
			{
				butcount++;
			}
			else
			{
				State = !State;
				PORTD = 0b00000001;
				_delay_ms(500);
																
				while(State)
				{
				  for(int i=0;i<=7;i++)
				  {					 
					 if (!(PINB&0b00000001))
					 {
						  State = false;
						   _delay_ms(500);
						    break;
					 }
						PORTD = (1<<i);
						_delay_ms(500);																
				  }
				   if (State)
				    for(int i=6;i>=1;i--)
				    {	
					 if (!(PINB&0b00000001)) break;				
						PORTD = (1<<i);
						_delay_ms(500);					
				    }
				 }
			
		    }
			}
		    else
		 {
			if(butcount > 0)
			{
				butcount--;
			}
			else
			{
				PORTD = 0b00000000;
			}
		}
		

	
	
}


}