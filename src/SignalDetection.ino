#include "DHT.h"

//#include <SeeedOLED.h>
//#include <Wire.h>
#include <U8x8lib.h>
#include <Volume.h>

#define DHTTYPE DHT11
#define DHTPIN 3
#define BUZZERPIN 5
#define LEDPIN 4

Volume vol;
U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(U8X8_PIN_NONE);
volatile boolean buttonPressed = false;
volatile long lastPressed = 0;
int volDown = -1;
int volUp = -1;
int toneVal = 3000;

int numTrials = 21;

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  attachInterrupt(digitalPinToInterrupt(2), handleInterrupt, RISING);
  pinMode(LEDPIN,OUTPUT);
  vol.begin();
  Serial.begin(9600);
  //Wire.begin();
  //SeeedOled.init();
  //SeeedOled.setNormalDisplay();
  //SeeedOled.setPageMode();
  //SeeedOled.clearDisplay();
  //SeeedOled.setTextXY(0,0);
  //SeeedOled.putString("hi");
  //SeeedOled.setTextXY(5,5);
  pinMode(BUZZERPIN, OUTPUT);
  dht.begin();
  u8x8.begin();
  u8x8.setFlipMode(1);
  u8x8.setCursor(0,0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  randomSeed(analogRead(0)*analogRead(1)/2);
}

void loop() {
  Serial.println("Hello, thank you for participating in my experiment.");
  Serial.println("We will start by calibrating the buzzer volume.");
  Serial.println("Press the button to start calibration");
  waitForButton();
  getBuzzerThreshold();
  buttonPressed = false;
  int avgVol = (volDown + volUp)/2;
  runExperiment(avgVol);
  while(true){
    vol.delay(10);
  }

  //u8x8.setCursor(5,5);
  //float val = dht.readTemperature(true);
  //u8x8.refreshDisplay();
  //u8x8.print(val);
  //SeeedOled.putFloat(val);
  //delay(00);
  //SeeedOled.setTextXY(0,0);
  // put your main code here, to run repeatedly:

}

void runExperiment(int volume) {
  Serial.println("Awesome, now that the device is calibrated, let's get to the Experiment!!");
  Serial.println("Press the button to start the experiment");
  waitForButton();
  for (int i=5; i >= 0; i--) {
    Serial.println(i);
    vol.delay(1000);
  }
  for (int i=0; i < numTrials; i++) {
    //Serial.println(i);
    runTrial(volume);
  }
  Serial.println("Thank you for participating in my experiment");
  Serial.println("End Experiment");
}

void runTrial(int volume) {
  long rn = random(2);
  digitalWrite(LEDPIN,HIGH);
  if (rn) {
    beep(1,volume);
  } else {
    vol.delay(400);
  }
  vol.delay(500);
  digitalWrite(LEDPIN,LOW);
  vol.delay(100);
  analyzeResult(rn);
  vol.delay(500);
}

void analyzeResult(int rn) {
  if (buttonPressed && rn) {
    Serial.println("h");
  } else if (buttonPressed && !rn) {
    Serial.println("fa");
  } else if (!buttonPressed && rn) {
    Serial.println("m");
  } else {
    Serial.println("cr");
  }
  buttonPressed = false;
}

void waitForButton() {
  while(!buttonPressed) {
    vol.delay(10);
  }
  vol.delay(500);
  buttonPressed = false;
  vol.delay(500);
}

void getBuzzerThreshold() {
  getVolDown();
  vol.delay(1000);
  getVolUp();
}

void getVolDown() {
  while (volDown == -1) {
    Serial.println("Instructions:");
    Serial.println("\tOnce you start, you will hear 3 beeps and a buzzer will gradually lower in volume.");
    Serial.println("\tPress the button when you can no longer hear the buzzer.");
    Serial.println("\tPress the button to begin");
    waitForButton();
    beep(3, 125);
    vol.delay(500);
    for (int i=255; i > 0; i--) {
      vol.tone(toneVal,i);
      vol.delay(50);
      if (buttonPressed) {
        volDown = i;
        buttonPressed = false;
        break;
      }
    }
    vol.tone(toneVal,0);
  }
}

void getVolUp() {
  while (volUp == -1) {
    Serial.println("Instructions:");
    Serial.println("\tOnce you start, you will hear 3 beeps and a buzzer will gradually increase in volume.");
    Serial.println("\tPress the button once you hear the buzzer.");
    Serial.println("\tPress the button to begin");
    waitForButton();
    beep(3, 125);
    vol.delay(500);
    for (int i=0; i <= 255; i++) {
      vol.tone(toneVal,i);
      vol.delay(100);
      if (buttonPressed) {
        volUp = i;
        buttonPressed = false;
        break;
      }
    }
    vol.tone(toneVal,0);
  }
}

void beep(int numTimes, int toneLevel) {
  for (int i=0; i<numTimes; i++) {
    vol.tone(toneVal,toneLevel);
    vol.delay(200);
    vol.tone(toneVal,0);
    vol.delay(200);
  }
}

void handleInterrupt() {
  if (vol.millis() > (lastPressed+100)) {
    buttonPressed = true;
  }
}
