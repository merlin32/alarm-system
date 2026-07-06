#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <IRremote.h>

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

Servo motor;
LiquidCrystal lcd(41, 39, 31, 37, 33, 29);

const byte ROWS = 4;
const byte COLS = 4;
char password[5];
int lcd_backlightPin = 35;
int buzzerPin = 11;
int motorsPin = 10;
int trigPin = 13;
int echoPin = 12;
int ledPin = A3;
int redLed = A4;
int PIRsensor1 = 53;
int PIRsensor2 = 51;
int IRsensor = 45;
int armed = 1;
int afk = 0;
int counter = 0;
int correctPassword = 1;
int ringTime, ringMode, oldDistance;
int button2 = 0;
int justArmed = 0;
unsigned long startTime = 0;
unsigned long startTime2 = 0;
bool wallRing = true;
bool trigger = false;

char keyMap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};

byte colPins[COLS] = {5, 4, 2, 3};
byte rowPins[ROWS] = {9, 8, 7, 6};
Keypad customKeypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.noCursor();
  pinMode(lcd_backlightPin, OUTPUT);
  digitalWrite(lcd_backlightPin, HIGH);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(PIRsensor1, INPUT);
  pinMode(PIRsensor2, INPUT);
  motor.attach(motorsPin);
  motor.write(0);
  IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
  int set = 0;
  while(set == 0)
  {
      if(passwordSet())
        set = 1;
      if(afk)
        digitalWrite(lcd_backlightPin, LOW);
  }
  set = 0;
  while(set == 0)
  {
      if(ringTimeSet())
        set = 1;
      if(afk)
        digitalWrite(lcd_backlightPin, LOW);
  }
  set = 0;
  while(set == 0)
  {
      if(ringModeSet())
        set = 1;
      if(afk)
       digitalWrite(lcd_backlightPin, LOW);
  }
  analogWrite(redLed, 255);
  lcd.setCursor(0,0);
  lcd.print("Enter password");
  lcd.setCursor(0,1);
  lcd.print("____");
  justArmed = 1;
  
  
}

