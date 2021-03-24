#include <LiquidCrystal.h> //addressable LED library

const int trigPin = 10;           //connects to the trigger pin on the distance sensor
const int echoPin = 9;           //connects to the echo pin on the distance sensor

#include "pitches.h"      //library for the soviet union theme

int speakerPin = 6;  //establishing of piezo speaker pin

int melody[] = {
  NOTE_G4, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_C4,
  NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5,
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_G4,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_C4,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_B4, NOTE_C5, NOTE_D5,
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_G4, NOTE_B4, NOTE_C5, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4,
  NOTE_C5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_A4,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_C4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
};
//how long the notes will last

int noteDurations[] = {
  8, 4, 6, 16, 4, 8, 8,
  4, 6, 16, 4, 8, 8,
  4, 8, 8, 4, 8, 8, 4, 8, 8, 2,
  4, 6, 16, 4, 8, 8,
  4, 6, 16, 4, 8, 8,
  4, 6, 16, 4, 6, 16,
  4, 6, 16, 8, 8, 8, 8,
  2, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8,
  2, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8,
  4, 6, 16, 4, 6, 16, 4, 8, 8, 2,
  2, 8, 8, 8, 8, 3, 8, 2,
  2, 8, 8, 8, 8, 3, 8, 2,
  4, 6, 16, 4, 4, 2, 4, 4, 1
};

const int buttonPin = 1;  //pin to control the blue LED inside the RGB LED

//arduino 9 goes to pin 4 from LCD (pin 12 in video)
//pin 6 is going to arduino 7  (pin 11 in video)
// pin 11 goes to arduini 0 (5 in the video)
// pin 12 goes to 1 (4 in the video)
// pin 13 goes to 2 (3 in the video)
// pin 14 goes to 3 (2 in the video)

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);   //liquid crystal display pins


int buttonStatus = 0;
boolean gameStarted = false;   //boolean that helps to set the up how the "idle state" of the game works and the other state
int score = 0;
int tries = 0;

float distance = 0;               //stores the distance measured by the distance sensor
// set up for lights

#include <FastLED.h>
#define BRIGHTNESS  255
#define LED_PIN     8
#define NUM_LEDS    115
const int elay = 3;
const int lights = 115;
CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin (9600);        //set up a serial connection with the computer
  pinMode(speakerPin, OUTPUT);


  pinMode(trigPin, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

  //set the RGB LED pins to output

  pinMode(buttonPin, INPUT_PULLUP);

  //setup for lights

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS)
  .setCorrection(TypicalLEDStrip)
  .setDither(BRIGHTNESS < 255);
  FastLED.setBrightness(BRIGHTNESS);
  int count;
  count = 0;
}

