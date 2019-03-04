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



#define SERVO_COUNT 6
#define LOOP_DELAY 10

int32_t clock;
#define HIT_DELAY 50

uint16_t pos[SERVO_COUNT];
int16_t dir[SERVO_COUNT];
int16_t offset[SERVO_COUNT];	// offset from "true" center position. offset +- angle may not go out of the range (1000, 2000), otherwise the servos will cry :(

uint16_t speed = 5;

uint16_t z = 1500;

uint16_t angle[SERVO_COUNT];
const uint16_t middle = 1500;

const int loopForeverAndEver = 1;



struct Hit {
	uint16_t string;
	uint16_t time;
};

#define HIT_COUNT 12
struct Hit hits[HIT_COUNT];



// gets called once at the start of the program
void init();


int main(void) {
	init();
	
	// each call to this function is one iteration of the loop
	while (1) {
		int16_t i;
		for (i = 0; i < SERVO_COUNT; i++) {
			// stop servos that went out of this range:
			// set dir to 0 (stop movement)
			// and set pos to the respective edge of the range
			if (pos[i] > middle + angle[i]) {
				dir[i] = 0;
				pos[i] = middle + angle[i];
			}
			if (pos[i] < middle - angle[i]) {
				dir[i] = 0;				
				pos[i] = middle - angle[i];
			}

			// iterate through all hits
			int16_t j;
			for (j = 0; j < HIT_COUNT; j++) {
				// if time remaining until the hit is supposed to take place <= time it takes to hit a string,
				// but before the next timestep has started
				if (clock >= hits[j].time - HIT_DELAY && clock < hits[j].time - HIT_DELAY + LOOP_DELAY) {
					// then start to move the servo: forwards/backwards, depending on which side we are on right now
					dir[hits[j].string] = pos[hits[j].string] < middle ? 1 : -1;
				}
			}		

			pos[i] += dir[i] * speed;

			multiSetServo(i, pos[i] + offset[i]);
		}				

		clock += LOOP_DELAY;
		
		if (loopForeverAndEver) {
			if (clock >= 5500)
		        clock = 0;
		}

		_delay_ms(LOOP_DELAY);
	}
}


//INIT
void init() {
	uartInit();			// serielle Ausgabe an PC
	ADCInit(0);			// Analoge Werte einlesen
	timerInit();		// "Systemzeit" initialisieren
	multiServoInit();	// Servoansteuerung initialisieren
	
	clock = -HIT_DELAY - 2500;

	hits[0].string = 3;
	hits[0].time = 0;

	hits[1].string = 3;
	hits[1].time = 500;

	hits[2].string = 3;
	hits[2].time = 1000;

	hits[3].string = 3;
	hits[3].time = 1500;

	hits[4].string = 3;
	hits[4].time = 2000;

	hits[5].string = 3;
	hits[5].time = 2500;

	hits[6].string = 3;
	hits[6].time = 3000;

	hits[7].string = 3;
	hits[7].time = 3500;

	hits[8].string = 3;
	hits[8].time = 4000;

	hits[9].string = 3;
	hits[9].time = 4500;

	hits[10].string = 3;
	hits[10].time = 5000;

	hits[11].string = 3;
	hits[11].time = 5500;
	
	/*
	pos[0] = 1500;
	dir[0] = 1;*/


	int16_t i;
	for (i = 0; i < SERVO_COUNT; i++)
		angle[i] = 20;

	for (i = 0; i < SERVO_COUNT; i++)
		pos[i] = middle - angle[i];
	
	for (i = 0; i < SERVO_COUNT; i++)
		dir[i] = 0;
		
	
	offset[0] = 0;
	offset[1] = 0;
	offset[2] = 0;
	offset[3] = -20;
	offset[4] = 0;
	offset[5] = 0;
}
