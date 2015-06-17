#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 1
#define DWELL_MAX 0

#define INITIALSNAKELENGTH 5 //The initial snake length at the start of the game.

Adafruit_NeoMatrix * matrix;

uint8_t rotation = 0;
uint16_t xd = 0;
uint16_t yd = 0;
uint8_t previousLeft = 0;
uint8_t previousRight = 0;

uint8_t A1val = 0;
uint8_t D0val = 0;
uint8_t dwell_time = 0;

uint8_t Direction = 1;

uint8_t snaketrix[255];
uint8_t snake_length = INITIALSNAKELENGTH;
uint8_t i;
uint8_t die = 0;

uint8_t appleCoord;

//Create a new apple once one has been eaten by the snake
void makeApple() {
  appleCoord = random(255);
  uint8_t conflict = 1;
  while (conflict) { //Check to see if the apple was created on top of the snake itself.
    conflict = 0;
    for (i = 0; i < snake_length; i++) {
      if (appleCoord == snaketrix[i]) {
        conflict = 1;
        appleCoord = (appleCoord + 1)%255;
      }
    }
  }
}

//Call to start the game over again
void setupSnake() {
  snake_length = INITIALSNAKELENGTH;
  memset(snaketrix,0x00,255);
  for (i = 0; i < snake_length; i++)
  {
    snaketrix[(snake_length-1)-i] = i+32;
  }
  makeApple();
  Direction = 1;
}

//Draws specified rectangles.
void drawFullRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
  uint8_t iterx = 0;
  uint8_t itery = 0;
  for (iterx = x; iterx < x+w; iterx++) {
    for (itery = y; itery < y+h; itery++) {
      matrix->drawPixel(iterx,itery,color);
    }
  }
}

//Shows the 'explosion' when the snake dies, resets the game
void epicDeath() {
  delay(30);
  drawFullRect(7,7,2,2,matrix->Color(0,255,0));
  
  rotation += 15;
  matrix->show();
  delay(30);
  drawFullRect(6,6,4,4,matrix->Color(0,255,0));
  
  rotation += 15;
  matrix->show();
 
  delay(30);
  drawFullRect(5,5,6,6,matrix->Color(0,255,0));
  
  rotation += 15;
  matrix->show();
 
  delay(30);
  drawFullRect(4,4,8,8,matrix->Color(0,255,0));
  
  rotation += 15;
  matrix->show();
  
   
  delay(30);
  drawFullRect(3,3,10,10,matrix->Color(0,255,0));
  rotation += 15;
  matrix->show();
  
    delay(30);
  drawFullRect(2,2,12,12,matrix->Color(0,255,0));
  rotation += 15;
  matrix->show();
  
  delay(30);
  drawFullRect(1,1,14,14,matrix->Color(0,255,0));
  rotation += 15;
  matrix->show();
  
  delay(30);
  drawFullRect(0,0,16,16,matrix->Color(0,255,0));
  rotation += 15;
  matrix->show();
 
  delay(30);
  drawFullRect(7,7,2,2,matrix->Color(165,255,0));
  
  rotation += 15;
  matrix->show();
  delay(30);
  drawFullRect(6,6,4,4,matrix->Color(165,255,0));
  
  rotation += 15;
  matrix->show();
 
  delay(30);
  drawFullRect(5,5,6,6,matrix->Color(165,255,0));
  
  rotation += 15;
  matrix->show();
 
  delay(30);
  drawFullRect(4,4,8,8,matrix->Color(165,255,0));
  
  rotation += 15;
  matrix->show();
  
   
  delay(30);
  drawFullRect(3,3,10,10,matrix->Color(165,255,0));
  rotation += 15;
  matrix->show();
  
    delay(30);
  drawFullRect(2,2,12,12,matrix->Color(165,255,0));
  rotation += 15;
  matrix->show();
  
    delay(30);
  drawFullRect(7,7,2,2,matrix->Color(255,255,0));
  
  rotation += 15;
  matrix->show();
  delay(30);
  drawFullRect(6,6,4,4,matrix->Color(255,255,0));
  
  rotation += 15;
  matrix->show();
 
  delay(30);
  drawFullRect(5,5,6,6,matrix->Color(255,255,0));
  
  rotation += 15;
  matrix->show();
 
  delay(30);
  drawFullRect(4,4,8,8,matrix->Color(255,255,0));
  
  rotation += 15;
  matrix->show();

  delay(2500);
  
  rotation += 15;
  matrix->show();
  matrix->fillScreen(0);
  
  setupSnake();
  makeApple();
  die = 0; 
}

