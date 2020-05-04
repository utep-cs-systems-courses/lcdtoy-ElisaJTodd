#include <msp430.h>
#include "libTimer.h"
#include "stateMachines.h"
#include "buzzer.h"
#include "lcddraw.h"

void
__interrupt_vec(WDT_VECTOR) WDT(){
  and_sr(~16); //cpu on
  static char second_count = 0;
  if (++second_count == 30 && i<4) { //note length
    play_notes(); 
    second_count = 0;
  }
}
