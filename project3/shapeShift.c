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
int shape2 = 1;
int size2 = 25;
u_int color2[] = {COLOR_ORANGE, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_SKY_BLUE, COLOR_MAGENTA, COLOR_PURPLE};
int color3 = 0;
char *str = "START";

void display_command(){
  clearScreen(COLOR_BLUE);
  drawString5x7((screenWidth/2)-(((strlen(str)+1)*7)/2),10, str, color2[color3], COLOR_BLUE);
  if (str[1] == 'L') clearScreen(COLOR_BLACK);
  else draw_shape();
}

void draw_shape(){

  switch(shape2){
  case 1:
    fillRectangle(size2,size2,color2[color3]);
    break;
  case 2:
    fillTrapezoid(size2,color2[color3]);
   break;
  case 3:
    fillTriangle(size2,color2[color3]);
   break;
  case 4:
    fillRhombus(size2,color2[color3]);
   break;
  case 5:
    fillParallelogram(size2,color2[color3]);
   break;
  case 6:
    fillRectangle(size2, size2/2, color2[color3]);
    break;
  default:
    fillHexagon(size2,color2[color3]);
    shape2 = 0;
  }
}

void main(void) 
{ 
  configureClocks();
  lcd_init();
  led_init(); // enable leds
  switch_init(); // enable buttons
  enableWDTInterrupts(); //timer for songs
  or_sr(0x8);  //GIE on
  for(;;) {
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
       or_sr(0x10);      /**< CPU OFF */
}
    redrawScreen = 0;
    state_advance();
    display_command();
  }
} 
