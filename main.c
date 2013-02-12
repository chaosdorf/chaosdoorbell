#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>

int main (void)
{
	MCUSR = 0;
	WDTCSR = _BV(WDE) | _BV(WDP2) | _BV(WDP0);

	DDRB = _BV(DDB2) | _BV(DDB4);
	DDRD = _BV(DDD5);
	PORTB = _BV(PB2) | _BV(PB4);
	PORTD = _BV(PD6);

	ACSR = _BV(ACD);
	DIDR = _BV(AIN1D) | _BV(AIN0D);

	CLKPR = _BV(CLKPCE);
	CLKPR = _BV(CLKPS1) | _BV(CLKPS0);

	OCR0A = 0;
	OCR0B = 128;
	OCR1A = 0xff;
	TCCR0A = _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS00);
	TCCR1A = 0x00;
	TCCR1B = _BV(WGM12) | _BV(CS12);
	TIMSK = _BV(OCIE1A);

	sei();

	while (1) {
		MCUSR |= _BV(SE);
		asm("sleep");
	}

	return 0;
}

ISR(TIMER1_COMPA_vect)
{
	asm("wdr");
	if (~PIND & _BV(PD6)) {
		TCCR0A = _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
		TCCR0B = _BV(CS00);
		PORTB = _BV(PB2);
	}
	else {
		TCCR0A = 0;
		TCCR0B = 0;
		PORTB = _BV(PB2) | _BV(PB4);
		PORTD &= ~_BV(PD5);
	}
}
