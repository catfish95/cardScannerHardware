#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>

// Pinbelegung
const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;
const int buzzerPin = 11;
const int ldrPin = A7;
const int buttonResetPin = 3;
const int buttonManualTrigger = 6;
const int micPin = A2;

const int displayClkPin = 4;
const int displayDioPin = 5;

// Initialize display
TM1637Display display(displayClkPin, displayDioPin);

// Initialize Neopixel LED
Adafruit_NeoPixel ledRing(8, A0, NEO_GRB + NEO_KHZ800);

// Variables
int cardCount = 0;
bool motorRunning = false;
bool scannerActive = false;
bool cardInPosition = false;
bool timeoutMode = false;
unsigned long lastButtonPress = 0;
unsigned long lastResetPress = 0;
unsigned long lastCardDetectedTime = 0;
unsigned long lastCardMovedTime = 0;
const unsigned long timeoutDuration = 8000;
const unsigned long cardCountDelay = 1000;
unsigned long lastCardCountTime = 0;
int baseLdrValue;

// Lighting control
const int reasonBrightness = 255;
unsigned long dimmingStartTime = 0;
const unsigned long dimmingDuration = 5000;
bool isDimmed = false;
unsigned long colorFadeTime = 0;
int fadeStep = 0;

//Startup sequence
const int startup = 5;  // Here you can select the desired start sequence (1, 2, 3, 4, 5)

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonResetPin, INPUT_PULLUP);
  pinMode(buttonManualTrigger, INPUT_PULLUP);
  pinMode(ldrPin, INPUT);
  pinMode(micPin, INPUT);

  ledRing.begin();
  setLightRed(reasonBrightness); // Calibrate LDR with red light
  ledRing.show();

  digitalWrite(ENA, HIGH);
  display.setBrightness(0x0f);
  display.showNumberDec(cardCount);

  delay(500);
  baseLdrValue = analogRead(ldrPin);
  Serial.print("Basis LDR-Wert: ");
  Serial.println(baseLdrValue);
  delay(500);

  // Selecting the start sequence
  switch (startup) {
    case 1:
      startupSequence1();
      break;
    case 2:
      startupSequence2();
      break;
    case 3:
      startupSequence3();
      break;
    case 4:
      startupSequence4();
      break;
    case 5:
      startupSequence5();
      break;
    default:
      Serial.println("Ungültige Startsequenz. Standard wird verwendet.");
      startupSequence1();  // Fallback to standard sequence
      break;
  }
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  Serial.println(ldrValue);
  bool soundDetected = digitalRead(micPin) == HIGH;

  // Start button: Switching the scanner
  if (digitalRead(buttonManualTrigger) == LOW) {
    if (millis() - lastButtonPress > 300) {
      scannerActive = !scannerActive;
      lastButtonPress = millis();

      if (scannerActive) {
        setLightRed(255);
        moveCard();
        timeoutMode = false;
        lastCardDetectedTime = millis();
        lastCardMovedTime = millis();
      } else {
        stopMotor();
        timeoutMode = false;
        isDimmed = false;
      }
      ledRing.show();
    }
  }

  // Motor control
  if (!scannerActive && motorRunning) {
    stopMotor();
  }

  // Reset button: Reset counter
  if (digitalRead(buttonResetPin) == LOW && millis() - lastResetPress > 300) {
    cardCount = 0;
    display.showNumberDec(cardCount);
    lastResetPress = millis();
    delay(500);
  }

  // When the scanner is active
  if (scannerActive) {
    if (!cardInPosition && ldrValue < baseLdrValue - 50) {
      cardInPosition = true;
      lastCardDetectedTime = millis();
      //Serial.println(cardInPosition);
      //Serial.println("Card is promoted");
      
    }

    if (cardInPosition && ldrValue > baseLdrValue - 30) {
      stopMotor();
      setLightRed(255);
      delay(50);
      baseLdrValue = analogRead(ldrPin);
      cardInPosition = false;
      //Serial.println(cardInPosition);
      //Serial.println("Card is scanned");
    }
    

    if (millis() - lastCardMovedTime >= 3000 && !cardInPosition && !timeoutMode) {
          isDimmed = true;
    }

    if (soundDetected) {
      isDimmed = false;
      setLightRed(255);
      moveCard();
      if (!timeoutMode && millis() - lastCardCountTime > cardCountDelay) {
        lastCardCountTime = millis();
        cardCount++;
        display.showNumberDec(cardCount);

        //Serial.println(cardInPosition);
      }
      lastCardMovedTime = millis();
    }

    if (millis() - lastCardMovedTime > timeoutDuration && !timeoutMode) {
      timeoutMode = true;
      isDimmed = false;
      stopMotor();
      tone(buzzerPin, 1000, 500);
      //Serial.println("Timeout Mode");
    }
  }

  // Timeout mode flashes
  if (timeoutMode) {
    blinkRedLEDAndSoundBuzzer();
  }

  // Pause mode - lights change smoothly
  if (!scannerActive && !timeoutMode) {
    fadeLights();
    //Serial.println("Pause Mode");
  }

  if (isDimmed) {
    //Serial.println(isDimmed);
    dimLights();
  }
}

