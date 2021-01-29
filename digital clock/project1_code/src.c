
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char TICK = 0;
unsigned char SEC = 0;
unsigned char MIN = 0;
unsigned char HOUR = 0;
unsigned char digit[2]= {0};

void Timer1_init(void);
void val_to_digit(char c);
void Int0_init ();
void Int1_init ();

int main ()
{
	Timer1_init();
	Int0_init ();
	Int1_init ();
	DDRB  = 0x1F ;
	DDRA  = 0x0F;
	PORTB = 0;
	PORTA = 0;

	while (1)
	{
		char count = 0 ;
		PORTB = (PORTB & 0XC0)|(1<<count);
		val_to_digit (SEC);
		PORTA = (PORTA & 0XF0) | (digit[0] & 0X0F);
		_delay_ms(5) ;

		count ++ ;
		PORTB = (PORTB & 0XC0)|(1<<count);
		val_to_digit (SEC);
		PORTA = (PORTA & 0XF0) | (digit[1] & 0X0F);
		_delay_ms(5) ;

		count ++ ;
		PORTB = (PORTB & 0XC0)|(1<<count);
		val_to_digit (MIN);
		PORTA = (PORTA & 0XF0) |(digit[0] & 0X0F);
		_delay_ms(5) ;

		count ++ ;
		PORTB = (PORTB & 0XC0)|(1<<count);
		val_to_digit (MIN);
		PORTA = (PORTA & 0XF0) | (digit[1] & 0X0F);
		_delay_ms(5) ;

		count ++ ;
		PORTB = (PORTB & 0XC0)|(1<<count);
		val_to_digit (HOUR);
		 PORTA = (PORTA & 0XF0) | (digit[0] & 0X0F);
		_delay_ms(5) ;

		count ++ ;
		PORTB = (PORTB & 0XC0)|(1<<count);
		val_to_digit (HOUR);
		PORTA = (PORTA & 0XF0) |( digit[1] & 0X0F);
		_delay_ms(5) ;

	}
}

void Timer1_init(void)
	{
		SREG  |= (1<<7);
		TCCR1A = (1<<FOC1B)|(1<<FOC1A);
		TCCR1B = (1<<WGM12)|(1<<CS12)|(1<<CS10);
		TCNT1 = 0;
		OCR1A = 1000;
		TIMSK |= (1<<OCIE1A);
	}

void val_to_digit(char c)
{
	digit [0] = c % 10;
	c = c/10;
	digit [1] = c;
}

void Int0_init ()
{
	SREG  |= (1<<7);
	DDRD  &= (~(1<<PD2));
	PORTD |=(1<<PD2);
	GICR  |= (1<<INT0);
	MCUCR = (1<<ISC01);
}

void Int1_init ()
{
	SREG  |= (1<<7);
	DDRD  &= (~(1<<PD3));
	PORTD |=(1<<PD3);
	GICR  |= (1<<INT1);
	MCUCR = (1<<ISC11);
}

ISR(TIMER1_COMPA_vect)
	{
		SEC = TICK ;
		if (SEC == 60)
		{
			SEC = 0;
			TICK = 0;
			MIN ++ ;
		}
		if (MIN == 60)
			{
				SEC = 0;
				MIN = 0;
				HOUR ++ ;
			}
		if (HOUR == 24)
			{
			SEC = 0;
			MIN = 0;
			HOUR = 0;
			}
		TICK ++;
	}

ISR(INT0_vect)
{
	SEC = 0;
	MIN = 0;
	HOUR = 0;
	TICK = 0;
}

ISR(INT1_vect)
{
	TCCR1B = 0;
}