void loop() {
  while (gameStarted == false) {

    for (int i = 0; i <= 7; i++)//for loop that runs through a series of different animations that play during the idle state
    {
      if (i == 6)
      {
        i = 0;
      }
      else
      {                        //sets up the loop that initializes the game after the button is held for some time

        runLights(i);
        buttonStatus = digitalRead(buttonPin);
        lcd.begin(16, 2);
        lcd.print("Welcome!");
        if (buttonStatus == LOW) {
          for (int i = 0; i <= lights; i++)
          {
            leds[i] = CRGB (0, 0, 0);
            FastLED.show();
            delay(1);

            leds[i] = CRGB (0, 0, 0);
            FastLED.show();
            delay(1);
          }
          gameStarted = true;
          tone(speakerPin, 392, 256);
          delay(175);
          tone(speakerPin, 262, 256);
          delay(250);
          break;
        }
      }
    }
  }

  while (gameStarted == true && tries < 3) {
   //different loops that correspond to different animations 
   //set up using distance thresholds
    
    //blue
    distance = getDistance();   //variable to store the distance measured by the sensor
    Serial.print(distance);     //print the distance that was measured
    Serial.println(" in");
    if (distance <= 4)
    {
      tone(speakerPin, 392, 256);
      delay(125);
      tone(speakerPin, 262, 256);
      delay(125);
      tone(speakerPin, 330, 256);
      delay(125);
      tone(speakerPin, 440, 256);
      delay(250);
      for (int i = 0; i <= 1; i++) {
        for (int i = 0; i <= lights; i++)
        {
          leds[i] = CRGB (0, 0, 144);
          FastLED.show();
          delay(1);

          leds[i] = CRGB (0, 0, 0);
          FastLED.show();
          delay(1);
        }
        gameStarted = true;
      }
      score = score + 10; //keeps track of the score repeated throughout all the thresholds
      lcd.clear();
      lcd.print("Score: ");
      lcd.print(score);
      tries++;
      Serial.print(tries);
    }
    //yellow
    else if (4 < distance && distance <= 8)
    { //if the object is a medium distance
      tone(speakerPin, 392, 256);
      delay(125);
      tone(speakerPin, 262, 256);
      delay(125);
      tone(speakerPin, 330, 256);
      delay(125);
      tone(speakerPin, 440, 256);
      delay(250);

      for (int i = 0; i <= 1; i++) {
        for (int i = 0; i <= lights; i++)
        {
          leds[i] = CRGB (144, 144, 0);
          FastLED.show();
          delay(1);

          leds[i] = CRGB (0, 0, 0);
          FastLED.show();
          delay(1);
        }
        gameStarted = true;
      }
      score = score + 25;
      lcd.clear();
      lcd.print("Score: ");
      lcd.print(score);
      tries++;
      Serial.print(tries);
    }
    //cyan
    else if (8 < distance && distance <= 12)
    {
      tone(speakerPin, 392, 256);
      delay(125);
      tone(speakerPin, 262, 256);
      delay(125);
      tone(speakerPin, 330, 256);
      delay(125);
      tone(speakerPin, 440, 256);
      delay(250);

      for (int i = 0; i <= 1; i++) {
        for (int i = 0; i <= lights; i++)
        {
          leds[i] = CRGB (14, 56, 27);
          FastLED.show();
          delay(1);

          leds[i] = CRGB (0, 0, 0);
          FastLED.show();
          delay(1);
        }
        gameStarted = true;
      }
      score = score + 50;
      lcd.clear();
      lcd.print("Score: ");
      lcd.print(score);
      tries++;
      Serial.print(tries);
    }
    //purple
    else if (12 < distance && distance <= 16)
    {
      tone(speakerPin, 392, 256);
      delay(125);
      tone(speakerPin, 262, 256);
      delay(125);
      tone(speakerPin, 330, 256);
      delay(125);
      tone(speakerPin, 440, 256);
      delay(250);

      for (int i = 0; i <= 1; i++) {
        for (int i = 0; i <= lights; i++)
        {
          leds[i] = CRGB (144, 0, 144);
          FastLED.show();
          delay(1);

          leds[i] = CRGB (0, 0, 0);
          FastLED.show();
          delay(1);
        }
        gameStarted = true;
      }
      score = score + 25;
      lcd.clear();
      lcd.print("Score: ");
      lcd.print(score);
      tries++;
      Serial.print(tries);
    }
    //red
    else if (16 < distance && distance <= 19)
    {
      tone(speakerPin, 392, 256);
      delay(125);
      tone(speakerPin, 262, 256);
      delay(125);
      tone(speakerPin, 330, 256);
      delay(125);
      tone(speakerPin, 440, 256);
      delay(250);

      for (int i = 0; i <= 1; i++) {
        for (int i = 0; i <= lights; i++)
        {
          leds[i] = CRGB (144, 34, 21);
          FastLED.show();
          delay(1);

          leds[i] = CRGB (0, 0, 0);
          FastLED.show();
          delay(1);
        }
        gameStarted = true;
      }
      score = score + 10;
      lcd.clear();
      lcd.print("Score: ");
      lcd.print(score);
      tries++;
      Serial.print(tries);
    }
  }
  delay(25);      //delay 50ms between each reading

  if (tries = 3) {
    if (score > 30) {
      lcd.clear();
      lcd.print("Your score: ");
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print("You win! :)");


      for (int thisNote = 0; thisNote < sizeof(melody) / 2; thisNote++)
      {
        int noteDuration = 2000 / noteDurations[thisNote];
        tone(speakerPin, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(speakerPin);

        pride();
        FastLED.show();
      }




    } else {
      lcd.clear();
      lcd.print("Your score: ");
      lcd.print(score);
      lcd.setCursor(0, 1);
      lcd.print("You lose :(");
    }
  }
}

//------------------FUNCTIONS-------------------------------

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
  //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}

int runLights(int count)
{
  if (count == 0)
  {
    for (int i = 0; i <= lights; i++)
    {


      leds[i] = CRGB (0, 0, 144);
      FastLED.show();
      delay(elay);
      buttonStatus = digitalRead(buttonPin);
      if (buttonStatus == LOW) {
        gameStarted = true;
      }
    }
  }

  if (count == 1)
  {
    for (int i = lights; i >= 0; i--)
    {
      leds[i] = CRGB (13, 0, 55);
      FastLED.show();
      delay(elay);
      buttonStatus = digitalRead(buttonPin);
      if (buttonStatus == LOW) {
        gameStarted = true;
      }
    }
  }

  if (count == 2)
  {
    for (int i = 0; i <= lights; i++)
    {
      leds[i] = CRGB (0, 11, 15);
      FastLED.show();
      delay(elay);
      buttonStatus = digitalRead(buttonPin);
      if (buttonStatus == LOW) {
        gameStarted = true;
      }
    }
  }

  if (count == 3)
  {
    for (int i = lights; i >= 0; i--)
    {
      leds[i] = CRGB (12, 115, 66);
      FastLED.show();
      delay(elay);
      buttonStatus = digitalRead(buttonPin);
      if (buttonStatus == LOW) {
        gameStarted = true;
      }
    }
  }

  if (count == 4)
  {
    for (int i = 0; i <= lights; i++)
    {
      leds[i] = CRGB (22, 112, 15);
      FastLED.show();
      delay(elay);
      buttonStatus = digitalRead(buttonPin);
      if (buttonStatus == LOW) {
        gameStarted = true;
      }
    }
  }
  if (count == 5 )
  {
    for (int i = lights; i >= 0; i--)
    {
      leds[i] = CRGB (135, 11, 145);
      FastLED.show();
      delay(elay);
      buttonStatus = digitalRead(buttonPin);
      if (buttonStatus == LOW) {
        gameStarted = true;
      }
    }
  }
}
//program for the Winning animation
void pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend( leds[pixelnumber], newcolor, 64);
  }
}
