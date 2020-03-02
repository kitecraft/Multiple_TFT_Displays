#include "HS_Matrix_Screen.h"

HS_Matrix_Screen::HS_Matrix_Screen(Queues *newQueues, TFT_eSPI* newTFT, int chipSelectPin) : HS_ScreenBase(newQueues, newTFT, chipSelectPin)
{
	screenRotation = 0;

    randomSeed(analogRead(A0));
    PrepDisplayForUpdate();
    
    SetupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);

    CompleteDisplayUpdate();
}

HS_Matrix_Screen::~HS_Matrix_Screen()
{

}


void HS_Matrix_Screen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > updateScreenInterval)
	{
		PrepDisplayForUpdate();
        if(buildingTheDisplay)
        {        
            if (pos[i] > 20) pos[i] -= 4; // Rapid fade initially brightness values
            if (pos[i] > 0) pos[i] -= 1; // Slow fade later
            if ((random(20) == 1) && (j<400)) pos[i] = 63; // ~1 in 20 probability of a new character
            TFT->setTextColor(pos[i] << 5, ILI9341_BLACK); // Set the green character brightness
            if (pos[i] == 63) TFT->setTextColor(ILI9341_WHITE, ILI9341_BLACK); // Draw white character
            xPos += TFT->drawChar(random(32, 128), xPos, yDraw, 1); // Draw the character
            
            i++;
            if(i >= 40)
            {
                i=0;
                j += TEXT_HEIGHT;
                yDraw = Scroll_Slow(TEXT_HEIGHT, 14); // Scroll, 14ms per pixel line
                xPos = 0;
            }
            if(j >= 330)
            {
                buildingTheDisplay = false;
            }
        } else 
        {
            if (millis() - lastUpdate > updateScreenInterval)
            {
                yDraw = Scroll_Slow(1,0); 
                lastUpdate = millis();
            }
        }
		CompleteDisplayUpdate();
	}
}


void HS_Matrix_Screen::SetupScrollArea(uint16_t TFA, uint16_t BFA)
{
    TFT->writecommand(ILI9341_VSCRDEF); // Vertical scroll definition
    TFT->writedata(TFA >> 8);
    TFT->writedata(TFA);
    TFT->writedata((320 - TFA - BFA) >> 8);
    TFT->writedata(320 - TFA - BFA);
    TFT->writedata(BFA >> 8);
    TFT->writedata(BFA);
}

int HS_Matrix_Screen::Scroll_Slow(int lines, int wait) 
{
  int yTemp = yStart;
  for (int i = 0; i < lines; i++) {
    yStart++;
    if (yStart == 320 - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA;
    ScrollAddress(yStart);
    delay(wait);
  }
  return  yTemp;
}

void HS_Matrix_Screen::ScrollAddress(uint16_t VSP) 
{
  TFT->writecommand(ILI9341_VSCRSADD); // Vertical scrolling start address
  TFT->writedata(VSP >> 8);
  TFT->writedata(VSP);
}