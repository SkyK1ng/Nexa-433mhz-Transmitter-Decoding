//NExa.ino

//Example of a decoded NEXA Pet-910 remote, Data code: 1910
//-----------------------------------------------------------------------------------------
//Top     ON   2, 1,0,0,0, 0,1,1,0, 1,0,0,0, 0,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 0,0,0,0, 0
//Middle  ON   2, 1,0,0,0, 0,1,1,0, 1,0,0,0, 0,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 0,0,0,1, 0
//Botton  ON   2, 1,0,0,0, 0,1,1,0, 1,0,0,0, 0,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 0,0,1,0, 0
//-----------------------------------------------------------------------------------------
//Top     OFF  2, 1,0,0,0, 0,1,1,0, 1,0,0,0, 0,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,0, 0,0,0,0, 0
//Middle  OFF  2, 1,0,0,0, 0,1,1,0, 1,0,0,0, 0,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,0, 0,0,0,1, 0
//Botton  OFF  2, 1,0,0,0, 0,1,1,0, 1,0,0,0, 0,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,0, 0,0,1,0, 0
//-----------------------------------------------------------------------------------------
//Last 4 bits are the bottom, ie top(0000), middle(0010) or bottom(0100), Se above.
//Second to the last 4 bits denote ON or OFF, ie ON = 10 / OFF = 01, Se above.


#define T 275 //Puls length in ms can be measured with SDR
#define TXpin 8

void setup() {
  initate();
}

void loop() {
  delay(2002);
  Transmitt(1, true);
  delay(4002);
  Transmitt(1, false);
  delay(4002);
  Transmitt(3, true);
  delay(4002);
  Transmitt(3, false);
};

void Transmitt(int Button, bool State){
  int AdressTopButtonON[] = {2,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,0};
  int AdressTopButtonOFF[] = {2,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0};
  int AdressMiddleButtonON[] = {2,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,0,0,1,0};
  int AdressMiddleButtonOFF[] = {2,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0};
  int AdressButtomButtonON[] = {2,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,0,0,1,0,0};
  int AdressButtomButtonOFF[] = {2,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0};
  switch(Button){ 
    case 1: //button equals 1 (TopButton)
      if (State == true) { // State ON
      Serial.println("Toppbutton ON");
      Sender(AdressTopButtonON);
      }
      else { // State OFF
      Serial.println("Toppbutton OFF");
      Sender(AdressTopButtonOFF);
      }
      break;
    case 2: //button equals 2 (MiddleButton)
      if (State == true) { // State ON
      Serial.println("Middlebutton ON");
      Sender(AdressMiddleButtonON);
      }
      else { // State OFF
      Serial.println("Middlebutton OFF");
      Sender(AdressMiddleButtonOFF);
      }
      break;
    case 3: //button equals 3 (BottumButton)
      if (State == true) { // State ON
      Serial.println("ButtomButton ON");
      Sender(AdressButtomButtonON);
      }
      else { // State OFF
      Serial.println("ButtomButton OFF");
      Sender(AdressButtomButtonOFF);
      }
      break;
    default: 
      // If incorrect button code is enterd
      Serial.print("No button found");
      break;
  }
}

void initate(){
  Serial.begin(9600);
  pinMode(TXpin, OUTPUT);   // sets the TXpin digital as output
}

void HighBit(){ //1 Bit
  digitalWrite(TXpin, HIGH);
  delayMicroseconds(T);
  digitalWrite(TXpin, LOW);
  delayMicroseconds(4*T);  
  digitalWrite(TXpin, HIGH);
  delayMicroseconds(T);
  digitalWrite(TXpin, LOW);
  delayMicroseconds(T);
}

void LowBit(){ //0 Bit
  digitalWrite(TXpin, HIGH);
  delayMicroseconds(T);
  digitalWrite(TXpin, LOW);
  delayMicroseconds(T);  
  digitalWrite(TXpin, HIGH);
  delayMicroseconds(T);
  digitalWrite(TXpin, LOW);
  delayMicroseconds(4*T);  
}

void Stopp(){ //2 STOP/START
  digitalWrite(TXpin, HIGH);
  delayMicroseconds(T);
  digitalWrite(TXpin, LOW);
  delayMicroseconds(3000);
};

void Sender(int Adress[]){
  int Arraysize = 34;
  int RepeatTimes = 4;
  int Bit;

  for (int count = 0; count < RepeatTimes; count++) {
  delay(10);
    for (int count = 0; count < Arraysize; count++) {
      delayMicroseconds(10); //Pause between sent bit
        Bit = Adress[count];    
        if (Bit == 2) {   //2 STOP
          Stopp();
        } 
        if (Bit == 0) {   //0 Bit
          LowBit(); 
        }
        if (Bit == 1) {   //1 Bit
          HighBit();
        }
    }
  }
};