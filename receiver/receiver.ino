/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <RF24_config.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>

#include "../../arduino-nrf24-multiple-tx/shared/pta.h"

#define I2C_ADDR 0x27
LiquidCrystal_I2C lcd(I2C_ADDR, 8, 2);

RF24 radio(9, 10);
static const int receiverAdd[2] = { 0xF0F0F0F0AA, 0xF0F0F0F066 };

void setup()
{
    lcd.init();
    lcd.blink();
    lcd.backlight();
    lcd.home();

    Serial.begin(9600);
    Serial.println(F("Receiver"));
    lcd.print("Receiver");

    radio.begin();
    radio.setAutoAck(true);
    radio.setPALevel(RF24_PA_LOW);
    radio.setRetries(15, 15);
    radio.openReadingPipe(1, receiverAdd[0]);
    radio.openReadingPipe(2, receiverAdd[1]);

    // Start the radio listening for data
    radio.startListening();
}

byte lastValue = 0;
void displayBufferOnLcd(byte value, int pipe)
{
    if (value != lastValue + 1)
    {
        Serial.println("--------");
    }
    lastValue = value;

    Serial.print(F("Pipe: "));
    Serial.print(pipe);
    Serial.print(F(", "));
    Serial.print(F("Value: "));
    Serial.println(value, DEC);
}


void loop()
{
    byte value;

    uint8_t pipeNum = 0;
    if (radio.available(&pipeNum))
    {
        PTA pta;
        radio.read(&pta, sizeof(PTA));
        PrintPTA(pta, Serial);
    }
}
