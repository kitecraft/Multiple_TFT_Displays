#include "HS_SplashScreen.h"

HS_SplashScreen::HS_SplashScreen(Queues *newQueues, TFT_eSPI* newTFT, int chipSelectPin) : HS_ScreenBase(newQueues, newTFT, chipSelectPin)
{
	screenRotation = 1;

	PrepDisplayForUpdate();
	AnalogMeter();
	CompleteDisplayUpdate();

  	updateTime = millis(); // Next update time
}

HS_SplashScreen::~HS_SplashScreen()
{

}
void HS_SplashScreen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > updateScreenInterval)
	{
		PrepDisplayForUpdate();

		// Create a Sine wave for testing
		d += 4; if (d >= 360) d = 0;
		value[0] = 50 + 50 * sin((d + 0) * 0.0174532925);
	
		PlotNeedle(value[0], 0); // It takes between 2 and 12ms to replot the needle with zero delay

		CompleteDisplayUpdate();

		lastUpdate = millis();
	}
}

void HS_SplashScreen::AnalogMeter()
{
	// Meter outline
	TFT->fillRect(0, 0, M_SIZE*239, M_SIZE*126, TFT_GREY);
	
	//TFT->fillRect(5, 3, M_SIZE*230, M_SIZE*119, TFT_BLUE);
	TFT->fillRect(5, 3, M_SIZE*230, M_SIZE*119, TFT_WHITE);

	TFT->setTextColor(TFT_BLACK);  // Text colour

	// Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
	for (int i = -50; i < 51; i += 5) 
	{
		// Long scale tick length
		int tl = 15;

		// Coodinates of tick to draw
		float sx = cos((i - 90) * 0.0174532925);
		float sy = sin((i - 90) * 0.0174532925);
		uint16_t x0 = sx * (M_SIZE*100 + tl) + M_SIZE*120;
		uint16_t y0 = sy * (M_SIZE*100 + tl) + M_SIZE*140;
		uint16_t x1 = sx * M_SIZE*100 + M_SIZE*120;
		uint16_t y1 = sy * M_SIZE*100 + M_SIZE*140;

		// Coordinates of next tick for zone fill
		float sx2 = cos((i + 5 - 90) * 0.0174532925);
		float sy2 = sin((i + 5 - 90) * 0.0174532925);
		int x2 = sx2 * (M_SIZE*100 + tl) + M_SIZE*120;
		int y2 = sy2 * (M_SIZE*100 + tl) + M_SIZE*140;
		int x3 = sx2 * M_SIZE*100 + M_SIZE*120;
		int y3 = sy2 * M_SIZE*100 + M_SIZE*140;

		// Yellow zone limits
		//if (i >= -50 && i < 0) {
		//  TFT->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
		//  TFT->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
		//}

		// Green zone limits
		if (i >= 0 && i < 25) {
			TFT->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREEN);
			TFT->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN);
		}

		// Orange zone limits
		if (i >= 25 && i < 50) {
			TFT->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_ORANGE);
			TFT->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_ORANGE);
		}

		// Short scale tick length
		if (i % 25 != 0) tl = 8;

		// Recalculate coords incase tick lenght changed
		x0 = sx * (M_SIZE*100 + tl) + M_SIZE*120;
		y0 = sy * (M_SIZE*100 + tl) + M_SIZE*140;
		x1 = sx * M_SIZE*100 + M_SIZE*120;
		y1 = sy * M_SIZE*100 + M_SIZE*140;

		// Draw tick
		TFT->drawLine(x0, y0, x1, y1, TFT_BLACK);

		// Check if labels should be drawn, with position tweaks
		if (i % 25 == 0) {
			// Calculate label positions
			x0 = sx * (M_SIZE*100 + tl + 10) + M_SIZE*120;
			y0 = sy * (M_SIZE*100 + tl + 10) + M_SIZE*140;
			switch (i / 25) {
				case -2: TFT->drawCentreString("0", x0, y0 - 12, 2); break;
				case -1: TFT->drawCentreString("25", x0, y0 - 9, 2); break;
				case 0: TFT->drawCentreString("50", x0, y0 - 7, 2); break;
				case 1: TFT->drawCentreString("75", x0, y0 - 9, 2); break;
				case 2: TFT->drawCentreString("100", x0, y0 - 12, 2); break;
			}
		}

		// Now draw the arc of the scale
		sx = cos((i + 5 - 90) * 0.0174532925);
		sy = sin((i + 5 - 90) * 0.0174532925);
		x0 = sx * M_SIZE*100 + M_SIZE*120;
		y0 = sy * M_SIZE*100 + M_SIZE*140;
		// Draw scale arc, don't draw the last part
		if (i < 50) TFT->drawLine(x0, y0, x1, y1, TFT_BLACK);
	}

	TFT->drawString("%RH", M_SIZE*(5 + 230 - 40), M_SIZE*(119 - 20), 2); // Units at bottom right
	TFT->drawCentreString("%RH", M_SIZE*120, M_SIZE*70, 4); // Comment out to avoid font 4
	TFT->drawRect(5, 3, M_SIZE*230, M_SIZE*119, TFT_BLACK); // Draw bezel line

	PlotNeedle(0, 0); // Put meter needle at 0

}

