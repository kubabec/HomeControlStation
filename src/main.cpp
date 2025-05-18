#include <Arduino.h>
#include <os/HomeStation_os.hpp>

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  OperatingSystem::init();

}

void loop() {
  static long long task10msCounter = 0;
  static long long task20msCounter = 0;
  static long long task50msCounter = 0;
  static long long task1sCounter = 0;

  if(abs(millis() - task10msCounter) > 10)
  {
    OperatingSystem::task10ms();
    task10msCounter = millis();
  }

  if(abs(millis() - task20msCounter) > 20)
  {
    OperatingSystem::task20ms();
    task20msCounter = millis();
  }

  if(abs(millis() - task50msCounter) > 50)
  {
    OperatingSystem::task50ms();
    task50msCounter = millis();
  }  

  if(abs(millis() - task1sCounter) > 1000)
  {
    OperatingSystem::task1s();
    task1sCounter = millis();
  }  
}