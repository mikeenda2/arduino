/*                       *
 * DO NOT EDIT THIS FILE *
 *                       */

/**************************************************************************//**
 *
 * @file supplement.h
 *
 * @author Christopher A. Bohn
 *
 * @brief Candidate functions, macros, and definitions for the CowPi library.
 *
 * These include:
 * - a debouncing macro that is appropriate when you can guarantee the
 *   debouncing action will only be invoked once per input activation
 *   (where, of course, this activation could really be a bounce)
 * - a debouncing function for values requiring no more than one byte;
 *   this function can safetly be used in a polling function but requires that
 *   a unique key (i.e., the input source) be associated with the value being
 *   debounced
 * - a function to place a string on the top or bottom row of an LCD1602
 *   display module
 *
 ******************************************************************************/

/* CowPi (c) 2021-23 Christopher A. Bohn
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SUPPLEMENT_H
#define SUPPLEMENT_H

#include <Arduino.h>
#include <CowPi.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEBOUNCE_THRESHOLD 40
#define NUMBER_OF_DISPLAYABLE_COLUMNS 16

enum input_names {
  LEFT_BUTTON = 0,
  RIGHT_BUTTON,
  LEFT_SWITCH,
  RIGHT_SWITCH,
  KEYPAD,
  NUMBER_OF_INPUTS
};

enum row_names {
  TOP_ROW = 0,
  BOTTOM_ROW
};

#define debounce_interrupt(action)                            \
  do {                                                        \
    static unsigned long last_trigger = 0L;                   \
    unsigned long now = millis();                             \
    if (now - last_trigger > DEBOUNCE_THRESHOLD) { action; }  \
    last_trigger = now;                                       \
  } while(0)

static inline uint8_t debounce_byte(uint8_t current_value, enum input_names input_name) {
  static uint8_t last_actual_value[NUMBER_OF_INPUTS] = { 0 };
  static uint8_t last_good_value[NUMBER_OF_INPUTS] = { 0 };
  static unsigned long last_change[NUMBER_OF_INPUTS] = {[0 ... (NUMBER_OF_INPUTS-1)] = 0x80000000};   // gcc extension
  unsigned long now = millis();

  /* 
  // responds immediately and then ignores further changes until input stabilizes -- more responsive
  last_good_value[input_name] = (now - last_change[input_name] < DEBOUNCE_THRESHOLD) ? last_good_value[input_name] : current_value;
  last_change[input_name] = (current_value == last_actual_value[input_name]) ? last_change[input_name] : now;
  */
  /* ...OR... */
  // ignores changes until input stabilizes and then responds -- more immune to transients
  last_change[input_name] = (current_value == last_actual_value[input_name]) ? last_change[input_name] : now;
  last_good_value[input_name] = (now - last_change[input_name] < DEBOUNCE_THRESHOLD) ? last_good_value[input_name] : current_value;

  last_actual_value[input_name] = current_value;
  return last_good_value[input_name];
}

static inline void display_string(const char string[static (NUMBER_OF_DISPLAYABLE_COLUMNS+1)], enum row_names row) {
  static char copy_of_string[NUMBER_OF_DISPLAYABLE_COLUMNS+1];
  memset(copy_of_string, 0, NUMBER_OF_DISPLAYABLE_COLUMNS+1);
  memcpy(copy_of_string, string, NUMBER_OF_DISPLAYABLE_COLUMNS);
  cowpi_lcd1602_place_cursor(0x40 * row);
  for (char *c = copy_of_string; *c; c++) {
    cowpi_lcd1602_send_character(*c);
  }
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif //SUPPLEMENT_H
