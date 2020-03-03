/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.ls
  

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define LED 2

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Jrvtb2luTCSBOEx_b83vEnG40dz8tl_M";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "EE-IOT-Platform-02";
char pass[32] = "g!TyA>hR2JTy";

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;

int LEDstate = 0;
int dimval = 0;

int timerCount = 0;
String content = "";

BlynkTimer timer;

void myTimerEvent(){

  if(timerCount == 100){
    Blynk.virtualWrite(V2, millis()/1000);
    timerCount = 0;
  }
  else{
    char character;
    while(Serial.available()){
      character = Serial.read();
      content.concat(character);
    }
    if(content != ""){
      Blynk.virtualWrite(V3, content);
      content = "";
    }
  }
  timerCount++;
}

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);

  timer.setInterval(10L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V0){        //turning LED on and off with button
  int pinValue = param.asInt();
  if(pinValue == 0){
    ledcWrite(ledChannel, 0);
    LEDstate = 0;
  }
  else{
    ledcWrite(ledChannel, dimval);
    LEDstate = 1;
  }
}

BLYNK_WRITE(V1){        //dimming LED with switch
  dimval = param.asInt();
  if(LEDstate == 1){
    ledcWrite(ledChannel, dimval);
  }
}
