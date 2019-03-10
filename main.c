/*
 *    Basis: 2009 Benjamin Reh und Joachim Schleicher
 *    Erweitert: 2019 Charles Barbret, Finn Münnich, Mihai Verzan
 */

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
#define LOOP_DELAY 10               // size of "timestep"

int32_t clock;                      // milliseconds. current position in the song
#define HIT_DELAY 20                 // time it takes to hit a string from the idle position

uint16_t pos[SERVO_COUNT];          // current position per servo
int16_t dir[SERVO_COUNT];           // current direction per servo: -1 for backwards, 1 for forwards, 0 for stopped
int16_t offset[SERVO_COUNT];        // offset from "true" center position. offset +- angle may not go out of the range (1000, 2000), otherwise the servos will cry :(

uint16_t speed = 35;

uint16_t angle[SERVO_COUNT];        // how wide the angle of each servo is
const uint16_t middle = 1500;       // don't change this

const uint16_t loopForeverAndEver = 0;
uint16_t hit_i = 0;                 // current note in the song


// gets called once at the start of the program
void init();


void loop() {
    int16_t i;

    // stop all servos that went out of range
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

    // if time remaining until the next hit is supposed to take place <= time it takes to hit a string,
    // but before the next timestep has started
    if (clock >= hits[hit_i + 1].time - HIT_DELAY &&
        clock  < hits[hit_i + 1].time - HIT_DELAY + LOOP_DELAY) {

        // then start to move the servo forwards/backwards, depending on which side we are on right now
        dir[hits[hit_i + 1].string] = pos[hits[hit_i + 1].string] < middle ? 1 : -1;

        hit_i += 1;
    }

    for (i = 0; i < SERVO_COUNT; i++) {
        pos[i] += dir[i] * speed;

        // the previous sections ignore the offset, it's only used here.
        multiSetServo(i, pos[i] + offset[i]);
    }

    clock += LOOP_DELAY;

    if (loopForeverAndEver) {
        if (hit_i >= HIT_COUNT - 1) {
            // reset time to beginning when we've reached the end
            clock = -HIT_DELAY;
            hit_i = 0;
        }
    }

    _delay_ms(LOOP_DELAY);
}


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


void init() {
    uartInit();         // serielle Ausgabe an PC
    ADCInit(0);         // Analoge Werte einlesen
    timerInit();        // "Systemzeit" initialisieren
    multiServoInit();   // Servoansteuerung initialisieren

    clock = -HIT_DELAY - 1500;

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