void setup() {
  matrix = new Adafruit_NeoMatrix(8, 8, 2, 2, PIN,
    NEO_MATRIX_TOP     + NEO_MATRIX_LEFT+
    NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE +
    NEO_TILE_TOP + NEO_TILE_ZIGZAG,
    NEO_RGB            + NEO_KHZ800);
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(25);
  matrix->fillScreen(0);
  memset(snaketrix,0x00,255);
  for (i = 0; i < snake_length; i++)
  {
    snaketrix[(snake_length-1)-i] = i+32;
  }
  pinMode(0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(A2));
  makeApple();
}

void loop() {

  
  if (digitalRead(0)) previousLeft = 1;
  if (digitalRead(A1)) previousRight = 1;
  for (i = 0; i < 140; i++ ) {
    D0val |= digitalRead(0);
    A1val |= digitalRead(A1);
    delay(1);
  }
  if (previousLeft) D0val = 0;
  if (previousRight) A1val = 0;
  previousLeft = 0;
  previousRight = 0;
  
  if (D0val && dwell_time >= DWELL_MAX) {
    Direction = (Direction + 1)%4;
    dwell_time = 0;
    D0val = 0;
    A1val = 0;
  }
  else if (A1val && dwell_time >= DWELL_MAX) {
    Direction = (Direction + 3)%4;
    dwell_time = 0;
    D0val = 0;
    A1val = 0;
  }
  dwell_time++;
  
 for (i = 0; i < snake_length; i++)
 {
   snaketrix[ (snake_length-1-i)+1] = snaketrix[(snake_length-1)-i];
 } 
  switch (Direction) {
    case 0:
      if ((snaketrix[0]>>4) > 0) snaketrix[0] -= 0x10;
      else {
        die = 1;
          for (i = 0; i < snake_length; i++)
          {
           snaketrix[i] = snaketrix[i+1];
          } 
      }
      break;
    case 1:
      if ( (snaketrix[0] & 0xf) < 15) snaketrix[0] += 0x1;
      else {
        die = 1;
          for (i = 0; i < snake_length; i++)
          {
           snaketrix[i] = snaketrix[i+1];
          } 
      }
      break;
    case 2:
      if ((snaketrix[0]>>4) < 15) snaketrix[0] += 0x10;
      else {
        die = 1;
          for (i = 0; i < snake_length; i++)
          {
           snaketrix[i] = snaketrix[i+1];
          } 
      }
      break;
    case 3:
      if ( (snaketrix[0] & 0xf) > 0) snaketrix[0] -= 0x1;
      else {
        die = 1;
          for (i = 0; i < snake_length; i++)
          {
           snaketrix[i] = snaketrix[i+1];
          } 
      }
      break;
  }
  
  if (snaketrix[0] == appleCoord) {
    snake_length++;
    makeApple();
  }
  
  for (i = 1; i < snake_length; i++) {
    if (snaketrix[i] == snaketrix[0]) die = 1;
  }
  
  matrix->clear();
  
  
  for (i = 0; i < snake_length; i++)
  {
    matrix->drawPixel( (((uint8_t)snaketrix[i] & 0xF0) >> 4), ((uint8_t)snaketrix[i]&0xf), matrix->Color(255,255*D0val,255*A1val));
    //matrix->drawPixel(1, 1, matrix->Color(255,0,0));
  }
  matrix->drawPixel( (((uint8_t)appleCoord & 0xF0) >> 4), ((uint8_t)appleCoord&0xf), matrix->Color(0,255,0));
  matrix->show();
  
  if (die) epicDeath();
  
}
