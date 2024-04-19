#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

struct t{
	char sec;
	char min;
	char hr;
}t1;

ISR(TIMER1_COMPA_vect){
	t1.sec++;
}

ISR(INT0_vect){

	TCNT1=0;
	t1.sec=0;
	t1.min=0;
	t1.hr=0;
}

ISR(INT1_vect){

	TCCR1B&=~(1<<CS11)&~(1<<CS10);
}

ISR(INT2_vect){

	TCCR1B|=(1<<CS11)|(1<<CS10);
}

void TIMER1_CTC(void){
	TCNT1=0;
	OCR1A=15625;
	TIMSK|=(1<<OCIE1A);
	TCCR1A|=(1<<FOC1A);
	TCCR1B|=(1<<WGM12)|(1<<CS11)|(1<<CS10);
}

void RESET_INT0(void){

	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);
	GICR|=(1<<INT0);
}

void PAUSE_INT1(void){

	MCUCR|=(1<<ISC11)|(1<<ISC10);
	GICR|=(1<<INT1);
}

void RESUME_INT2(void){

	MCUCSR&=~(1<<ISC2);
	GICR|=(1<<INT2);
}

int main(void){
	char i;
	DDRA|=0x3F;
	DDRC|=0x0F;
	DDRB&=~(1<<PB2);
	DDRD&=0xF3;

	PORTA&=~(0x3F);
	PORTC&=0xF0;

	SREG|=(1<<7);

	TIMER1_CTC();
	RESET_INT0();
	PAUSE_INT1();
	RESUME_INT2();

	while(1){
		if(t1.sec==60){
			t1.sec=0;
			t1.min++;
		}
		if(t1.min==60){
			t1.min=0;
			t1.hr++;
		}
		for(i=0;i<6;i++){
			if(i%2){
				PORTC=(PORTC&0xF0)|(((*(&t1.sec+(i/2)))/10)&0x0F);
				PORTA|=(1<<i);
				_delay_us(2);
				PORTA&=~(1<<i);
			}
			else{
				PORTC=(PORTC&0xF0)|(((*(&t1.sec+(i/2)))%10)&0x0F);
				PORTA|=(1<<i);
				_delay_us(2);
				PORTA&=~(1<<i);
			}

		}
	}
}
