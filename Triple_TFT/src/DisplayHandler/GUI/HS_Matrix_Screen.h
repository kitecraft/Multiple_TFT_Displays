#pragma once

/*
	Based on, and mostly copied from, the tft_espi example called:
	TFT_Matrix
*/

#include "HS_ScreenBase.h"


#define TEXT_HEIGHT 8 // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0  // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 0  // Number of lines in top fixed area (lines counted from top of screen)

class HS_Matrix_Screen :
	public HS_ScreenBase
{
private:
	unsigned long lastUpdate = 0;
	int updateScreenInterval = 5;

	uint16_t yStart = TOP_FIXED_AREA;
	uint16_t yArea = 320 - TOP_FIXED_AREA - BOT_FIXED_AREA;
	uint16_t yDraw = 320 - BOT_FIXED_AREA - TEXT_HEIGHT;
	byte     pos[42];
	uint16_t xPos = 0;

	int j = 0;
	int i = 0;

	bool buildingTheDisplay = true;

	void SetupScrollArea(uint16_t TFA, uint16_t BFA);
	void ScrollAddress(uint16_t VSP);
	int Scroll_Slow(int lines, int wait);

public:
	HS_Matrix_Screen(Queues *newQueues, TFT_eSPI* newTFT, int chipSelectPin);
	~HS_Matrix_Screen();
	void UpdateScreenOnInterval();
};