#pragma once
/*
	Based on, and mostly copied from, the tft_espi example called:
	TFT_Meter_linear
*/

#include "HS_ScreenBase.h"

#define M_SIZE 1.3333

#define TEXT_COLOR 0xCE59
#define PANEL_HCOLOR 0x5AEB
#define PANEL_BGCOLOR 0x738E
#define BOX_BORDER_COLOR 0x9CD3
#define BOX_DROP_SHADOW 0x4208

#define NET_PANEL_X 0
#define NET_PANEL_Y 40

#define SERVER_PANEL_X 0
#define SERVER_PANEL_Y 85

class HS_SplashScreen :
	public HS_ScreenBase
{
private:

	unsigned long lastUpdate = 0;
	int updateScreenInterval = 30;

	float ltx = 0;    // Saved x coord of bottom of needle
	uint16_t osx = M_SIZE*120, osy = M_SIZE*120; // Saved x & y coords
	uint32_t updateTime = 0;       // time for next update

	int old_analog =  -999; // Value last displayed

	int value[6] = {0, 0, 0, 0, 0, 0};
	int old_value[6] = { -1, -1, -1, -1, -1, -1};
	int d = 0;

	void AnalogMeter();
	void PlotNeedle(int value, byte ms_delay);


public:
	HS_SplashScreen(Queues *newQueues, TFT_eSPI* newTFT, int chipSelectPin);
	~HS_SplashScreen();
	void UpdateScreenOnInterval();
};

