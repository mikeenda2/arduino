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
volatile enum Mode;

void handle_pulse_interrupt(){}

cowpi_timer16bit_t *timer1 = (cowpi_timer16bit_t *)(COWPI_IO_BASE + 0x60);

//create enum or state for different controls

void initialize_sensor(void) {
  object_detected = false;
  Mode = normal_operation;
  attachInterrupt(digitalPinToInterrupt(3), handle_pulse_interrupt,CHANGE);
  timer1 -> control = 0xC8;//hex 200, sets prescaler to 8
  timer1 -> compareA = 65536 - 1;
  volatile uint16_t *timer_interrupt_masks = 1 | 2;

}

void manage_sensor(void) {
  if(requested_ping == true){
    //set arduino d2 to logic-high
    digitalWrite(2,HIGH);
    requested_ping = false;
    //Next, delay for 10μs
    delayMicroseconds(10);
    //Finally, set pin D2 to logic-low
    digitalWrite(2,LOW);
  }
}

// ISR(TIMER1_COMPA_vect) {
  
// }
