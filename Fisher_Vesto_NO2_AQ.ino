#include"AirQuality.h"
#include"Arduino.h"
#include <SD.h>
AirQuality airqualitysensor;
int current_quality =-1;
File myFile;

void setup()
{
    Serial.begin(9600);
    airqualitysensor.init(A2);
    Serial.print("Initializing SD card...");
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt"); 
    //This code for the sd card doesn't actually record data in the file. The code below will need to be edited to record the values onto the sd card
}
}
void loop()
{
      current_quality=airqualitysensor.slope();
      if (current_quality >= 0)// if a valid data returned.
      {
          if (current_quality==0)
              Serial.println("High pollution! Force signal active"); //Serial.println will need to be edited to record the data in the sd card and not the serial monitor
              //FileName.println should resemble an option for recording the data onto the sd card, but there will first need to be a file created and opened, and closed once the data stops being recorded. 
          else if (current_quality==1)
              Serial.println("High pollution!");
          else if (current_quality==2)
              Serial.println("Low pollution!");
          else if (current_quality ==3)
              Serial.println("Fresh air");
      }
    // read the input on analog pin 0:
  int sensorValue1 = analogRead(A0);
  float voltage1 = sensorValue1 * ((5.0 / 1023.0)*1000);
  float concen1 = .274*voltage1; //reports the concentration in ppb
//   print out the value you read:
  Serial.println(concen1);
  delay(500);
}

ISR(TIMER2_OVF_vect)
{
    if(airqualitysensor.counter==122)//set 2 seconds as a detected duty
    {
        airqualitysensor.last_vol=airqualitysensor.first_vol;
        airqualitysensor.first_vol=analogRead(A0);
        airqualitysensor.counter=0;
        airqualitysensor.timer_index=1;
        PORTB=PORTB^0x20;
    }
    else
    {
        airqualitysensor.counter++;
    }
}