void loop()
{
   unsigned long startTime3 = millis();
   int correctPassword = 1;
   char button = customKeypad.getKey();
  if(IrReceiver.decode())
  {
    switch(IrReceiver.decodedIRData.decodedRawData)
    {
      case 0xF30CFF00:
        armed = 0;
        unlocked();
        justArmed = 0;
        break;
      case 0xE718FF00:
        armed = 1;
        locked();
        while(millis() - startTime3 <= 7000);
        break;
    }
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
  }
  else
{
  IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
  delay(500);
   if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
   while(counter < 4 && millis() - startTime3 <= 7000 && afk == 0)
   {
   		button = customKeypad.getKey();
   		if(button)
   		{
     		startTime3 = millis();
        	if(button == '#')
  			{ 
     			analogWrite(buzzerPin, 20);
     			delay(25);
     			analogWrite(buzzerPin, 0);
          delay(100);
     			passwordReset();
          startTime3 = millis();
     			lcd.setCursor(0,0);
          if(armed)
              lcd.print("Enter password");
     			else
              		lcd.print("Password to arm");
          		lcd.setCursor(0,1);
          		lcd.print("____");
   			}
			else if(button == 'A')
        	{
          	analogWrite(buzzerPin, 20);
     	  		delay(25);
     	  		analogWrite(buzzerPin, 0);
            delay(100);
     			  ringTimeReset();
            startTime3 = millis();
          	lcd.setCursor(0,0);
          	if(armed)
     				  lcd.print("Enter password");
     	  		else
             		lcd.print("Password to arm");
          	lcd.setCursor(0,1);
          	lcd.print("____");
        	}
          else if(button == 'B')
          {
            analogWrite(buzzerPin, 20);
     	  		delay(25);
     	  		analogWrite(buzzerPin, 0);
            delay(100);
     			  ringModeReset();
            startTime3 = millis();
          	lcd.setCursor(0,0);
          	if(armed)
     				  lcd.print("Enter password");
     	  		else
             		lcd.print("Password to arm");
          	lcd.setCursor(0,1);
          	lcd.print("____");
          }
     		else if(button == 'C')
        	{
          	analogWrite(buzzerPin, 20);
     	  		delay(25);
     	  		analogWrite(buzzerPin, 0);
            delay(100);
     			  wallRingSet();
            startTime3 = millis();
          	lcd.setCursor(0,0);
          	if(armed)
     				  lcd.print("Enter password");
     	  		else
              		lcd.print("Password to arm");
          	lcd.setCursor(0,1);
         		lcd.print("____");
        	}
     		else
        	{
     			analogWrite(buzzerPin, 20);
    			delay(25);
     			analogWrite(buzzerPin, 0);
          delay(100);
     			lcd.setCursor(counter, 1);
     			lcd.print(button);
     			if(button != password[counter])
     				correctPassword = 0;
    			counter++;
     			
       	 }
     }
   	}
  } 
  	if(millis() - startTime3 >= 7000)
    {  
      digitalWrite(lcd_backlightPin, LOW);
      afk = 1;
      justArmed = 0;
    }
   if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
     
     if(correctPassword && counter == 4)
     {
         if(armed)
         {
            unlocked();
            armed = 0;
            justArmed = 0;
         }
         else
         {
            locked();
            armed = 1;
            justArmed = 1;
          }
      }
     else if(correctPassword == 0)
     {
       	displayWrong();
       	lcd.setCursor(0,0);
        lcd.print("Enter password");
        lcd.setCursor(0,1);
        lcd.print("____");
     }
  
  	if(wallRing)
      wallMount();

    buttonCheck();
    // Serial.print("Just armed: ");
    // Serial.println(justArmed);
     if(armed)
    {
      int val1 = digitalRead(PIRsensor1);
      int val2 = digitalRead(PIRsensor2);
      startTime2 = millis();
      if(val1 == HIGH && justArmed == 0)
      {
        // Serial.print("AFK: ");
        // Serial.println(afk);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Warning in");
        lcd.setCursor(0,1);
        lcd.print("Zone 1");
        trigger = true;
        IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
        while (millis() - startTime2 < ringTime && trigger == true)
        {
          if (IrReceiver.decode())
          {
              //IrReceiver.printIRResultShort(&Serial);
              delay(20);
              if(IrReceiver.decodedIRData.decodedRawData == 0xA15EFF00)
                trigger = false;
              IrReceiver.resume();
          }
           if (ringMode == 1)
              ringMode1();
          else
            ringMode2();
          IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
          IrReceiver.resume();
          delay(500);
        }
        IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter password");
        lcd.setCursor(0,1);
        lcd.print("____");
      }
      if(val2 == HIGH && justArmed == 0)
      {
        // Serial.print("AFK: ");
        // Serial.println(afk);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Warning in");
        lcd.setCursor(0,1);
        lcd.print("Zone 2");
        trigger = true;
        IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
        while (millis() - startTime2 < ringTime && trigger == true)
        {
          if (IrReceiver.decode())
          {
            delay(20);
              //IrReceiver.printIRResultShort(&Serial);
              if(IrReceiver.decodedIRData.decodedRawData == 0xA15EFF00)
                trigger = false;
              IrReceiver.resume();
          }
           if (ringMode == 1)
              ringMode1();
          else
            ringMode2();
          IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
          IrReceiver.resume();
          delay(500);
        }
        IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter password");
        lcd.setCursor(0,1);
        lcd.print("____");
      }
    }
  	counter = 0;
    	
}

void wallMount()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  // Serial.print("Duration ");
  // Serial.println(duration);
  int distance = duration*0.034/2;
  // Serial.print("Distance ");
  // Serial.println(distance);
  startTime = millis();
  if(distance >= 4 && distance <= 100 && abs(distance - oldDistance) < 10)
  {
    while(millis() - startTime <= ringTime)
      if(ringMode == 1)
      	ringMode1();
      else
      	ringMode2();
    IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
  }
  oldDistance = distance;
}

//cazul armed

void locked()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Has armed");
  analogWrite(buzzerPin, 20);
  delay(500);
  analogWrite(buzzerPin, 0);
  delay(200);
  analogWrite(buzzerPin, 20);
  delay(500);
  analogWrite(buzzerPin, 0);
  delay(1000);
  motor.write(0);
  IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
  delay(100);
  analogWrite(ledPin, 0);
  analogWrite(redLed, 255);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter password");
  lcd.setCursor(0,1);
  lcd.print("____");
  
}

//cazul disarmed

void unlocked()
{
  	  lcd.clear();
  	  lcd.setCursor(0,0);
  	  lcd.print("Correct password");
  	  analogWrite(buzzerPin, 20);
      delay(500);
      analogWrite(buzzerPin, 0);
      delay(200);
  	  analogWrite(ledPin, 255);
      analogWrite(redLed, 0);
  	  delay(1000);
      motor.write(90);
      IrReceiver.begin(IRsensor, ENABLE_LED_FEEDBACK);
      delay(100);
  	  lcd.clear();
  	  lcd.setCursor(0,0);
  	  lcd.print("Password to arm");
  	  lcd.setCursor(0,1);
      lcd.print("____");
  	   
}

