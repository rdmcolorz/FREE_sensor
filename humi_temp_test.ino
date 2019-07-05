#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <SPI.h>
#include <SD.h>
 
Adafruit_SHT31 sht31 = Adafruit_SHT31();
byte inByte;
//bool sdInitSuccess = false; //card init status
File myFile;
String readText;
char readCharArray[128];
unsigned long fileSize;
unsigned long filePos = 0;
String FILE_NAME = "new.txt";

void setup() 
{
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for the serial port to connect.
  }
  
  // start up the adafruit_sht31 temp / humidity sensor according to the setup link at 0x44
  if (!sht31.begin(0x44)) 
  {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  Serial.println("Setup SD");
  // start up the SD card reader at pin D8
  if (!SD.begin(D8)) {
    Serial.println("Initializing SD card");
    while (1) delay(1);
  }
  else {
    Serial.println("Initialize Sucess");
  }
}

//String readUntil(File &myFile, char n) { //read until n
//  int i = 0;
//  myFile.seek(filePos); //read from current filepos
//  
//  do {
//    if (myFile.peek() != -1) { //check if file is empty
//      readCharArray[i++] = myFile.read(); //read otherwise
//      filePos++; //advance the filepos
//    }
//  } while ((myFile.peek() != -1) && (readCharArray[i - 1] != n)); //if not eof | \n
//  if (myFile.peek() == -1) { //if eof reached
//    filePos = 0; //if eof, reset to start pos
//  }
//  readCharArray[i - 1] = '\0'; //remove the extra \n
//  return readCharArray;
//}

void loop() 
{
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();


  Serial.print(t);
  Serial.print(",");
  Serial.println(h);
  if (Serial.available() > 0) {
    inByte = Serial.read();
  }
  
  myFile = SD.open(FILE_NAME, FILE_WRITE);
  if (myFile) {
    if (!isnan(t) && !isnan(h)) {
      myFile.print(t);
      myFile.print(",");
      myFile.println(h);
      myFile.close(); //this writes to the card
      Serial.println("* wrote to file *");
    }
    else { 
      Serial.println("Failed to read sht31 (temperature/humidity)");
    }
  }
  else { 
    //else show error
    Serial.println("Error opeing file.\n");
  }
  delay(1000);
}
//    else if (inByte == 'r' || inByte == 'R') {
//      if (sdInitSuccess) { //proceed only if card is initialized
//        myFile = SD.open("TEST.txt");
//        if (myFile) {
//          Serial.println("File opened successfully.");
//          Serial.println("Reading from TEST.text");
//          readText = (String) readUntil(myFile, 10); //read until newline
//          Serial.print(readText);
//          Serial.print(", ");
//          Serial.println(filePos); //print current file position
//          Serial.println();
//          myFile.close();
//        }
//        else {
//          Serial.println("Error opening file.\n");
//        }
