#include <avr/io.h>
#include <avr/interrupt.h>
#include "multiservo.h"


volatile uint16_t servoPos[6];
volatile uint8_t counter=0;


// Initialisiert die Pins B1 und B2 zur Ansteuerung von Servos
void multiServoInit() {
	
	uint8_t i;
	for (i=0; i < 6; i++) {
		servoPos[i] = 3000;
	}
	DDRB |= (1 << PB1) |(1 << PB2) |(1 << PB3) |(1 << PB4) |(1 << PB5);
	DDRC |= (1 << PC0);

	
	
	TIMSK1 |= (1<<OCIE1A) |(1<<TOIE1) ;
	TCCR1A |= (1 << WGM11);
	TCCR1B = (1 << WGM13) |(1 << WGM12) |  (1 << CS11) ;

	TCNT1 = 0;	
	ICR1 = 6666;
	

}

// Steuert ein Servo auf den entsprechenden Winkel. 
// Positionsangabe us zwischen 1000us und 2000us.
void multiSetServo(uint8_t nr, uint16_t us) {
	if ((us < SERVO_MAX && us > SERVO_MIN ) && nr <6) {
		servoPos[nr] = us*2;
	}
}

ISR(TIMER1_OVF_vect) {
	if (counter < 5) {
		PORTB |=(1 << (counter+1));
		counter++;
	} else {
		PORTC |=(1<<0);
		counter=0;
	}
	OCR1A = servoPos[counter];
}
ISR(TIMER1_COMPA_vect){
	// Alle aus
	PORTB &= ~((1 << PB1) |(1 << PB2) |(1 << PB3) |(1 << PB4) |(1 << PB5));
	PORTC &= ~((1 << PC0));
	
}
