#include <avr/io.h>
#include <stdlib.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "multiservo.h"

#include "notes1.h"


#define SERVO_COUNT 6
#define LOOP_DELAY 10

int32_t clock;
#define HIT_DELAY 50

uint16_t pos[SERVO_COUNT];
int16_t dir[SERVO_COUNT];
int16_t offset[SERVO_COUNT];	// offset from "true" center position. offset +- angle may not go out of the range (1000, 2000), otherwise the servos will cry :(

uint16_t speed = 35;

uint16_t angle[SERVO_COUNT];
const uint16_t middle = 1500;

const int loopForeverAndEver = 0;
uint16_t hit = 0;                 // current note in the song


// gets called once at the start of the program
void init();

void loop();

int main(void) {
	init();

	while (1) {
		loop();
	}
}


// returns 1 if Hit a comes before Hit b, -1 if the other way around, 0 if the times are the same.
// used to sort the array
int compare_hits(const void* a, const void* b) {
	struct Hit arg1 = *(const struct Hit*)a;
	struct Hit arg2 = *(const struct Hit*)b;

	if (arg1.time < arg2.time) return -1;
	if (arg1.time > arg2.time) return 1;
	return 0;
}

//INIT
void init() {
	uartInit();			// serielle Ausgabe an PC
	ADCInit(0);			// Analoge Werte einlesen
	timerInit();		// "Systemzeit" initialisieren
	multiServoInit();	// Servoansteuerung initialisieren

	clock = -HIT_DELAY - 2500;

	set_notes();        // from the (hopefully) automatically generated notes.h

	// sort the array based on the time of each hit
	qsort(hits, HIT_COUNT, sizeof(struct Hit), compare_hits);

	int16_t i;
	for (i = 0; i < SERVO_COUNT; i++)
		angle[i] = 35;

	for (i = 0; i < SERVO_COUNT; i++)
		pos[i] = middle - angle[i];

	for (i = 0; i < SERVO_COUNT; i++)
		dir[i] = 0;


	offset[0] = -70; // (length and position) adjusted
	offset[1] = 40; // (length and position) adjusted
	offset[2] = -40; // (length and position) adjusted
	offset[3] = -40; // (length and position) adjusted
	offset[4] = 92; // (length and position) adjusted
	offset[5] = -105; // (length and position) adjusted
}

void loop() {
	// stop servos that went out of this range
	int16_t i;
	for (i = 0; i < SERVO_COUNT; i++) {
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
	}

	// iterate through all hits
	int16_t j;
	for (j = hit; j < HIT_COUNT; j++) {
		// if time remaining until the hit is supposed to take place <= time it takes to hit a string,
		// but before the next timestep has started
		if (clock >= hits[j].time - HIT_DELAY && 
			clock  < hits[j].time - HIT_DELAY + LOOP_DELAY) {
			// then start to move the servo: forwards/backwards, depending on which side we are on right now
			dir[hits[j].string] = pos[hits[j].string] < middle ? 1 : -1;
			hit += 1;
		} // else if (clock >= hits[j].time + HIT_DELAY + LOOP_DELAY) {
			// break;
		// } // this else block does not work, it swallowes too many notes... nearly
		// all of them, need to check where the logic mistake is TODO
	}

	// move servos
	for (i = 0; i < SERVO_COUNT; i++) {
		pos[i] += dir[i] * speed;
		multiSetServo(i, pos[i] + offset[i]);
	}

	clock += LOOP_DELAY; 
	
	// should the song be played for ever or just once
	if (loopForeverAndEver) {
		if (hit >= HIT_COUNT - 1){
			clock = -HIT_DELAY;
			hit = 0;
		}
	}

	_delay_ms(LOOP_DELAY);
}	
