#include "HS_ScreenBase.h"

HS_ScreenBase::HS_ScreenBase(Queues *newQueues, TFT_eSPI *newTFT, int chipSelectPin)
{
	TFT = newTFT;
	allQueues = newQueues;
	cs_pin = chipSelectPin;

}

HS_ScreenBase::~HS_ScreenBase()
{
	TFT = NULL;
}

void HS_ScreenBase::PrepDisplayForUpdate()
{
	digitalWrite(cs_pin,LOW);
	TFT->setRotation(screenRotation);
}

void HS_ScreenBase::CompleteDisplayUpdate()
{
	digitalWrite(cs_pin,HIGH);
}
