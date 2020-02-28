#pragma once
#include "HS_SplashScreen.h"


HS_SplashScreen* HS_Current_SplashScreen = NULL;

void Create_SplashScreen(Queues *newQueues, TFT_eSPI* TFT, int csPin) {
	HS_Current_SplashScreen = new HS_SplashScreen(newQueues, TFT, csPin);
}

void Destroy_SplashScreen()
{
	if (HS_Current_SplashScreen != NULL)
	{
		delete(HS_Current_SplashScreen);
		HS_Current_SplashScreen = NULL;
	}
}

void Update_SplashScreen_OnInterval()
{
	HS_Current_SplashScreen->UpdateScreenOnInterval();
}