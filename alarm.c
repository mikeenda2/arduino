/**************************************************************************//**
 *
 * @file alarm.c
 *
 * @author Michael Endacott & Jackson Merkel
 *
 * @brief Code to manage the piezodisc and LEDs.
 *
 ******************************************************************************/

/*
 * RangeFinder GroupLab assignment and starter code (c) 2023 Christopher A. Bohn
 * RangeFinder solution (c) the above-named students
 */

#include <CowPi.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "alarm.h"
#include "shared_variables.h"
#include "supplement.h"


const unsigned int on_period = 7000;          // a made-up number, probably not the value you want to use
volatile unsigned int total_period = 50000;

// need to declare these in the other c file too if globally used
// when a ping is requested, alarm_should_be_sounded = true, counter_since_start becomes 0, and ping requested variable set to false
volatile uint8_t counter = 0;
volatile uint8_t counter_since_start = 0;
volatile bool alarm_should_be_sounded = false;
volatile bool requested_ping;


ISR(TIMER1_COMPA_vect) {
  counter_since_start++;
  // after this is working, set this if statement so it only goes when on contiuous tone mode
  // if (Mode == continuous_tone) {
    counter++;
    if (counter % 2 == 0) {
        digitalWrite(9,HIGH);
    } else {
        digitalWrite(9,LOW);
    }
  
  if(counter_since_start >= total_period) {
    counter_since_start = 0;
  }
}

void initialize_alarm(void) {
  cowpi_timer8bit_t *timer = (cowpi_timer8bit_t*) (COWPI_IO_BASE + 0x90); // if this doesnt work anymore i added a * to the end of () cowpi
  timer -> control = 0x202;
  timer -> compareA = 199;
  volatile uint8_t *timer_interrupt_masks = COWPI_IO_BASE + 0x4E;
  timer_interrupt_masks[2] = 0x02;
  volatile cowpi_ioport_t *ioports = (cowpi_ioport_t*)(COWPI_IO_BASE + 0x20);
  ioports[D8_D13].direction |= (1 << (9 - D8_D13));
}

void manage_alarm(void) {

}