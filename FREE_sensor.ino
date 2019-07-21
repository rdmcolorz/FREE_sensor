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
String FILE_NAME = "data19.txt";
byte DEBUG = 0;
String STATE;

void setup() 
{
  STATE = "WRITE";
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for the serial port to connect.
  }
  
  // start up the adafruit_sht31 temp / humidity sensor according to the setup link at 0x44
  if (!sht31.begin(0x44)) 
  {
    if (DEBUG) Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  delay(500);
  Serial.println("Setup SD");
  // start up the SD card reader at pin D8
  if (!SD.begin(D8)) {
    if (DEBUG) Serial.println("Initializing SD card");
    while (1) delay(1);
  }
  else {
    if (DEBUG) Serial.println("Initialize Sucess");
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

String ENDF2(String &p_line, int &p_start, char p_delimiter) {
//EXTRACT NEXT DELIMITED FIELD VERSION 2
//Extract fields from a line one at a time based on a delimiter.
//Because the line remains intact we dont fragment heap memory
//p_start would normally start as 0
//p_start increments as we move along the line
//We return p_start = -1 with the last field

  //If we have already parsed the whole line then return null
  if (p_start == -1) {
    return "";
  }

  int l_start = p_start;
  int l_index = p_line.indexOf(p_delimiter,l_start);
  if (l_index == -1) { //last field of the data line
    p_start = l_index;
    return p_line.substring(l_start);
  }
  else { //take the next field off the data line
    p_start = l_index + 1;
    return p_line.substring(l_start,l_index); //Include, Exclude
  }
}

void writeString(String stringData) { // Used to serially push out a String with Serial.write()

  for (int i = 0; i < stringData.length(); i++) {
    Serial.write(stringData[i]);   // Push each char 1 by 1 on each loop pass
  }
  Serial.write('\n');
}

String read_all(File &myFile) {
  String l_line;
    l_line.reserve(128); //Avoids heap memory fragmentation
                         //Reserve space for your longest expected data line
    while (myFile.available()) {
      l_line = myFile.readStringUntil('\n');
      l_line.trim();
      if (l_line != "") {
        int l_start_posn = 0;
        while (l_start_posn != -1) {
          String outline;
          outline = ENDF2(l_line,l_start_posn,'\n');
          writeString(outline);
        }
      } //skip blank (NULL) lines
    }//Read the file line by line
}


void write_data() {
// writes sensor data to sdcard, currently only temperature and humidity from sht31
// board
// Closing the file actually copies the partial buffer to the file, and closes the file.
// It is that copying of the buffer to the file that takes most of the time.

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  
  if (DEBUG) {
    Serial.print(t);
    Serial.print(",");
    Serial.println(h);
  }
  
  myFile = SD.open(FILE_NAME, FILE_WRITE);
  if (myFile) {
    if (!isnan(t) && !isnan(h)) {
      myFile.print(t);
      myFile.print(",");
      myFile.println(h);
      myFile.close(); //this writes to the card
    }
    else { 
      if (DEBUG) Serial.println("Failed to read sht31 (temperature/humidity)");
    }
  }
  else { 
    if (DEBUG) Serial.println("Error opeing file.\n");
  }
}

void read_data() {
// writes data read from the file on the microSD card through Serial.write()
// after reading the file, the esp will go into standby mode and will not
// write any data into file.

  myFile = SD.open(FILE_NAME);
  
  if (myFile) {
    read_all(myFile);
  }
  STATE = "STANDBY";
  myFile.close();
}

void loop() 
{
  if (Serial.available() > 0) {
    inByte = Serial.read();
  }
  if (inByte == '1') { // when we want to read the recorded file to the cloud
    STATE = "READ";
  }
  else if (inByte == '2') {
    STATE = "WRITE";
  }
  
  if (STATE == "READ") {
    read_data();
    Serial.println("END");
  }
  if (STATE == "STANDBY") {
    if (DEBUG) {
      Serial.println("***********");
      Serial.println("Standing by");
    }
  }
  if (STATE == "WRITE") {
    write_data();
    delay(5000);
  }
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
