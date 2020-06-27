/*
  Modified from Arduino > Examples > Communication > Dimmer
 
 Demonstrates the sending data from the computer to the Arduino board,
 in this case to control the frequency of an oscillator.  The data is sent
 in individual bytes, each of which ranges from 0 to 255.  Arduino
 reads these bytes and uses them to set the frequency of the oscillator.
 
 Circuit: Audio output on digital pin 9. (STANDARD_PLUS mode in mozzi_config.h)
 Serial connection to Processing, Max/MSP, or another serial application
 
 Dimmer example created 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Tom Igoe and Scott Fitzgerald
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Dimmer
 
*/



#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <tables/saw2048_int8.h> // sine table for oscillator
#include <tables/saw8192_int8.h>
#include <tables/square_analogue512_int8.h>

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above

//Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> aSaw(SAW2048_DATA);
//Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> bSaw(SAW2048_DATA);
//Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> cSaw(SAW2048_DATA);


//Oscil <SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSaw(SQUARE_ANALOGUE512_DATA);
//Oscil <SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> bSaw(SQUARE_ANALOGUE512_DATA);
//Oscil <SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> cSaw(SQUARE_ANALOGUE512_DATA);



//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSaw(SIN2048_DATA);
//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> bSaw(SIN2048_DATA);
//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> cSaw(SIN2048_DATA);

Oscil <SAW8192_NUM_CELLS, AUDIO_RATE> aSaw(SAW8192_DATA);
Oscil <SAW8192_NUM_CELLS, AUDIO_RATE> bSaw(SAW8192_DATA);
Oscil <SAW8192_NUM_CELLS, AUDIO_RATE> cSaw(SAW8192_DATA);


void setup(){
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  //Serial.begin(9600); // for Teensy 3.1, beware printout can cause glitches
  Serial.begin(115200);
  startMozzi(); // uses default control rate of 64
}

float volume;
int detune = 0;
int note = 1;
int freq = 0;
byte midiData[2];
void updateControl(){
  // check if data has been sent from the computer:
  detune = mozziAnalogRead(7);
  //Serial.println(detune);
  if (Serial.available()) {
    // read the most recent byte (which will be from 0 to 255):
    //int midiData = Serial.read();
      
    Serial.readBytes(midiData, 2);    
    note = midiData[0];
    volume = midiData[1] / 255.0;
    
    freq = (int)pow(2, ((note+36+0.37462476) / 12.0)) + 5;    
    //Serial.println((int)pow(2, ((val - 30) / 12.0)));
  }
  aSaw.setFreq(freq);
  if (detune > 1) {
      //freq = (int)pow(2, ((note + 27) / 12.0));
      bSaw.setFreq(freq + (int)(freq * (detune / 4000.0)));
      //freq = (int)pow(2, ((note + 31) / 12.0));
      cSaw.setFreq(freq - (int)(freq * (detune / 4000.0)));
  }
  else {

      //freq = (int)pow(2, ((note + 27) / 12.0));
      bSaw.setFreq(0);
      //freq = (int)pow(2, ((note + 31) / 12.0));
      cSaw.setFreq(0);
  }
}


int updateAudio(){
  return (aSaw.next() - bSaw.next() - cSaw.next()) * volume; // return an int signal centred around 0
}

void loop(){
  audioHook(); // required here
}

/* Processing code for this example
 // Dimmer - sends bytes over a serial port
 // by David A. Mellis
 // This code is in the Public Domain.
 
 import processing.serial.*;
 Serial port;
 
 void setup() {
 size(256, 150);
 
 println("Available serial ports:");
 println(Serial.list());
 
 // Uses the first port in this list (number 0).  Change this to
 // select the port corresponding to your Arduino board.  The last
 // parameter (e.g. 9600) is the speed of the communication.  It
 // has to correspond to the value passed to Serial.begin() in your
 // Arduino sketch.
 port = new Serial(this, Serial.list()[0], 9600);  
 
 // If you know the name of the port used by the Arduino board, you
 // can specify it directly like this.
 //port = new Serial(this, "COM1", 9600);
 }
 
 void draw() {
 // draw a gradient from black to white
 for (int i = 0; i < 256; i++) {
 stroke(i);
 line(i, 0, i, 150);
 }
 
 // write the current X-position of the mouse to the serial port as
 // a single byte
 port.write(mouseX);
 }
*/