//functie pentru resetarea parolei


void passwordReset()
{
  	lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Enter old passwd");
    lcd.setCursor(0,1);
    lcd.print("____");
  	int ok = passwordCheck();
  	if(ok == 1)
    {
      	lcd.clear();
      	passwordSet();
      	
    }
    if(ok == 3)
      lcd.clear();
  	
}

//functie pentru setarea parolei

int passwordSet()
{
  lcd.setCursor(0,0);
  lcd.print("Set password");
  lcd.setCursor(0,1);
  lcd.print("____");
  int i = 0;
  unsigned long start = millis();
  while (i < 4 && millis() - start <= 20000)
  {
    	char button = customKeypad.getKey();
    	if(button)
        {
            afk = 0;
            digitalWrite(lcd_backlightPin, HIGH);
          	lcd.setCursor(i, 1);
          	analogWrite(buzzerPin, 20);
          	delay(25);
          	analogWrite(buzzerPin, 0);
            delay(100);
            password[i] = button;
          	lcd.print(button);
          	i++;
        }
    	
  }
  if(millis() - start <= 20000)
  {
    delay(50);
    lcd.clear();
    lcd.setCursor(0,0);
    delay(500);
    lcd.print("Password set");
    analogWrite(buzzerPin, 20);
    delay(200);
    analogWrite(buzzerPin, 0);
    delay(2300);
    lcd.clear();
    return 1;
  }
  else
  {
     afk = 1;
     return 0;
  }
}

int ringTimeSet()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ring time(sec)");
  lcd.setCursor(0,1);
  lcd.print("__");
  int i = 0;
  unsigned long start = millis();
  while (i < 2 && millis() - start <= 20000)
  {
    char button = customKeypad.getKey();
    if(button && i == 1)
    {
        afk = 0;
        digitalWrite(lcd_backlightPin, HIGH);
      	analogWrite(buzzerPin, 20);
     	delay(25);
     	analogWrite(buzzerPin, 0);
      delay(100);
      	lcd.setCursor(i,1);
      	lcd.print(button);
      	ringTime *= 10 + (button - '0');
      	ringTime *= 1000;
      	i++;
    }
    if(button && i == 0)
    {
        afk = 0;
        digitalWrite(lcd_backlightPin, HIGH);
      	analogWrite(buzzerPin, 20);
     	delay(25);
     	analogWrite(buzzerPin, 0);
      delay(100);
      	lcd.setCursor(i,1);
      	lcd.print(button);
      	ringTime = button - '0';
      	i++;
    }
  }
  if(millis() - start <= 20000)
  {
    delay(50);
    lcd.clear();
    lcd.setCursor(0,0);
    delay(500);
    lcd.print("Set");
    analogWrite(buzzerPin, 20);
    delay(200);
    analogWrite(buzzerPin, 0);
    delay(2300);
    lcd.clear();
    return 1;
  }
  else
  {
    afk = 1;
    lcd.clear();
    return 0; 
  }
}

int ringModeSet()
{
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ring mode [1/2]");
    lcd.setCursor(0,1);
    lcd.print("_");
    int i = 0;
    unsigned long start = millis();
    while (i < 1 && millis() - start <= 20000)
    {
      char button = customKeypad.getKey();
      if(button)
      {
        afk = 0;
        digitalWrite(lcd_backlightPin, HIGH);
      	if(button == '1' || button == '2')
        {
      		analogWrite(buzzerPin, 20);
     		  delay(25);
     		  analogWrite(buzzerPin, 0);
          delay(100);
      		lcd.setCursor(i,1);
      		lcd.print(button);
      		ringMode = button - '0';
      		i++;
        }
      }
    }
    if(millis() - start <= 20000)
    {
      delay(500);
      analogWrite(buzzerPin, 20);
      delay(200);
      analogWrite(buzzerPin, 0);
      delay(100);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Setup done");
      delay(2000);
      lcd.clear();
      return 1;
    }
    else
    {
      afk = 1;
      lcd.clear();
      return 0;
    }
}

void ringTimeReset()
{
  	lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Type password");
  	lcd.setCursor(0,1);
    lcd.print("____");
    int ok = passwordCheck();
  	if(ok == 1)
    {
      	lcd.clear();
      	ringTimeSet();	
    }
    if(ok == 3)
      lcd.clear();
}

void ringModeReset()
{
  	lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Type password");
  	lcd.setCursor(0,1);
    lcd.print("____");
    int ok = passwordCheck();
  	if(ok == 1)
    {
      	lcd.clear();
      	ringModeSet();	
    }
    if(ok == 3)
      lcd.clear();
}