void moveCard() {
  if (scannerActive && !timeoutMode) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    motorRunning = true;
  }
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  motorRunning = false;
}

void blinkRedLEDAndSoundBuzzer() {
  static unsigned long lastBlinkTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlinkTime >= 500) {
    lastBlinkTime = currentMillis;
    static bool ledState = false;
    ledState = !ledState;

    if (ledState) {
      setLightRed(255);
      tone(buzzerPin, 1000, 100);
    } else {
      ledRing.clear();
      noTone(buzzerPin);
    }
    ledRing.show();
  }
}

void dimLights() {
  unsigned long elapsedTime = millis() - dimmingStartTime;  // Calculate the elapsed time since the start of the dimming process
  
    int brightness = map(elapsedTime, 0, dimmingDuration, 255, 0);  // Dim brightness from 255 (maximum) to 0 (minimum brightness)

    // Set the LEDs to the calculated brightness
    setLightRed(brightness);  // Reduce the brightness of the LEDs
  //}

  ledRing.show();  // Set the LEDs to the current state
}



void fadeLights() {
  unsigned long currentMillis = millis();
  if (currentMillis - colorFadeTime >= 2000) {  // Colors change every 2 seconds
    colorFadeTime = currentMillis;

    switch (fadeStep % 6) {
      case 0: ledRing.fill(ledRing.Color(255, 0, 0)); break;     // Red
      case 1: ledRing.fill(ledRing.Color(255, 127, 0)); break;   // Orange
      case 2: ledRing.fill(ledRing.Color(255, 255, 0)); break;   // Yellow
      case 3: ledRing.fill(ledRing.Color(0, 255, 0)); break;     // Green
      case 4: ledRing.fill(ledRing.Color(0, 0, 255)); break;     // Blue
      case 5: ledRing.fill(ledRing.Color(75, 0, 130)); break;    // Indigo
    }

    ledRing.show();
    fadeStep++;
  }
}

void setLightRed(int brightness) {
  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(brightness, 0, 0));
  }
  ledRing.show();
}

void startupSequence1() {
  ledRing.clear();
  ledRing.show();
  
  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 0, 0)); 
    ledRing.show();
    tone(buzzerPin, 1000, 100);
    delay(100);
  }
  delay(200);

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(0, 255, 0)); 
    ledRing.show();
    tone(buzzerPin, 1200, 100);
    delay(100);
  }
  delay(200);

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(0, 0, 255)); 
    ledRing.show();
    tone(buzzerPin, 1400, 100);
    delay(100);
  }
  delay(200);

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 255, 255)); 
    ledRing.show();
    tone(buzzerPin, 1600, 100);
    delay(100);
  }
  delay(500);

  ledRing.fill(ledRing.Color(255, 255, 255));
  ledRing.show();
  tone(buzzerPin, 2000, 300);
  delay(300);

  ledRing.clear();
  ledRing.show();
  noTone(buzzerPin);
}

void startupSequence2() {
  ledRing.clear();
  ledRing.show();

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 165, 0));
    ledRing.show();
    tone(buzzerPin, 800 + (i * 50), 100); 
    delay(100);
  }
  delay(300);


  for (int j = 0; j < 3; j++) { 
    for (int brightness = 0; brightness <= 255; brightness += 5) {
      ledRing.fill(ledRing.Color(brightness, brightness / 2, 0)); 
      ledRing.show();
      delay(10);
    }
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
      ledRing.fill(ledRing.Color(brightness, brightness / 2, 0));
      ledRing.show();
      delay(10);
    }
    tone(buzzerPin, 1200, 150); 
    delay(150);
  }

  for (int i = 0; i < 3; i++) {
    ledRing.fill(ledRing.Color(255, 255, 255));  
    ledRing.show();
    tone(buzzerPin, 2000, 100);
    delay(100);

    ledRing.clear();
    ledRing.show();
    noTone(buzzerPin);
    delay(100);
  }
}

