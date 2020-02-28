#pragma once

/*
	Based on, and mostly copied from, the tft_espi example called:
	TFT_Pong
*/
#include "HS_ScreenBase.h"

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

class HS_PongScreen :
	public HS_ScreenBase
{
private:
	//HS_Theme pongTheme;

	unsigned long lastUpdate = 0;
	int updateScreenInterval = 2;


    int16_t h = 240;
    int16_t w = 320;

    int dly = 5;

    int16_t paddle_h = 30;
    int16_t paddle_w = 4;

    int16_t lpaddle_x = 0;
    int16_t rpaddle_x = w - paddle_w;

    int16_t lpaddle_y = 0;
    int16_t rpaddle_y = h - paddle_h;

    int16_t lpaddle_d = 1;
    int16_t rpaddle_d = -1;

    int16_t lpaddle_ball_t = w - w / 4;
    int16_t rpaddle_ball_t = w / 4;

    int16_t target_y = 0;

    int16_t ball_x = 2;
    int16_t ball_y = 2;
    int16_t oldball_x = 2;
    int16_t oldball_y = 2;

    int16_t ball_dx = 1;
    int16_t ball_dy = 1;

    int16_t ball_w = 6;
    int16_t ball_h = 6;

    int16_t dashline_h = 4;
    int16_t dashline_w = 2;
    int16_t dashline_n = h / dashline_h;
    int16_t dashline_x = w / 2 - 1;
    int16_t dashline_y = dashline_h / 2;

    int16_t lscore = 12;
    int16_t rscore = 4;

    void Initgame();
    void midline();
    void lpaddle();
    void rpaddle();
    void calc_target_y();
    void ball();

public:
	HS_PongScreen(Queues *newQueues, TFT_eSPI* newTFT, int chipSelectPin);
	~HS_PongScreen();
	//void UpdateScreen(String value);
	void UpdateScreenOnInterval();
};

