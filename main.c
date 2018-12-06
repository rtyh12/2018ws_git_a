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

	uint16_t z = 1500; // recomend starting at the same position as the initialized value, as this way when the program starts up, it does not perform a jump
	//uint16_t y = 1300;

	uint16_t c = 15; // having the speed at 15 seems to be a good speed to adequately play notes
	uint16_t dir = 1;

	while (1) {
		multiSetServo(0, z);
		//multiSetServo(1, y);
		//multiSetServo(5, z);

		if (dir) {	
			z += c;
			//y -= c;
		}
		else {
			z -= c;
			//y += c;
		}

		if (z > 1650)
			dir = 0;
		if (z < 1350) // having a distance of 300 seems to be a good distance to play a singular string
			dir = 1;
		
		_delay_ms(20);
		
	}
}


//INIT
void init() {
	uartInit();			// serielle Ausgabe an PC
	ADCInit(0);			// Analoge Werte einlesen
	timerInit();		// "Systemzeit" initialisieren
	multiServoInit();	// Servoansteuerung initialisieren
}