/* Max/MSP v5 patch for this example
 
----------begin_max5_patcher----------
1008.3ocuXszaiaCD9r8uhA5rqAeHIa0aAMaAVf1S6hdoYQAsDiL6JQZHQ2M
YWr+2KeX4vjnjXKKkKhhiGQ9MeyCNz+X9rnMp63sQvuB+MLa1OlOalSjUvrC
ymEUytKuh05TKJWUWyk5nE9eSyuS6jesvHu4F4MxOuUzB6X57sPKWVzBLXiP
xZtGj6q2vafaaT0.BzJfjj.p8ZPukazsQvpfcpFs8mXR3plh8BoBxURIOWyK
rxspZ0YI.eTCEh5Vqp+wGtFXZMKe6CZc3yWZwTdCmYW.BBkdiby8v0r+ST.W
sD9SdUkn8FYspPbqvnBNFtZWiUyLmleJWo0vuKzeuj2vpJLaWA7YiE7wREui
FpDFDp1KcbAFcP5sJoVxp4NB5Jq40ougIDxJt1wo3GDZHiNocKhiIExx+owv
AdOEAksDs.RRrOoww1Arc.9RvN2J9tamwjkcqknvAE0l+8WnjHqreNet8whK
z6mukIK4d+Xknv3jstvJs8EirMMhxsZIusET25jXbX8xczIl5xPVxhPcTGFu
xNDu9rXtUCg37g9Q8Yc+EuofIYmg8QdkPCrOnXsaHwYs3rWx9PGsO+pqueG2
uNQBqWFh1X7qQG+3.VHcHrfO1nyR2TlqpTM9MDsLKNCQVz6KO.+Sfc5j1Ykj
jzkn2jwNDRP7LVb3d9LtoWBAOnvB92Le6yRmZ4UF7YpQhiFi7A5Ka8zXhKdA
4r9TRGG7V4COiSbAJKdXrWNhhF0hNUh7uBa4Mba0l7JUK+omjDMwkSn95Izr
TOwkdp7W.oPRmNRQsiKeu4j3CkfVgt.NYPEYqMGvvJ48vIlPiyzrIuZskWIS
xGJPcmPiWOfLodybH3wjPbMYwlbFIMNHPHFOtLBNaLSa9sGk1TxMzCX5KTa6
WIH2ocxSdngM0QPqFRxyPHFsprrhGc9Gy9xoBjz0NWdR2yW9DUa2F85jG2v9
FgTO4Q8qiC7fzzQNpmNpsY3BrYPVJBMJQ1uVmoItRhw9NrVGO3NMNzYZ+zS7
3WTvTOnUydG5kHMKLqAOjTe7fN2bGSxOZDkMrBrGQ9J1gONBEy0k4gVo8qHc
cxmfxVihWz6a3yqY9NazzUYkua9UnynadOtogW.JfsVGRVNEbWF8I+eHtcwJ
+wLXqZeSdWLo+FQF6731Tva0BISKTx.cLwmgJsUTTvkg1YsnXmxDge.CDR7x
D6YmX6fMznaF7kdczmJXwm.XSOOrdoHhNA7GMiZYLZZR.+4lconMaJP6JOZ8
ftCs1YWHZI3o.sIXezX5ihMSuXzZtk3ai1mXRSczoCS32hAydeyXNEu5SHyS
xqZqbd3ZLdera1iPqYxOm++v7SUSz
-----------end_max5_patcher-----------
*/
