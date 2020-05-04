/* \file lcddraw.c
 *  \brief Adapted from RobG's EduKit
 */
#include "lcdtypes.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "string.h"
#include "buzzer.h"
#include "libTimer.h"

char era[]="CLEAR";
char col[]="COLOR";
char siz[]="SIZE";
char shp[]="SHAPE";

char *str;
/** Draw single pixel at x,row 
 *
 *  \param col Column to draw to
 *  \param row Row to draw to
 *  \param colorBGR Color of pixel in BGR
 */
void drawPixel(u_char col, u_char row, u_int colorBGR) 
{
  lcd_setArea(col, row, col, row);
  lcd_writeColor(colorBGR);
}

/** Fill rectangle
 *
 *  \param colMin Column start
 *  \param rowMin Row start
 *  \param width Width of rectangle
 *  \param height height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void fillRectangle(u_char width, u_char height, 
		   u_int colorBGR)
{
  u_int center = screenWidth/2;
  u_int top = (screenHeight/2) - (height/2);
  for(int i = 0; i<height; i++){
    for(int j = center-(width/2); j < center + (width/2); j++){
      drawPixel(j, top+i, colorBGR);
    }
  }
}

void fillTrapezoid(u_char width, u_int colorBGR){
  u_char height = width/3;
  u_int center = screenWidth/2;
  u_int top = (screenHeight/2) - (height/2);
  
  for(int i=0; i<height; i++){//row
    //columns
    for(int j=0; j<=i+(width/4); j++){
      drawPixel(center-j, i+top, colorBGR); //left triangle
      drawPixel(center+j, i+top, colorBGR); //right triangle
    }
  }
}

void fillTriangle(u_char width, u_int colorBGR){

  u_char height = width/2;
  u_int center = screenWidth/2;
  u_int top = (screenHeight/2) - (height/2);

  for(int i = 0; i< height; i++){
    for(int j = 0; j <= i; j++){
      drawPixel(center - j, i + top , colorBGR);
      drawPixel(center + j, i + top, colorBGR);
    }
  }
}
void fillRhombus(u_char width, u_int colorBGR){

  u_char height = width;
  u_int center = screenWidth/2;
  u_int top = (screenHeight/2) - (height/2);

  for(int i = 0; i<= height/2; i++){
    for(int j = 0; j <= i; j++){
      drawPixel(center - j, i + top, colorBGR);
      drawPixel(center + j, i + top, colorBGR);
      drawPixel(center - j, top + height - i, colorBGR);
      drawPixel(center + j, top + height - i, colorBGR);
    }
  }
}
void fillParallelogram(u_char width, u_int colorBGR){
  u_char height = width/4;
  u_int center = screenWidth/2;
  u_int top = (screenHeight/2) - (height/2);
  for(int i = 0; i<height; i++){
    for(int j = center-(width/2); j < center+height; j++){
      drawPixel(j+i, top+i, colorBGR);
    }
  }
}
void fillHexagon(u_char width, u_int colorBGR){
  u_char height = width;
  u_int center = screenWidth/2;
  u_int top = (screenHeight/2) - (height/2);
  for(int i=0; i<=height/2; i+=2){//row
    //columns
    for(int k = 0; k<=2; k++){
      for(int j=0; j<=(i/2)+(width/4); j++){
      drawPixel(center-j, i+top+k, colorBGR); //left triangle
      drawPixel(center+j, i+top+k, colorBGR); //right triangle
      drawPixel(center-j, top+k + height - i, colorBGR); //left triangle
      drawPixel(center+j, top+k + height - i, colorBGR); //right triangle

    }
    }
  }
}
void fillRectangles(u_char colMin, u_char rowMin, u_char width, u_char height, 
		   u_int colorBGR)
{
  u_char colLimit = colMin + width, rowLimit = rowMin + height;
  lcd_setArea(colMin, rowMin, colLimit - 1, rowLimit - 1);
  u_int total = width * height;
  u_int c = 0;
  while ((c++) < total) {
    lcd_writeColor(colorBGR);
  }
}

/** Clear screen (fill with color)
 *  
 *  \param colorBGR The color to fill screen
 */
void clearScreen(u_int colorBGR) 
{
  u_char w = screenWidth;
  u_char h = screenHeight;
  fillRectangles(0,0,screenWidth, screenHeight, colorBGR);
}

/** 8x12 font - this function draws background pixels
 *  Adapted from RobG's EduKit
 */
void drawChar5x7(u_char rcol, u_char rrow, char c, 
     u_int fgColorBGR, u_int bgColorBGR) 
{
  u_char col = 0;
  u_char row = 0;
  u_char bit = 0x01;
  u_char oc = c - 0x20;

  lcd_setArea(rcol, rrow, rcol + 4, rrow + 7); /* relative to requested col/row */
  while (row < 8) {
    while (col < 5) {
      u_int colorBGR = (font_5x7[oc][col] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
    }
    col = 0;
    bit <<= 1;
    row++;
  }
}

/** Draw string at col,row
 *  Type:
 *  FONT_SM - small (5x8,) FONT_MD - medium (8x12,) FONT_LG - large (11x16)
 *  FONT_SM_BKG, FONT_MD_BKG, FONT_LG_BKG - as above, but with background color
 *  Adapted from RobG's EduKit
 *
 *  \param col Column to start drawing string
 *  \param row Row to start drawing string
 *  \param string The string
 *  \param fgColorBGR Foreground color in BGR
 *  \param bgColorBGR Background color in BGR
 */
void drawString5x7(u_char col, u_char row, char *string,
		u_int fgColorBGR, u_int bgColorBGR)
{
  u_char cols = col;
  while (*string) {
    drawChar5x7(cols, row, *string++, fgColorBGR, bgColorBGR);
    cols += 9;
  }
}

int shape2 = 0;
int size2 = 25;
u_int color2[] = {COLOR_ORANGE, COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_SKY_BLUE, COLOR_MAGENTA, COLOR_PURPLE};
int color3 = 0;

void display_command(){
  buzzer_init();
  clearScreen(COLOR_BLUE);
  drawString5x7((screenWidth/2)-(((strlen(str)+1)*7)/2),10, str, color2[color3], COLOR_BLUE);
  if (str[1] != 'L') draw_shape();
  else clearScreen(COLOR_BLACK);
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