void wallRingSet()
{
  	lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Type password");
  	lcd.setCursor(0,1);
    lcd.print("____");
  	int ok = passwordCheck();
    if(ok == 1)
    {
      lcd.clear();
  		lcd.setCursor(0,0);
  		lcd.print("Wall ring?[0/1]");
  		lcd.setCursor(0,1);
  		lcd.print("_");
  		int i = 0;
      unsigned long start = millis();
  		while(i < 1 && millis() - start <= 20000)
    	{
      		char button = customKeypad.getKey();
      		if(button)
      		{
        		if(button == '0')
        		{
              afk = 0;
              lcd.setCursor(0,1);
        			lcd.print(button);
        			analogWrite(buzzerPin, 20);
        			delay(25);
        			analogWrite(buzzerPin, 0);
              delay(100);
          			wallRing = false;
                  	i++;
       			}
        		else if(button == '1')
            {	
              afk = 0;
              lcd.setCursor(0,1);
        			lcd.print(button);
        			analogWrite(buzzerPin, 20);
        			delay(25);
        			analogWrite(buzzerPin, 0);
              delay(100);
                  	wallRing = true;
                  	i++;
            }
      		}
    	}
      if(millis() - start <= 20000)
      {
            delay(500);
            analogWrite(buzzerPin, 20);
            delay(200);
            analogWrite(buzzerPin, 0);
            delay(100);
            lcd.clear();
      }
      else
      {  
        afk = 1;
        lcd.clear();
      }
  	
    }
    if(ok == 3)
    {  
      lcd.clear();
    }
}

void ringMode1()
{
	int alarmTone[] = {NOTE_C5, 
                    NOTE_CS5,
					NOTE_D5,
					NOTE_DS5,
					NOTE_E5,
					NOTE_F5,
					NOTE_FS5,
					NOTE_G5,
					NOTE_GS5,
					NOTE_A5,
					NOTE_AS5,
					NOTE_B5};
  	for (int thisNote = 0; thisNote < 12; thisNote++) 
    {
    	int noteDuration = 1000 / 10;
    	tone(buzzerPin, alarmTone[thisNote], noteDuration);
    	delay(50);
    	noTone(buzzerPin);
  	}
  	for (int thisNote = 11; thisNote >= 0; thisNote--) 
    {
    	int noteDuration = 1000 / 10;
    	tone(buzzerPin, alarmTone[thisNote], noteDuration);
    	delay(50);
    	noTone(buzzerPin);
  	}
    

}

void ringMode2()
{
  int alarmTone[] = {NOTE_C5, 
                     NOTE_C6};
  	for (int thisNote = 0; thisNote < 2; thisNote++) 
    {
    	int noteDuration = 1000 / 2;
    	tone(buzzerPin, alarmTone[thisNote], noteDuration);
    	delay(1000);
    	noTone(buzzerPin);
  	}
}

int passwordCheck()
{
    int ok = 1;
  	int j = 0;
    unsigned long start = millis();
  	while (j < 4 && millis() - start <= 20000)
    {
      	char button = customKeypad.getKey();
      	if(button)
        {
          	lcd.setCursor(j, 1);
          	analogWrite(buzzerPin, 20);
          	delay(25);
          	analogWrite(buzzerPin, 0);
            delay(100);
          	lcd.print(button);
          	if(button != password[j])
				        ok = 0;
          	j++;
        }
    }
    if(millis() - start >= 20000)
    {    
      afk = 1;
    }
  	if(ok == 0 && millis() - start <= 20000)
    	displayWrong();
    else if(ok == 1 && millis() - start <= 20000)
      return 1;
    else if(millis() - start >= 20000)
      return 3;
}

void buttonCheck()
{
    char button = customKeypad.getKey();
    if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
  delay(10);
  if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
  delay(10);
  if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
  if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
  delay(10);
  if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
  delay(10);
  if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
  delay(10);
  if(button)
   {   
    afk = 0;
    digitalWrite(lcd_backlightPin, HIGH);
   }
  delay(10);


}

void displayWrong()
{
  		lcd.clear();
      	lcd.setCursor(0,0);
      	lcd.print("Wrong password");
      	analogWrite(buzzerPin, 20);
      	delay(200);
      	analogWrite(buzzerPin, 0);
      	delay(100);
      	analogWrite(buzzerPin, 20);
      	delay(200);
      	analogWrite(buzzerPin, 0);
      	delay(1000);
      	lcd.clear();
  
}
                     