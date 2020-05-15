#include <msp430.h>
#include "libTimer.h"
#include "stateMachines.h"
#include "buzzer.h"
#include "lcddraw.h"
#include "shapeShift.h"
#include "switches.h"

void
__interrupt_vec(WDT_VECTOR) WDT(){
  static char second_count = 0;
  second_count++;
  if (second_count == 30){
    if(i<4) { //note length
      play_notes(); 
    }
    redrawScreen = 1;
    and_sr(~16);
    second_count = 0;
  }
}
