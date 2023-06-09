/**************************************************************************//**
 *
 * @file user_controls.c
 *
 * @author (STUDENTS -- TYPE YOUR NAMES HERE)
 *
 * @brief Code to get inputs from the user.
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
#include <string.h>
#include <stdlib.h>
#include "user_controls.h"
#include "shared_variables.h"
#include "supplement.h"

volatile enum Mode Mode;

volatile bool requested_ping;


void initialize_controls(void) {
  requested_ping = false;

  if(cowpi_left_switch_is_in_left_position() && cowpi_right_switch_is_in_left_position()){
    Mode = normal_operation;
  }
  if (cowpi_left_switch_is_in_right_position() && cowpi_right_switch_is_in_left_position()){
    Mode = single_pulse_operation;
  }
  if(cowpi_left_switch_is_in_right_position() && cowpi_right_switch_is_in_right_position()){
    Mode = threshold_adjustment;
  }
  if(cowpi_left_switch_is_in_left_position() && cowpi_right_switch_is_in_right_position()){
    Mode = continuous_tone;
  }

}

void manage_controls(void) {
  static char top_line[17];
  if(cowpi_left_button_is_pressed() && Mode == single_pulse_operation){
    requested_ping = true;
  }
  if(Mode == threshold_adjustment){
    sprintf(top_line,"Enter threshold range (cm)");
    display_string(top_line, TOP_ROW);
  }
}
