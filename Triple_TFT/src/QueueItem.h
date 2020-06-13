#pragma once
#include <Arduino.h>
#include <ArduinoQueue.h>
#include "freertos/portmacro.h"


struct DISPLAY_QUEUE_ITEM {
	char key;
	String value;
	int screen;
};

class Queues
{
private:

public:
	Queues();
	~Queues();
	ArduinoQueue<DISPLAY_QUEUE_ITEM> displayQueue;

	portMUX_TYPE displayQueueMux;

	void AddItemToDisplayQueue(char key, String value, int screenID);
};