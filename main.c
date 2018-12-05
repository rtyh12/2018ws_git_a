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
#include "servo.h"


void init();

int main(void)
{
	//Initialisierung ausfuehren
	
	init();

	while (1)
	{
		uart_putc('X');
	}	
}


//INIT
void init()
{
	uartInit();   // serielle Ausgabe an PC
	ADCInit(0);   // Analoge Werte einlesen
	PWMInit();    // Pulsweite auf D6 ausgeben 
	timerInit();  // "Systemzeit" initialisieren
	//servoInit();  // Servoansteuerung initialisieren
}
