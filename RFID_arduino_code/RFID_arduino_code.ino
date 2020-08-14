//RFID 

/**
 * 
 * this code is used to control a servo motor to lock and unlock a door
 * with a rfid signal. A button is also use to lock or unlock the door 
 * manually. 
 * 
 * Leds are used to indicate if a rfid card is valid or not. 
 */

 #include <SPI.h>
 #include <Servo.h>
 #include <MFRC522.h>

 #define SS_PIN 10
 #define RST_PIN 9
 #define led_green 4
 #define led_red 5
 #define True 1
 #define False 0

 const int buttonPin = 2;
 int buttonState = 0;

 int lock = False; // 0 unlock, 1 lock

MFRC522 mfrc522(SS_PIN, RST_PIN);
 Servo servo; // define servo name

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();   // Initiate MFRC522
  servo.attach(3); //servo pin
  servo.write(0); // start position
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  
  Serial.println("put your card to the reader");
  Serial.println();
  
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  if(buttonState == HIGH){
    Serial.println("button pushed");
    
    if(lock == False){// lock the door
      Serial.println("door locked");
      servo.write(100);
      lock = True;
      
    } else if (lock == True){ //unlock the door
      Serial.println("door unlocked");
      servo.write(0);
      lock = False;
    }
    delay(1000);
  }

     
      //look for new cards
    if( ! mfrc522.PICC_IsNewCardPresent()){
      return;
    }
    //Select one of the cards
    if( ! mfrc522.PICC_ReadCardSerial()){
      return;
    }

  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();



    if( content.substring(1) == "XX XX XX XX"){ // your rfid chip number

        Serial.println("Authorized access");
        Serial.println();
        delay(500);
        digitalWrite(led_green, HIGH);
        delay(500);
        digitalWrite(led_green, LOW);
        delay(500);
        digitalWrite(led_green, HIGH);
        delay(500);
        digitalWrite(led_green, LOW);
    
        if(lock == False){// lock the door
          Serial.println("door locked");
          servo.write(100);
          lock = True;
          
        } else if (lock == True){ //unlock the door
          Serial.println("door unlocked");
          servo.write(0);
          lock = False;
        }
        
      } else {
        Serial.println(" Access denied");
        digitalWrite(led_red, HIGH);
        delay(1000);
        digitalWrite(led_red, LOW);
      }

 
  }

  
