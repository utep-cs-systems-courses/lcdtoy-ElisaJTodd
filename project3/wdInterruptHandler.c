#include <msp430.h>
#include "stateMachines.h"
#include "buzzer.h"
#include "lcddraw.h"

void
__interrupt_vec(WDT_VECTOR) WDT(){
  static char second_count = 0;
  if (++second_count == 30 && i<4) { //note length
    play_notes(); 
    second_count = 0;
  }
}
