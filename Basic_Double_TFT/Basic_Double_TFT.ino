/*
In the TFT_ESPI user_setup.h file
be sure to comment out the line for TFT_CS
//#define TFT_CS   21  // Chip select control pin
*/

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#define firstScreenCS 21
#define secondScreenCS 22
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library


void setup() {
  Serial.begin(115200);
  
  pinMode(firstScreenCS, OUTPUT);
  digitalWrite(firstScreenCS, HIGH);
  
  pinMode(secondScreenCS, OUTPUT);
  digitalWrite(secondScreenCS, HIGH);
  

  // We need to 'init' both displays
  // at the same time. so set both cs pins low
  digitalWrite(firstScreenCS, LOW);
  digitalWrite(secondScreenCS, LOW);
  
  tft.init();
  tft.setTextSize(2);
  
  // Set both cs pins HIGH, or 'inactive'
  digitalWrite(firstScreenCS, HIGH);
  digitalWrite(secondScreenCS, HIGH);



  /*
   Update the first screen
   all 'tft.' call must be done
   after setting the cs pin low
  */
  digitalWrite(firstScreenCS, LOW);
  tft.setRotation(1);  
  tft.fillScreen(TFT_RED);
  tft.setCursor(0,10);
  tft.print("Hello World.\nI'm display #1");
  // set the cs pin back to HIGH when finished
  // with this particular display
  digitalWrite(firstScreenCS, HIGH);


  // Update the second screen
  digitalWrite(secondScreenCS, LOW);
  tft.setRotation(2);  
  tft.fillScreen(TFT_GREEN);
  tft.setCursor(0,10);
  tft.print("Hello World.\nI'm display #2");
  digitalWrite(secondScreenCS, HIGH);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
}
