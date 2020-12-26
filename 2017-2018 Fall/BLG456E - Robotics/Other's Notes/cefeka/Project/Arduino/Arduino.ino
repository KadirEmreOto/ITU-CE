#include <Servo.h>

Servo servo1;  // Create horizontal servo object
Servo servo2;  // Create vertical servo object

char command[2];
int i;
int position_1 = 90;  // Servo1 initialize degree 
int position_2 = 80;  // Servo2 initialize degree 

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  servo1.attach(9);   // Servo1 signal pin is 9 | Horizontal
  servo2.attach(10);  // Servo2 signal pin is 9 | Vertical

  servo1.write(position_1); // Initialize servo position
  servo2.write(position_2); // Initialize servo position
  
  Serial.println("Merhaba ! "); // Device Begin Message
}

void loop() {
  
    if(Serial.available() >= 2) // We wait untill can get two chars
    {
      //Serial.println("Geldi ! "); // Data are came
      for(i=0;i<2;i++)
      {
        command[i] = Serial.read();   // command[0] : Horizontal command[1]: Vertical
      }
      
      /* -#-#-#-#-#-#-#-#-#-#-#-# APPLY COMMAND #-#-#-#-#-#-#-#-#-#- */
      
      /* -------------------- BASE --------------------------------- */
      if (command[0] == 'L' && position_1 < 180)
      {
        position_1++;
        servo1.write(position_1);
        delay(15); // Wait for done servo move
      }
      else if(command[0] == 'R' && position_1 > 0)
      {
        position_1--;
        servo1.write(position_1);
        delay(15); 
      }
      else if(command[0] == 'C')
      {
        //position_1;
        //myservo.write(position_1);
        //delay(15); 
      }
      else if((command[0] == 'L' && position_1 == 180) || (command[0] == 'R' && position_1 == 0))
      {
        Serial.println("Target out of range ! ");
      }
      else
      {
        Serial.println("Wrong Input for parameter 1 ! ");
      }


      /* -------------------- Vertical --------------------------------- */
      if (command[1] == 'U' && position_2 > 0)
      {
        position_2--; 
        servo2.write(position_2);
        delay(15); 
      }
      else if(command[1] == 'D' && position_2 < 180)
      {
        position_2++;
        servo2.write(position_2);
        delay(15); 
      }
      else if(command[1] == 'C')
      {
        //position_2;
        //myservo.write(position_2);
        //delay(15); 
      }
      else if((command[1] == 'U' && position_2 == 0) || (command[1] == 'D' && position_2 == 180))
      {
        Serial.println("Target out of range ! ");
      }
      else
      {
        Serial.println("Wrong Input for parameter 2 ! ");
      }
      
     }
}