void startupSequence3() {
  ledRing.clear();
  ledRing.show();

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 0, 255)); 
    ledRing.show();
    tone(buzzerPin, 800 + (i * 30), 50); 
    delay(50);
  }
  noTone(buzzerPin);
  delay(300);

  int half = ledRing.numPixels() / 2;
  for (int i = 0; i <= half; i++) {
    ledRing.setPixelColor(half - i, ledRing.Color(0, 255, 255)); 
    ledRing.setPixelColor(half + i, ledRing.Color(0, 255, 255));
    ledRing.show();
    tone(buzzerPin, 1000 + (i * 40), 80);
    delay(80);
  }
  noTone(buzzerPin);
  delay(300);

  for (int j = 0; j < 3; j++) {
    ledRing.fill(ledRing.Color(0, 0, 255));
    ledRing.show();
    tone(buzzerPin, 1500, 100);
    delay(200);

    ledRing.fill(ledRing.Color(0, 0, 100));
    ledRing.show();
    delay(200);
  }
  noTone(buzzerPin);
  delay(300);

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 255, 255)); 
    ledRing.setPixelColor((i + ledRing.numPixels() / 2) % ledRing.numPixels(), 0);
    ledRing.show();
    tone(buzzerPin, 2000 + (i * 20), 30);
    delay(30);
  }
  noTone(buzzerPin);
  delay(300);

  ledRing.fill(ledRing.Color(255, 255, 255));
  ledRing.show();
  tone(buzzerPin, 2500, 150);
  delay(150);
  ledRing.clear();
  ledRing.show();
  noTone(buzzerPin);
}


void startupSequence4() {
  Serial.println("Starte Startup-Sequenz 4");
  ledRing.clear();
  ledRing.show();

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 0, 255));
    ledRing.show();
    tone(buzzerPin, 800 + (i * 30), 50); 
    delay(50);
  }
  noTone(buzzerPin);
  delay(300);

  int half = ledRing.numPixels() / 2;
  for (int i = 0; i <= half; i++) {
    ledRing.setPixelColor(half - i, ledRing.Color(0, 255, 255)); 
    ledRing.setPixelColor(half + i, ledRing.Color(0, 255, 255));
    ledRing.show();
    tone(buzzerPin, 1000 + (i * 40), 80);
    delay(80);
  }
  noTone(buzzerPin);
  delay(300);


  for (int j = 0; j < 3; j++) {
    ledRing.fill(ledRing.Color(0, 0, 255)); 
    ledRing.show();
    tone(buzzerPin, 1500, 100);
    delay(200);

    ledRing.fill(ledRing.Color(0, 0, 100));  
    ledRing.show();
    delay(200);
  }
  noTone(buzzerPin);
  delay(300);

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 255, 255)); 
    ledRing.setPixelColor((i + ledRing.numPixels() / 2) % ledRing.numPixels(), 0);
    ledRing.show();
    tone(buzzerPin, 2000 + (i * 20), 30);
    delay(30);
  }
  noTone(buzzerPin);
  delay(300);

  ledRing.fill(ledRing.Color(255, 255, 255));
  ledRing.show();
  tone(buzzerPin, 2500, 150);
  delay(150);
  ledRing.clear();
  ledRing.show();
  noTone(buzzerPin);
}


void startupSequence5() {
  ledRing.clear();
  ledRing.show();

  for (int i = 0; i < ledRing.numPixels(); i++) {
    ledRing.setPixelColor(i, ledRing.Color(0, 0, 255));
    ledRing.show();
    tone(buzzerPin, 800 + (i * 20), 50); 
    delay(50);
    ledRing.setPixelColor(i, 0); 
  }
  noTone(buzzerPin);
  delay(300);

  int half = ledRing.numPixels() / 2;
  for (int i = half; i >= 0; i--) {
    ledRing.setPixelColor(half - i, ledRing.Color(255, 255, 0)); 
    ledRing.setPixelColor(half + i, ledRing.Color(255, 255, 0));  
    ledRing.show();
    tone(buzzerPin, 1000 + (i * 50), 50);
    delay(80);
  }
  noTone(buzzerPin);
  delay(300);

  for (int j = 0; j < 3; j++) {
    ledRing.fill(ledRing.Color(255, 255, 255)); 
    ledRing.show();
    tone(buzzerPin, 1500, 100);
    delay(200);

    ledRing.clear();
    ledRing.show();
    delay(200);
  }
  noTone(buzzerPin);
  delay(300);

  for (int i = 0; i <= half; i++) {
    ledRing.setPixelColor(half - i, 0); 
    ledRing.setPixelColor(half + i, 0);
    ledRing.show();
    tone(buzzerPin, 2000 - (i * 50), 50);  
    delay(80);
  }
  noTone(buzzerPin);
  delay(300);

  for (int i = 0; i < 2; i++) {
    ledRing.fill(ledRing.Color(255, 0, 0)); 
    ledRing.show();
    tone(buzzerPin, 2500, 150);
    delay(150);

    ledRing.clear();
    ledRing.show();
    noTone(buzzerPin);
    delay(150);
  }
}