void HS_SplashScreen::PlotNeedle(int value, byte ms_delay)
{
	TFT->setTextColor(TFT_BLACK, TFT_WHITE);
	char buf[8]; dtostrf(value, 4, 0, buf);
	TFT->drawRightString(buf, M_SIZE*40, M_SIZE*(119 - 20), 2);
	if (value < -10) 
	{
		value = -10; // Limit value to emulate needle end stops
	}
	if (value > 110)
	{
		value = 110;	
	} 

	// Move the needle until new value reached
	while (!(value == old_analog)) 
	{
		if (old_analog < value)
		{
			old_analog++;
		}
		else
		{
			old_analog--;
		}

		if (ms_delay == 0)
		{
			old_analog = value; // Update immediately if delay is 0
		}

		float sdeg = map(old_analog, -10, 110, -150, -30); // Map value to angle
		// Calcualte tip of needle coords
		float sx = cos(sdeg * 0.0174532925);
		float sy = sin(sdeg * 0.0174532925);

		// Calculate x delta of needle start (does not start at pivot point)
		float tx = tan((sdeg + 90) * 0.0174532925);

		// Erase old needle image
		//TFT->drawLine(M_SIZE*(120 + 20 * ltx - 1), M_SIZE*(140 - 20), osx - 1, osy, TFT_BLUE);
		//TFT->drawLine(M_SIZE*(120 + 20 * ltx), M_SIZE*(140 - 20), osx, osy, TFT_BLUE);
		//TFT->drawLine(M_SIZE*(120 + 20 * ltx + 1), M_SIZE*(140 - 20), osx + 1, osy, TFT_BLUE);
		TFT->drawLine(M_SIZE*(120 + 20 * ltx - 1), M_SIZE*(140 - 20), osx - 1, osy, TFT_WHITE);
		TFT->drawLine(M_SIZE*(120 + 20 * ltx), M_SIZE*(140 - 20), osx, osy, TFT_WHITE);
		TFT->drawLine(M_SIZE*(120 + 20 * ltx + 1), M_SIZE*(140 - 20), osx + 1, osy, TFT_WHITE);

		// Re-plot text under needle
		TFT->setTextColor(TFT_BLACK);
		TFT->drawCentreString("%RH", M_SIZE*120, M_SIZE*70, 4); // // Comment out to avoid font 4

		// Store new needle end coords for next erase
		ltx = tx;
		osx = M_SIZE*(sx * 98 + 120);
		osy = M_SIZE*(sy * 98 + 140);

		// Draw the needle in the new postion, magenta makes needle a bit bolder
		// draws 3 lines to thicken needle
		TFT->drawLine(M_SIZE*(120 + 20 * ltx - 1), M_SIZE*(140 - 20), osx - 1, osy, TFT_RED);
		TFT->drawLine(M_SIZE*(120 + 20 * ltx), M_SIZE*(140 - 20), osx, osy, TFT_MAGENTA);
		TFT->drawLine(M_SIZE*(120 + 20 * ltx + 1), M_SIZE*(140 - 20), osx + 1, osy, TFT_RED);

		// Slow needle down slightly as it approaches new postion
		if (abs(old_analog - value) < 10)
		{
			ms_delay += ms_delay / 5;
		}
		// Wait before next update
		delay(ms_delay);
	}
}

