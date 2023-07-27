#include "Arduino.h"
#include "SPI.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

#define MINIMUM_FIRMWARE_VERSION "0.6.6"
#define BUTTON_PIN 5

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper *err)
{
  while (1)
    ;
}

void setup(void)
{
  delay(500);

  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(115200);

  if (!ble.begin(true))
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  ble.echo(true);
  ble.verbose(false);

  if (!ble.sendCommandCheckOK(F("AT+GAPDEVNAME=Bluefruit Keyboard")))
  {
    error(F("Could not set device name?"));
  }

  if (ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION))
  {
    if (!ble.sendCommandCheckOK(F("AT+BleHIDEn=On")))
    {
      error(F("Could not enable Keyboard"));
    }
  }
  else
  {
    if (!ble.sendCommandCheckOK(F("AT+BleKeyboardEn=On")))
    {
      error(F("Could not enable Keyboard"));
    }
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (!ble.reset())
  {
    error(F("Couldn't reset??"));
  }

  Serial.println("Ready");

  // Display prompt
  Serial.println("");
  Serial.print(F("keyboard > "));
}


void listenForButton(void) {
  if (digitalRead(BUTTON_PIN))
  {
    Serial.println("BUTTON PRESSED");
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)

    // Here's the docs about keys: https://learn.adafruit.com/introducing-adafruit-ble-bluetooth-low-energy-friend/ble-services
    // You have sets of seven hexidecimal numbers. The first is modifiers, the rest are
    // key presses. Modifiers can be added together by adding the numbers.

    // iOS
    ble.println("AT+BleKeyboardCode=06-00-04-00-00-00-00"); // shift + alt + a
    ble.println("AT+BleKeyboardCode=00-00");
    // Windows
    ble.println("AT+BleKeyboardCode=04-00-04-00-00-00-00"); // alt + a
    ble.println("AT+BleKeyboardCode=00-00");

    if (!ble.waitForOK())
    {
      Serial.println(F("Handling button press FAILED!"));
    }
    delay(500);

  } else {
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  }
}


void getUserInput(char buffer[], uint8_t maxSize)
{
  memset(buffer, 0, maxSize);
  while( Serial.available() == 0 ) {
    listenForButton();
    delay(1);
  }

  uint8_t count=0;

  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    Serial.print(F(","));
    delay(1);
  } while( (count < maxSize) && !(Serial.available() == 0) );
}

void loop(void)
{
  // Check for user input and echo it back if anything was found
  char keys[BUFSIZE+1];
  getUserInput(keys, BUFSIZE);
  
  Serial.print(keys);

  ble.print("AT+BleKeyboard=");
  ble.println(keys);

  if( !ble.waitForOK() )
  {
    Serial.println( strcat("FAILED TO SEND:", keys) );
  }

  delay(10);
}
