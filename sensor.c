/**************************************************************************//**
 *
 * @file sensor.c
 *
 * @author (STUDENTS -- TYPE YOUR NAMES HERE)
 *
 * @brief Code to manage the distance sensor.
 *
 ******************************************************************************/

/*
 * RangeFinder GroupLab assignment and starter code (c) 2023 Christopher A. Bohn
 * RangeFinder solution (c) the above-named students
 */

#include <CowPi.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "sensor.h"
#include "shared_variables.h"
#include "supplement.h"

volatile bool object_detected;
volatile long object_distance;
volatile long object_approach;


void initialize_sensor(void) {
  object_detected = false;
  Mode = normal;
  attachInterrupt(digitalPinToInterrupt(3),handle_button_action,CHANGE);
  cowpi_timer16bit_t *timer1 = (cowpi_timer16bit_t *)(COWPI_IO_BASE + 0x80);
  timer1->TCCR1A = 0;
  timer1->TCCR1B = (1 << CS10) | (1 << CS12); // Prescaler of 1024
  timer1->TCNT1 = 0;
  timer1->OCR1A = 511; // Compare value for 32,768μs overflow
  timer1->TIMSK1 |= (1 << OCIE1A); // Enable overflow interrup
}

void manage_sensor(void) {

}

ISR(TIMER1_COMPA_vect) {
  
}
