#include "Pong.h"

HS_PongScreen::HS_PongScreen(Queues *newQueues, TFT_eSPI* newTFT, int chipSelectPin) : HS_ScreenBase(newQueues, newTFT, chipSelectPin)
{
	screenRotation = 1;
    PrepDisplayForUpdate();
    TFT->fillScreen(TFT_BLACK);
    
    Initgame();
    TFT->setTextColor(WHITE, BLACK);

    CompleteDisplayUpdate();
}

HS_PongScreen::~HS_PongScreen()
{

}


void HS_PongScreen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > updateScreenInterval)
	{
		PrepDisplayForUpdate();

        lpaddle();
        rpaddle();
        midline();
        ball();
        
		CompleteDisplayUpdate();
        lastUpdate = millis();
	}
}

void HS_PongScreen::Initgame()
{
  lpaddle_y = random(0, h - paddle_h);
  rpaddle_y = random(0, h - paddle_h);

  // ball is placed on the center of the left paddle
  ball_y = lpaddle_y + (paddle_h / 2);
  
  calc_target_y();

  midline();

  TFT->fillRect(0,h-26,w,239,GREY);

  TFT->setTextDatum(TC_DATUM);
  TFT->setTextColor(WHITE,GREY);
  TFT->drawString("TFT_eSPI example", w/2, h-26 , 4);
}

void HS_PongScreen::midline() {

  // If the ball is not on the line then don't redraw the line
  if ((ball_x<dashline_x-ball_w) && (ball_x > dashline_x+dashline_w)) return;

  TFT->startWrite();

  // Quick way to draw a dashed line
  TFT->setAddrWindow(dashline_x, 0, dashline_w, h);
  
  for(int16_t i = 0; i < dashline_n; i+=2) {
    TFT->pushColor(WHITE, dashline_w*dashline_h); // push dash pixels
    TFT->pushColor(BLACK, dashline_w*dashline_h); // push gap pixels
  }

  TFT->endWrite();
}

void HS_PongScreen::lpaddle() {
  
  if (lpaddle_d == 1) {
    TFT->fillRect(lpaddle_x, lpaddle_y, paddle_w, 1, BLACK);
  } 
  else if (lpaddle_d == -1) {
    TFT->fillRect(lpaddle_x, lpaddle_y + paddle_h - 1, paddle_w, 1, BLACK);
  }

  lpaddle_y = lpaddle_y + lpaddle_d;

  if (ball_dx == 1) lpaddle_d = 0;
  else {
    if (lpaddle_y + paddle_h / 2 == target_y) lpaddle_d = 0;
    else if (lpaddle_y + paddle_h / 2 > target_y) lpaddle_d = -1;
    else lpaddle_d = 1;
  }

  if (lpaddle_y + paddle_h >= h && lpaddle_d == 1) lpaddle_d = 0;
  else if (lpaddle_y <= 0 && lpaddle_d == -1) lpaddle_d = 0;

  TFT->fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
}

void HS_PongScreen::rpaddle() {
  
  if (rpaddle_d == 1) {
    TFT->fillRect(rpaddle_x, rpaddle_y, paddle_w, 1, BLACK);
  } 
  else if (rpaddle_d == -1) {
    TFT->fillRect(rpaddle_x, rpaddle_y + paddle_h - 1, paddle_w, 1, BLACK);
  }

  rpaddle_y = rpaddle_y + rpaddle_d;

  if (ball_dx == -1) rpaddle_d = 0;
  else {
    if (rpaddle_y + paddle_h / 2 == target_y) rpaddle_d = 0;
    else if (rpaddle_y + paddle_h / 2 > target_y) rpaddle_d = -1;
    else rpaddle_d = 1;
  }

  if (rpaddle_y + paddle_h >= h && rpaddle_d == 1) rpaddle_d = 0;
  else if (rpaddle_y <= 0 && rpaddle_d == -1) rpaddle_d = 0;

  TFT->fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);
}

void HS_PongScreen::calc_target_y() {
  int16_t target_x;
  int16_t reflections;
  int16_t y;

  if (ball_dx == 1) {
    target_x = w - ball_w;
  } 
  else {
    target_x = -1 * (w - ball_w);
  }

  y = abs(target_x * (ball_dy / ball_dx) + ball_y);

  reflections = floor(y / h);

  if (reflections % 2 == 0) {
    target_y = y % h;
  } 
  else {
    target_y = h - (y % h);
  }
}

void HS_PongScreen::ball() {
  ball_x = ball_x + ball_dx;
  ball_y = ball_y + ball_dy;

  if (ball_dx == -1 && ball_x == paddle_w && ball_y + ball_h >= lpaddle_y && ball_y <= lpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
    dly = random(5); // change speed of ball after paddle contact
    calc_target_y(); 
  } else if (ball_dx == 1 && ball_x + ball_w == w - paddle_w && ball_y + ball_h >= rpaddle_y && ball_y <= rpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
    dly = random(5); // change speed of ball after paddle contact
    calc_target_y();
  } else if ((ball_dx == 1 && ball_x >= w) || (ball_dx == -1 && ball_x + ball_w < 0)) {
    dly = 5;
  }

  if (ball_y > h - ball_w || ball_y < 0) {
    ball_dy = ball_dy * -1;
    ball_y += ball_dy; // Keep in bounds
  }

  //TFT->fillRect(oldball_x, oldball_y, ball_w, ball_h, BLACK);
  TFT->drawRect(oldball_x, oldball_y, ball_w, ball_h, BLACK); // Less TFT refresh aliasing than line above for large balls
  TFT->fillRect(   ball_x,    ball_y, ball_w, ball_h, WHITE);
  oldball_x = ball_x;
  oldball_y = ball_y;
}