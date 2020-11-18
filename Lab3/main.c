/*
 * GccApplication1.c
 *
 * Created: 05.11.2020 13:46:18
 * Author : USER
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/delay.h>
#include <string.h>
unsigned char key_code[4][3]={{'1','2','3'},
							  {'4','5','6'},
							  {'7','8','9'},
							  {'*','0','#'}};
void String_7219(char* number)
{
	for(int i=1;i<=4;i++)
	{
		Send_7219(i,number[4-i]-'0');
	}
	
}								  
char dict[10][4]={"0000","0001","0011","0111","1111","1110","1100","1000","0000","0001"};
void pause (unsigned int a)
{ unsigned int i;
	
	for (i=a;i>0;i--);
}
char dg = 4;
int NumForPrint=0;
void SPI_init(void)
{
	DDRB |= ((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5)); //����� SPI �� �����
	PORTB &= ~((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5)); //������ �������
	SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR0);//������� ����, ������� �������, �������� 16
}

void SPI_SendByte (char byte)
{
	SPDR = byte;
	while (!(SPSR & (1 << SPIF)));
}

void Send_7219(char rg, char dt)
{
	PORTB &= ~(1<<PORTB2);
	SPI_SendByte(rg);
	SPI_SendByte(dt);
	PORTB |= (1<<PORTB2);
}

void Clear_7219(void)
{
	char i = dg;
	// Loop until 0, but don't run for zero
	do {
		// Set each display in use to blank
		Send_7219(i, 0xF); //CHAR BLANK
	} while (--i);
}

void Number_7219(volatile long n)
{
	char ng = 0; //���������� ��� ������
	if (n < 0)
	{
		ng = 1;
		n *= -1;
	}
	Clear_7219();
	if (n == 0) {
		Send_7219(0x01, 0); //� ������ ������ ������� 0
		return;
	}
	char i = 0;
	do {
		Send_7219(++i, n%10);
		n /= 10;
	} while (n);
	if (ng) {
		Send_7219(i+1, 0xA); //������ -
	}
}

void NumberFull_7219(volatile long n)
{
	int i;
	Clear_7219();
	for(i=0;i<dg;i++)
	{
		Send_7219(i+1,(n/((long)pow(10,i)))%10);
	}
}

void init_timer (void)
{
	TIMSK0=(1<<TOIE0);		        //��������� ���������� �� ������������ �������0
	TCCR0B=(1<<CS00)|(1<<CS01)|(0<<CS02);  //�������� =/64
}
ISR (TIMER0_OVF_vect)
{ unsigned char i,j;
	
	 DDRC=0x00;	//������������� ���� C ��� ����
	 PORTC=0x0F;   //������� �� 4 ������� ���� ����� C ���. 1
	 DDRD=0x0F;	//������������� ���� B ��� �����
	 PORTD=0x00;   //�������� ���� B
	 
	 pause(10);	//�������� ��� ���������� ������ ���������� ���������, ����� �� �� ������!
	 i=4;
	 if ((PINC&0x01)==0x00) i=0;		//���� ������ ������� � 0� �������, i=0
	 if ((PINC&0x02)==0x00) i=1;		//���� ������ ������� � 1� �������, i=1
	 if ((PINC&0x04)==0x00) i=2;           //���� ������ ������� � 3� �������, i=3
	 
	 DDRC =0x0F;	//������������� ���� C ��� �����
	 PORTC=0x00;   //�������� ���� C
	 DDRD =0x00;	//������������� ���� B ��� ����
	 PORTD=0x0F;   //������� �� 4 ������� ���� ����� B ���. 1
	 
	 pause(10);	//�������� ��� ���������� ������ ���������� ���������, ����� �� �� ������!
	 j=4;
	 if ((PIND&0x01)==0x00) j=0;		//���� ������ ������� � 0� ������, j=0
	 if ((PIND&0x02)==0x00) j=1;		//...
	 if ((PIND&0x04)==0x00) j=2;
	 if ((PIND&0x08)==0x00) j=3;
	if ((i!=4)&&(j!=4)) {			//���� ���� ������ �������
		while ((PIND&_BV(j))==0x00);	//���� ���� ������ ��������
			
		String_7219(dict[key_code[j][i]-'0']);	
		
	}
	
	TCNT0=0x00;		//������� �������
	TIFR0=0x00;              //������� ���� ������������
	
	return;
}
int main(void)
{
	
    /* Replace with your application code */
	init_timer(); 
 
   SPI_init();
   Send_7219(0x09, 0xFF); //������� ����� �������������
   Send_7219(0x0B, dg - 1); //������� �������� ����������
   Send_7219(0x0A, 0x0E); //�������
   Send_7219(0x0C, 1); //������� ���������
   Clear_7219();
   Number_7219((long)5555);
   _delay_ms(500);
    Clear_7219();
   Number_7219((long)4444);
   sei();
   while(1);
}

