#include "DisplayHandler.h"
#include "FS.h"
#include "SPIFFS.h"

#include "GUI//SplashScreen_Functions.h"
#include "GUI//Pong_Functions.h"
#include "GUI//Matrix_Screen_Functions.h"

DisplayHandler::DisplayHandler()
{
	screenPositions[FIRST_SCREEN].cs_pin = FIRST_SCREEN_CS;
	screenPositions[SECOND_SCREEN].cs_pin = SECOND_SCREEN_CS;
	screenPositions[THIRD_SCREEN].cs_pin = THIRD_SCREEN_CS;
}

DisplayHandler::~DisplayHandler()
{

}

void DisplayHandler::Init(Queues *newQueues)
{
	allQueues = newQueues;
	int displayLoopCounter;

	// Setup the Chip Select pins for the displays
	for(int displayLoopCounter = 0; displayLoopCounter < NUMBER_OF_TFT_DISPLAYS; displayLoopCounter++)
	{
		pinMode(screenPositions[displayLoopCounter].cs_pin, OUTPUT);
		digitalWrite(screenPositions[displayLoopCounter].cs_pin, LOW);
	}

	tftDisplay.init();
	tftDisplay.setRotation(0);
	tftDisplay.fillScreen(TFT_BLACK);

	for(int displayLoopCounter = 0; displayLoopCounter < NUMBER_OF_TFT_DISPLAYS; displayLoopCounter++)
	{
		digitalWrite(screenPositions[displayLoopCounter].cs_pin, HIGH);
	}
	
}

void DisplayHandler::Run()
{
	int counter = 0;
	while (true)
	{
		DispatchCommand();
		for(counter = 0; counter < NUMBER_OF_TFT_DISPLAYS; counter++)
		{
			if (screenPositions[counter].UpdateCurentScreenOnInterval != NULL)
			{
				screenPositions[counter].UpdateCurentScreenOnInterval();
			}
		}
    	vTaskDelay(pdMS_TO_TICKS(1));
	}
}

void DisplayHandler::LoadNewScreen(ScreenTypes newScreenType, int screenLocation)
{
	if(screenPositions[screenLocation].DestroyCurrentScreen != NULL){
		screenPositions[screenLocation].DestroyCurrentScreen();
		screenPositions[screenLocation].DestroyCurrentScreen = NULL;
	}

	screenPositions[screenLocation].UpdateCurentScreen = NULL;
	screenPositions[screenLocation].UpdateCurentScreenOnInterval = NULL;

	switch (newScreenType) {
	case ScreenTypes::SplashScreen:
		screenPositions[screenLocation].DestroyCurrentScreen = Destroy_SplashScreen;
		screenPositions[screenLocation].UpdateCurentScreenOnInterval = Update_SplashScreen_OnInterval;
		Create_SplashScreen(allQueues, &tftDisplay, screenPositions[screenLocation].cs_pin);
		break;
	case ScreenTypes::Pong_ScreenType:
		screenPositions[screenLocation].DestroyCurrentScreen = Destroy_PongScreen;
		screenPositions[screenLocation].UpdateCurentScreenOnInterval = Update_Pong_OnInterval;
		Create_PongScreen(allQueues, &tftDisplay, screenPositions[screenLocation].cs_pin);
		break;
	case ScreenTypes::Matrix_ScreenType:
		screenPositions[screenLocation].DestroyCurrentScreen = Destroy_MatrixScreen;
		screenPositions[screenLocation].UpdateCurentScreenOnInterval = Update_MatrixStream_OnInterval;
		Create_MatrixScreen(allQueues, &tftDisplay, screenPositions[screenLocation].cs_pin);
		break;
	default:
		break;
	}
}

void DisplayHandler::DispatchCommand()
{
	while (!allQueues->displayQueue.isEmpty())
	{
		portENTER_CRITICAL(&allQueues->displayQueueMux);
		DISPLAY_QUEUE_ITEM currItem = allQueues->displayQueue.dequeue();
		portEXIT_CRITICAL(&allQueues->displayQueueMux);

		switch (currItem.key) {
		case DisplayCommands::ChangeScreen:
			if (currItem.value.length() == 1) 
			{
				LoadNewScreen(static_cast<ScreenTypes>(currItem.value.toInt()),currItem.screen);
			}			
			break;
		case DisplayCommands::UpdateValue:
			if (screenPositions[currItem.screen].UpdateCurentScreen != NULL && currItem.value.length() >= 3) 
			{
				screenPositions[currItem.screen].UpdateCurentScreen(String(currItem.value));
			}
			break;
		default:
			break;
		}
		yield();
	}
}
