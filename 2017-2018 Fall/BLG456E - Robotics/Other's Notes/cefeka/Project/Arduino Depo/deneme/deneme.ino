/*
  ASCII table

 Prints out byte values in all possible formats:
 * as raw binary values
 * as ASCII-encoded decimal, hex, octal, and binary values

 For more on ASCII, see http://www.asciitable.com and http://en.wikipedia.org/wiki/ASCII

 The circuit:  No external hardware needed.

 created 2006
 by Nicholas Zambetti
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 <http://www.zambetti.com>

 */

char buf[1000];
int idx;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  pinMode(13,OUTPUT);
  // prints title with ending line break
  Serial.println("Merhaba ! ");
  idx = 0;
}


void loop() {
  // if there's any serial available, read it:

    int gelen_baytlar=0;
    int i = 0;


    gelen_baytlar = Serial.available();

    
    for(i=0;i<gelen_baytlar;i++)
    {
      char gelen = Serial.read();

      buf[idx++] = gelen;

      if(gelen == '\n')
      {
       Serial.write(buf,idx);
       idx = 0;
      }
      else if ('c' == gelen)
      {
        char pindurum = 0;

        pindurum = digitalRead(13);

        pindurum = pindurum ^ 1;
        
         digitalWrite(13,pindurum);
      }  
      else
      {
        
      }
  }
}
