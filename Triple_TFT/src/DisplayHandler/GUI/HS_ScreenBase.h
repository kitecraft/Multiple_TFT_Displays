#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../../GlobalDefinitions.h"
#include "../../QueueItem.h"


class HS_ScreenBase
{
private:
	int cs_pin;

public:
	HS_ScreenBase(Queues *newQueues, TFT_eSPI *newTFT, int chipSelectPin);
	~HS_ScreenBase();
	void PrepDisplayForUpdate();
	void CompleteDisplayUpdate();
	int screenRotation = 0;

	TFT_eSPI *TFT;
	Queues *allQueues;
};

