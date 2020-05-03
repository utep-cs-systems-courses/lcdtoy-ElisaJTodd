#include <msp430.h>
#include "led.h"
#include "switches.h"
#include "stateMachines.h"
#include "buzzer.h"
#include "lcdtypes.h"
#include "lcddraw.h"
#include "lcdutils.h"

char switch_state_down; /* effectively boolean */
char sw_1, sw_2, sw_3, sw_4;

static unsigned char switches_last_reported;
static unsigned char switches_current;

static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  switches_current = P2IN & SWITCHES;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (switches_current);  /* if switch up, sense down */
  P2IES &= (switches_current | ~SWITCHES); /* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE = SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

void
switch_interrupt_handler()
{
  char p1val = switch_update_interrupt_sense();
  sw_1 = (p1val & SW1) ? 0 : 1;
  sw_2 = (p1val & SW2) ? 0 : 1;
  sw_3 = (p1val & SW3) ? 0 : 1;
  sw_4 = (p1val & SW4) ? 0 : 1; // 0 when switch down
  switch_state_down = (sw_1 || sw_2 || sw_3 || sw_4); //any switch pressed
  state_advance();
  if(switch_state_down){
    display_command();
    green_on = 1;
    red_on = 0;
    led_changed = 1;
    led_update();
  }
}