/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "multiservo.h"


void init();

int main(void) {
	// Initialisierung ausfuehren

	init();
	
	
	uint16_t z = 1000;
	while (1) {
		multiSetServo(1,z);
		z+=10;
		if (z>2000)
			z=1000;
		
		_delay_ms(20);
		
	}
}


//INIT
void init() {
	uartInit();		// serielle Ausgabe an PC
	ADCInit(0);		// Analoge Werte einlesen
	timerInit();		// "Systemzeit" initialisieren
	multiServoInit(); // Servoansteuerung initialisieren
}
