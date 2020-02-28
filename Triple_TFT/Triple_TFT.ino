/*
    This is an example of using 3 ILI9341 based
    TFT displays with Bodmers TFT_ESPI library
    https://github.com/Bodmer/TFT_eSPI


    The screens displayed in this example are pulled from examples
    that come with the TFt_ESPI library. I've just adpated them
    to my DisplayManager as an example.

    The ChipSelect pin for each display in this example
    is defined in the GlobalDefinitions.h file.
    #define LEFT_SCREEN_CS 15
    #define CENTER_SCREEN_CS 21
    #define RIGHT_SCREEN_CS 22    

    In the TFT_ESPI user_setup.h file
    be sure to comment out the line for TFT_CS
//#define TFT_CS   21  // Chip select control pin

    You can still use this sketch with only two displays.
    In the GlobalDefinitions.h file, change NUMBER_OF_TFT_DISPLAYS to 2
#define NUMBER_OF_TFT_DISPLAYS 2

    You won't be able to use third_screen items of course.    
*/


#include "src/DisplayHandler/DisplayHandler.h"
#include "src/QueueItem.h"


TaskHandle_t Display_Core_Task_Handle;

DisplayHandler displayHandler;
Queues allQueues;


void setup() {
	displayHandler.Init(&allQueues);

	xTaskCreatePinnedToCore(
		TFT_Core_Proc,                  /* pvTaskCode */
		"DisplayHandler",            /* pcName */
		3000,                   /* usStackDepth */
		NULL,                   /* pvParameters */
		1,                      /* uxPriority */
		&Display_Core_Task_Handle,                 /* pxCreatedTask */
		0);

    allQueues.AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::Matrix_ScreenType), FIRST_SCREEN);
	allQueues.AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::Pong_ScreenType), SECOND_SCREEN);
	allQueues.AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::SplashScreen), THIRD_SCREEN);
}


void loop() 
{
    delay(20);
}

void TFT_Core_Proc(void* parameter)
{
	displayHandler.Run();
}