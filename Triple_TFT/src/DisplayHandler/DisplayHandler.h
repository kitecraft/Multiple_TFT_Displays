#pragma once
#include "Arduino.h"
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../GlobalDefinitions.h"
#include "../QueueItem.h"


struct ScreenDefinition
{
	int cs_pin;
	int screenRotation;
	void(*UpdateCurentScreen)(String value) = NULL;
	void(*UpdateCurentScreenOnInterval)() = NULL;
	void(*DestroyCurrentScreen)() = NULL;
};

class DisplayHandler
{
private:
	TFT_eSPI tftDisplay;
	ScreenDefinition screenPositions[NUMBER_OF_TFT_DISPLAYS];
	Queues *allQueues;

	void DispatchCommand();
	void LoadNewScreen(ScreenTypes newScreenType, int screenLocation);
	
public:
	DisplayHandler();
	~DisplayHandler();
	void Init(Queues *newQueues);
	void Run();

};

