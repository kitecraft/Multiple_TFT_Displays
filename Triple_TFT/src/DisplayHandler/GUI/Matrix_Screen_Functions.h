#pragma once
#include "HS_Matrix_Screen.h"


HS_Matrix_Screen* matrixScreen = NULL;

void Create_MatrixScreen(Queues *newQueues, TFT_eSPI* TFT, int csPin) {
	matrixScreen = new HS_Matrix_Screen(newQueues, TFT, csPin);
}

void Destroy_MatrixScreen()
{
	if (matrixScreen != NULL)
	{
		delete(matrixScreen);
		matrixScreen = NULL;
	}
}

void Update_MatrixStream_OnInterval()
{
	matrixScreen->UpdateScreenOnInterval();
}