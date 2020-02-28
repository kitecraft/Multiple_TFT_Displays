#pragma once


#define TFT_GREY 0x5AEB

#define NUMBER_OF_TFT_DISPLAYS 3

#define FIRST_SCREEN_CS 15
#define SECOND_SCREEN_CS 21
#define THIRD_SCREEN_CS 22

#define FIRST_SCREEN 0
#define SECOND_SCREEN 1
#define THIRD_SCREEN 2

enum DisplayCommands {
	ChangeScreen = 0,
	UpdateValue,
};

enum ScreenTypes {
	SplashScreen = 0,
	Pong_ScreenType,
	Matrix_ScreenType,
};
