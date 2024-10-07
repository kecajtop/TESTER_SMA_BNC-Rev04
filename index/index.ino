#define ledRed 14 //A0
#define ledGreen 15 //A1
#define ledBlue 16 //A2
#define testSwitch 17 //A3

#define smaPin1 8 /D8
#define smaPin2 12 //D12
#define smaShield1 9 D9
#define smaShield2 13 //D13
#define activateSma1 9 //D9
#define activateSma2 10 //D10

#define bncPin1 7 //D7
#define bncPin2 5 //D5
#define bncShield1 6 //D6
#define bncShield2 4 //D4


void setup() {
  Serial.begin(9600);
  pinMode(testSwitch, INPUT_PULLUP);
  startLedSignature();
}

void loop() {
  testSma();
  testBnc();
}
void testBnc() {
  byte testResult = true;
  static byte testSteps = 0;

  pinMode(bncPin1, INPUT_PULLUP);
  pinMode(bncPin2, OUTPUT);
  digitalWrite(bncPin2, LOW);

  if (digitalRead(bncPin1) == LOW && testSteps == 0 && digitalRead(testSwitch)==LOW) {
    Serial.println("Rozpoczynam test BNC");
    signatureLed(3);
    delay(500);
    testSteps = 1;
    pinMode(bncShield1, INPUT_PULLUP);
    pinMode(bncShield2, OUTPUT);
    digitalWrite(bncShield2, LOW);

    if (digitalRead(bncShield1) == LOW) {
      Serial.println("Test polaczen BNC OK");
    } else {
      Serial.println("Test polaczen BNC NIEOK");
      testResult = false;
    }
  }
  //test na zwarcie
  if (testSteps == 1) {
    pinMode(bncPin2, OUTPUT);
    pinMode(bncShield2, OUTPUT);
    digitalWrite(bncPin2, HIGH);
    digitalWrite(bncShield2, LOW);

    pinMode(bncPin1, INPUT_PULLUP);
    pinMode(bncShield1, OUTPUT);
    digitalWrite(bncShield1, LOW);
    if (digitalRead(bncPin1) == LOW) {
      Serial.println("zwarcie");
      testSteps = 2;
      testResult = false;
      signatureLed(4);
    } else {
      testSteps = 2;
      Serial.println("Test zwarcia BNC OK");
    }
  }

  //wynik testu
  if (testResult && testSteps == 2) {
    Serial.println("Test BNC OK");
    Serial.println("");
    //testSteps = 3; wylaczam blokade ponownego testu - dodano switch
    testSteps = 0;
    signatureLed(2);
  } else if (!testResult && testSteps == 2) {
    Serial.println("Test BNC NIEOK");
    Serial.println("");
    //testSteps = 3; wylaczam blokade ponownego testu - dodano switch
    testSteps = 0;
    signatureLed(1);
  }

  //oczekiwanie na odpiecie przewodow z testera//wylaczone z racji podlaczenia switch
  // if (testSteps == 3) {
  //   //czekam na odpiecie przewodu od testera
  //   pinMode(bncPin1, INPUT_PULLUP);
  //   pinMode(bncPin2, OUTPUT);
  //   digitalWrite(bncPin2, LOW);
  //   if (digitalRead(bncPin1) == HIGH) {
  //     Serial.println("Odpieto wiazke z testera");
  //     Serial.println("");
  //     testSteps = 0;
  //     delay(300);
  //   }
  // }
}
void testSma() {
  byte testResult = true;
  static byte testSteps = 0;

  pinMode(activateSma1, INPUT_PULLUP);
  pinMode(activateSma2, OUTPUT);
  digitalWrite(activateSma2, LOW);

  //uruchomienie testu po aktywacji
  if (digitalRead(activateSma1) == LOW && testSteps == 0 && digitalRead(testSwitch)==LOW) {
    Serial.println("Rozpoczynam test SMA");
    signatureLed(3);
    testSteps = 1;
    /******************************************/
    pinMode(smaPin1, INPUT_PULLUP);
    pinMode(smaPin2, OUTPUT);
    digitalWrite(smaPin2, LOW);
    /******************************************/
    pinMode(smaShield1, INPUT_PULLUP);
    pinMode(smaShield2, OUTPUT);
    digitalWrite(smaShield2, LOW);
    /******************************************/
    delay(500);
    if (digitalRead(smaPin1) == LOW && digitalRead(smaShield1) == LOW) {
      Serial.println("Test polaczen SMA OK");
    } else {
      Serial.println("Test polaczen SMA NIEOK");
      testResult = false;
    }
  }

  //test na zwarcie
  if (testSteps == 1) {
    pinMode(smaPin2, OUTPUT);
    pinMode(smaShield2, OUTPUT);
    digitalWrite(smaPin2, HIGH);
    digitalWrite(smaShield2, LOW);

    pinMode(smaPin1, INPUT_PULLUP);
    pinMode(smaShield1, OUTPUT);
    digitalWrite(smaShield1, LOW);
    if (digitalRead(smaPin1) == LOW) {
      testSteps = 2;
      testResult = false;
      signatureLed(4);
      Serial.println("Test zwarcia SMA NIEOK");
    } else {
      testSteps = 2;
      Serial.println("Test zwarcia SMA OK");
    }
  }

  //wynik testu
  if (testResult && testSteps == 2) {
    Serial.println("Test SMA OK");
    Serial.println("");
    //testSteps = 3; wylaczam blokade ponownego testu - dodano switch
    testSteps = 0;
    signatureLed(2);
  } else if (!testResult && testSteps == 2) {
    Serial.println("Test SMA NIEOK");
    Serial.println("");
    //testSteps = 3; wylaczam blokade ponownego testu - dodano switch
    testSteps = 0;
    signatureLed(1);
  }

  // if (testSteps == 3) {  //wylaczone z racji podlaczenia switcha
  //   //czekam na odpiecie przewodu od testera
  //   pinMode(activateSma1, INPUT_PULLUP);
  //   pinMode(activateSma2, OUTPUT);
  //   digitalWrite(activateSma2, LOW);
  //   if (digitalRead(activateSma1) == HIGH) {
  //     Serial.println("no i zakonczono test...");
  //     testSteps = 0;
  //     delay(300);
  //   }
  // }
}
void signatureLed(byte status) {
  switch (status) {
    case 1:  //blad
      digitalWrite(ledBlue, LOW);
      digitalWrite(ledRed, HIGH);
      delay(2000);
      digitalWrite(ledRed, LOW);
      break;
    case 2:  //ok
      digitalWrite(ledBlue, LOW);
      digitalWrite(ledGreen, HIGH);
      delay(2000);
      digitalWrite(ledGreen, LOW);
      break;
    case 3:  //oczekiwanie na test
      digitalWrite(ledBlue, HIGH);
      break;
    case 4:  //zwarcie
      for (byte i = 0; i < 4; i++) {
        digitalWrite(ledRed, HIGH);
        delay(100);
        digitalWrite(ledRed, LOW);
        delay(100);
      }
      break;
  }
}
void startLedSignature() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  for (byte i = 0; i < 2; i++) {
    digitalWrite(ledRed, HIGH);
    delay(200);
    digitalWrite(ledRed, LOW);
    delay(200);
  }
  for (byte i = 0; i < 2; i++) {
    digitalWrite(ledBlue, HIGH);
    delay(200);
    digitalWrite(ledBlue, LOW);
    delay(200);
  }
  for (byte i = 0; i < 2; i++) {
    digitalWrite(ledGreen, HIGH);
    delay(200);
    digitalWrite(ledGreen, LOW);
    delay(200);
  }
}
