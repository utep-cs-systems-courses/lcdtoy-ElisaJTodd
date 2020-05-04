#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"
#include "stateMachines.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "string.h"
int redrawScreen = 1;

void main(void) 
{ 
  configureClocks();
  
  led_init(); // enable leds
  switch_init(); // enable buttons
  lcd_init();
  enableWDTInterrupts(); //timer for songs
  or_sr(0x18);  // CPU off, GIE on
} 
