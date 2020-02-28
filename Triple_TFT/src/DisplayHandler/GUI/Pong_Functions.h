#pragma once
#include "Pong.h"


HS_PongScreen* pongScreen = NULL;

void Create_PongScreen(Queues *newQueues, TFT_eSPI* TFT, int csPin) {
	pongScreen = new HS_PongScreen(newQueues, TFT, csPin);
}

void Destroy_PongScreen()
{
	if (pongScreen != NULL)
	{
		delete(pongScreen);
		pongScreen = NULL;
	}
}

void Update_Pong_OnInterval()
{
	pongScreen->UpdateScreenOnInterval();
}