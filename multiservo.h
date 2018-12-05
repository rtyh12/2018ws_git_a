#ifndef MULTISERVO_H
#define MULTISERVO_H

/* 
	Servo 0 -> B1
	Servo 1 -> B2
	Servo 2 -> B3
	Servo 3 -> B4
	Servo 4 -> B5
	Servo 5 -> C0
*/

#define SERVO_MIN 999		// Links-Anschlag des Servos
#define SERVO_MAX 2001		// Rechts-Anschlag des Servos

void multiServoInit();
void multiSetServo(uint8_t nr, uint16_t us);

#endif
