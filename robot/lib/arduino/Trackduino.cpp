 #include <Arduino.h>
 #include <TimerOne.h>
 #include <TimerThree.h>
 //#include <PinChangeInt.h>
 #include <EnableInterrupt.h>
 #include <OneWire.h>
 #include <Sentry.h>
 #include <Trackcv.h>
 #include <U8glib.h>

 
 extern "C" { 
#include "trackcv/xprintf.h"

}


 #include <SoftPWM.h>
 #include "Wire.h"

 #include "1rus4x6.h"
 #include "2rus5x7.h"
 #include "3rus7x13.h"
 #include "4rusUnifont.h"
 #include "5rus9x15.h"
 #include "6rus10x20.h"
 // #include "ArduinoJson.h"

 // void test()

 // {digitalWrite(13, HIGH);
 // 	delay(1000);
 // 	digitalWrite(13,LOW);
 // 	delay(1000);}

 // i2c address of MPU (gyroscope)
 
 typedef Sentry2 Sentry;
 Sentry sen;
 U8GLIB_SSD1306_128X64 ubg(U8G_I2C_OPT_NONE);
 
 #define MPU (0x68)
 #define ACCEL_XOUT_H (0x3b)
 #define ACCEL_XOUT_L (0x3c)
 #define ACCEL_YOUT_H (0x3d)
 #define ACCEL_YOUT_L (0x3e)
 #define ACCEL_ZOUT_H (0x3f)
 #define ACCEL_ZOUT_L (0x40)
 #define TEMP_OUT_H (0x41)
 #define TEMP_OUT_L (0x42)
 #define GYRO_XOUT_H (0x43)
 #define GYRO_XOUT_L (0x44)
 #define GYRO_YOUT_H (0x45)
 #define GYRO_YOUT_L (0x46)
 #define GYRO_ZOUT_H (0x47)
 #define GYRO_ZOUT_L (0x48)

 #define READ_2_BYTES (Wire.read() << 8 | Wire.read())
 
 #define SENTRY_I2C
#define VISION_MASK_FACE Sentry::kVisionFace
#define VISION_MASK_BLOB Sentry::kVisionBlob
#define VISION_MASK_CARD Sentry::kVisionCard
#define VISION_MASK_QR Sentry::kVisionQrCode
#define VISION_MASK_LINE Sentry::kVisionLine
#define VISION_MASK_20CLASSES Sentry::kVision20Classes
#define VISION_MASK_APRILTAG Sentry::kVisionAprilTag
#define VISION_MASK_COLOR Sentry::kVisionColor
#define VISION_MASK_LEARNING Sentry::kVisionLearning
#define VISION_MASK_MOTION Sentry::kVisionMotionDetect



 void setupGyro(void) {
     Wire.begin();
     Wire.beginTransmission(MPU);
     Wire.write(0x6b);
     Wire.write(0);
     Wire.endTransmission(true);
 }

 int GyroScan(gyroAxis axis) {
     Wire.beginTransmission(MPU);
     Wire.write(GYRO_XOUT_H);
     Wire.endTransmission(false);
     Wire.requestFrom(MPU, 6, true);
     int X = READ_2_BYTES;
     int Y = READ_2_BYTES;
     int Z = READ_2_BYTES;
     switch (axis) {
         case GYRO_X_AXIS:
             return X;
         case GYRO_Y_AXIS:
             return Y;
         case GYRO_Z_AXIS:
             return Z;
     }
 }

 void trackduinoInit(void) {
     pinMode(RGB_BLUE, OUTPUT);
     pinMode(RGB_RED, OUTPUT);
     pinMode(RGB_GREEN, OUTPUT);

     digitalWrite(RGB_BLUE, LOW);
     digitalWrite(RGB_RED, LOW);
     digitalWrite(RGB_GREEN, LOW);

     pinMode(MOTOR1_IN1, OUTPUT);
     pinMode(MOTOR1_IN2, OUTPUT);
     pinMode(MOTOR1_EN, OUTPUT);
     pinMode(MOTOR2_IN1, OUTPUT);
     pinMode(MOTOR2_IN2, OUTPUT);
     pinMode(MOTOR2_EN, OUTPUT);
     pinMode(MOTOR3_IN1, OUTPUT);
     pinMode(MOTOR3_IN2, OUTPUT);
     pinMode(MOTOR3_EN, OUTPUT);
     pinMode(MOTOR4_IN1, OUTPUT);
     pinMode(MOTOR4_IN2, OUTPUT);
     pinMode(MOTOR4_EN, OUTPUT);

     digitalWrite(MOTOR1_IN1, LOW);
     digitalWrite(MOTOR1_IN2, HIGH);
     digitalWrite(MOTOR1_EN, LOW);
     digitalWrite(MOTOR2_IN1, LOW);
     digitalWrite(MOTOR2_IN2, HIGH);
     digitalWrite(MOTOR2_EN, LOW);
     digitalWrite(MOTOR3_IN1, LOW);
     digitalWrite(MOTOR3_IN2, HIGH);
     digitalWrite(MOTOR3_EN, LOW);
     digitalWrite(MOTOR4_IN1, LOW);
     digitalWrite(MOTOR4_IN2, HIGH);
     digitalWrite(MOTOR4_EN, LOW);

     pinMode(IN1, INPUT);
     pinMode(IN1INT, INPUT);
     pinMode(IN2, INPUT);
     pinMode(IN2INT, INPUT);
     pinMode(IN3, INPUT);
     pinMode(IN3INT, INPUT);
     pinMode(IN4, INPUT);
     pinMode(IN4INT, INPUT);
     pinMode(IN5, INPUT);
     pinMode(IN6, INPUT);
     pinMode(IN7, INPUT);
     pinMode(IN8, INPUT);

     pinMode(OUT1, OUTPUT);
     pinMode(OUT2, OUTPUT);
     pinMode(OUT3, OUTPUT);
     pinMode(OUT4, OUTPUT);
     pinMode(OUT5, OUTPUT);
     pinMode(OUT6, OUTPUT);
     pinMode(OUT7, OUTPUT);
     pinMode(OUT8, OUTPUT);

     digitalWrite(OUT1, LOW);
     digitalWrite(OUT2, LOW);
     digitalWrite(OUT3, LOW);
     digitalWrite(OUT4, LOW);
     digitalWrite(OUT5, LOW);
     digitalWrite(OUT6, LOW);
     digitalWrite(OUT7, LOW);
     digitalWrite(OUT8, LOW);

     pinMode(BTN_UP, INPUT_PULLUP);
     pinMode(BTN_DOWN, INPUT_PULLUP);
     pinMode(BTN_LEFT, INPUT_PULLUP);
     pinMode(BTN_RIGHT, INPUT_PULLUP);
     pinMode(BTN_CENTER, INPUT_PULLUP);

     pinMode(SOUND_PIN, OUTPUT);
     digitalWrite(SOUND_PIN, LOW);

     pinMode(BT_SW, OUTPUT);
     // digitalWrite(BT_SW,LOW);

     pinMode(13, OUTPUT);
     digitalWrite(13, LOW);

     // Timer3.initialize(1000);         // initialize timer1 to overflow every ...
     // Timer3.attachInterrupt(motorsHandler);  // attaches motorsHandler() to a timer overflow interrupt

     // SoftPWMBegin();
 }

 void stopProgram() {
     while (1) {
     }
 }

 // -----------Working with quadratic Encoder--------------
 // the user should define ENC_CH_A and ENC_CH_B. By default IN1 and IN2 correspondigly
 // to understand how this code is working, please read this article: easyelectronics.ru/avr-uchebnyj-kurs-inkrementalnyj-enkoder.html

 #define ENC1_STATUS ((digitalRead(ENC1_CH_B) << 1) + digitalRead(ENC1_CH_A))
 #define ENC2_STATUS ((digitalRead(ENC2_CH_B) << 1) + digitalRead(ENC2_CH_A))
 #define ENC3_STATUS ((digitalRead(ENC3_CH_B) << 1) + digitalRead(ENC3_CH_A))
 #define ENC4_STATUS ((digitalRead(ENC4_CH_B) << 1) + digitalRead(ENC4_CH_A))

 int ENC1_CH_A;
 int ENC1_CH_B;
 int ENC2_CH_A;
 int ENC2_CH_B;
 int ENC3_CH_A;
 int ENC3_CH_B;
 int ENC4_CH_A;
 int ENC4_CH_B;

 volatile long Enc1Data = 0;
 volatile long Enc2Data = 0;
 volatile long Enc3Data = 0;
 volatile long Enc4Data = 0;

 void setEncToZero(int encoder) {
     switch (encoder) {
         case 1: {
             Enc1Data = 0;
             break;
         }
         case 2: {
             Enc2Data = 0;
             break;
         }
         case 3: {
             Enc3Data = 0;
             break;
         }
         case 4: {
             Enc4Data = 0;
             break;
         }
     }
 }
 void clearMRTEncoder(int encoder) {setEncToZero(encoder);}

 long getEncDegrees(int encoder) {
     switch (encoder) {
         case 1: {
             return (Enc1Data * 5);
             break;
         }
         case 2: {
             return (Enc2Data * 5);
             break;
         }
         case 3: {
             return (Enc3Data * 5);
             break;
         }
         case 4: {
             return (Enc4Data * 5);
             break;
         }
     }
     // return (EncData*5); //we have 72 clicks per rotation, so 5 degrees per click
 }

 double getMRTState(int encoder) {
	 switch (encoder) {
         case 1: {
             return (Enc1Data * 1.82);
             break;
         }
         case 2: {
             return (Enc2Data* 1.82);
             break;
         }
         case 3: {
             return (Enc3Data* 1.82);
             break;
         }
         case 4: {
             return (Enc4Data* 1.82);
             break;
         }
     }
 }

 void Encoder1Scan(void) {
     static int Enc1State = ENC1_STATUS;
     int New = ENC1_STATUS;
     switch (Enc1State) {
         case 0b10: {
             if (New == 3) Enc1Data++;
             if (New == 0) Enc1Data--;
             break;
         }
         case 0b00: {
             if (New == 2) Enc1Data++;
             if (New == 1) Enc1Data--;
             break;
         }
         case 0b01: {
             if (New == 0) Enc1Data++;
             if (New == 3) Enc1Data--;
             break;
         }
         case 0b11: {
             if (New == 1) Enc1Data++;
             if (New == 2) Enc1Data--;
             break;
         }
     }
     Enc1State = New;
 }

 void Encoder2Scan(void) {
     static int Enc2State = ENC2_STATUS;
     int New = ENC2_STATUS;
     switch (Enc2State) {
         case 0b10: {
             if (New == 3) Enc2Data++;
             if (New == 0) Enc2Data--;
             break;
         }
         case 0b00: {
             if (New == 2) Enc2Data++;
             if (New == 1) Enc2Data--;
             break;
         }
         case 0b01: {
             if (New == 0) Enc2Data++;
             if (New == 3) Enc2Data--;
             break;
         }
         case 0b11: {
             if (New == 1) Enc2Data++;
             if (New == 2) Enc2Data--;
             break;
         }
     }
     Enc2State = New;
 }

 void Encoder3Scan(void) {
     static int Enc3State = ENC3_STATUS;
     int New = ENC3_STATUS;
     switch (Enc3State) {
         case 0b10: {
             if (New == 3) Enc3Data++;
             if (New == 0) Enc3Data--;
             break;
         }
         case 0b00: {
             if (New == 2) Enc3Data++;
             if (New == 1) Enc3Data--;
             break;
         }
         case 0b01: {
             if (New == 0) Enc3Data++;
             if (New == 3) Enc3Data--;
             break;
         }
         case 0b11: {
             if (New == 1) Enc3Data++;
             if (New == 2) Enc3Data--;
             break;
         }
     }
     Enc3State = New;
 }

 void Encoder4Scan(void) {
     static int Enc4State = ENC4_STATUS;
     int New = ENC4_STATUS;
     switch (Enc4State) {
         case 0b10: {
             if (New == 3) Enc4Data++;
             if (New == 0) Enc4Data--;
             break;
         }
         case 0b00: {
             if (New == 2) Enc4Data++;
             if (New == 1) Enc4Data--;
             break;
         }
         case 0b01: {
             if (New == 0) Enc4Data++;
             if (New == 3) Enc4Data--;
             break;
         }
         case 0b11: {
             if (New == 1) Enc4Data++;
             if (New == 2) Enc4Data--;
             break;
         }
     }
     Enc4State = New;
 }

 void Encoder1Count() {
	static int Enc1State = ENC1_STATUS;
     int New = ENC1_STATUS;
	 switch (Enc1State) {
		 case 2: {
	if (digitalRead(ENC1_CH_B) == HIGH && New==3) Enc1Data--;
	else if (digitalRead(ENC1_CH_B) == HIGH && New==0) Enc1Data++;
	else if (digitalRead(ENC1_CH_A) == HIGH && New==3) Enc1Data--;

		 }

		 case 0: {
	if (digitalRead(ENC1_CH_B) == HIGH && New==2) Enc1Data--;
	else if (digitalRead(ENC1_CH_B) == HIGH && New==1) Enc1Data++;
	else if (digitalRead(ENC1_CH_A) == HIGH && New==2) Enc1Data--;

		 }

		 case 1: {
	if (digitalRead(ENC1_CH_B) == HIGH && New==0) Enc1Data--;
	else if (digitalRead(ENC1_CH_B) == HIGH && New==3) Enc1Data++;
	else if (digitalRead(ENC1_CH_A) == HIGH && New==0) Enc1Data--;

		 }

		 case 3: {
	if (digitalRead(ENC1_CH_B) == HIGH && New==1) Enc1Data--;
	else if (digitalRead(ENC1_CH_B) == HIGH && New==2) Enc1Data++;
	else if (digitalRead(ENC1_CH_A) == HIGH && New==1) Enc1Data--;

		 }
 }
 Enc1State = New;
 }

 void Encoder2Count() {
	static int Enc2State = ENC2_STATUS;
     int New = ENC2_STATUS;
	 switch (Enc2State) {
		 case 2: {
	if (digitalRead(ENC2_CH_B) == HIGH && New==3) Enc2Data--;
	else if (digitalRead(ENC2_CH_B) == HIGH && New==0) Enc2Data++;
	else if (digitalRead(ENC2_CH_A) == HIGH && New==3) Enc2Data--;

		 }

		 case 0: {
	if (digitalRead(ENC2_CH_B) == HIGH && New==2) Enc2Data--;
	else if (digitalRead(ENC2_CH_B) == HIGH && New==1) Enc2Data++;
	else if (digitalRead(ENC2_CH_A) == HIGH && New==2) Enc2Data--;

		 }

		 case 1: {
	if (digitalRead(ENC2_CH_B) == HIGH && New==0) Enc2Data--;
	else if (digitalRead(ENC2_CH_B) == HIGH && New==3) Enc2Data++;
	else if (digitalRead(ENC2_CH_A) == HIGH && New==0) Enc2Data--;

		 }

		 case 3: {
	if (digitalRead(ENC2_CH_B) == HIGH && New==1) Enc2Data--;
	else if (digitalRead(ENC2_CH_B) == HIGH && New==2) Enc2Data++;
	else if (digitalRead(ENC2_CH_A) == HIGH && New==1) Enc2Data--;

		 }
 }
 Enc2State = New;
 }

 void Encoder3Count() {
	static int Enc3State = ENC3_STATUS;
     int New = ENC3_STATUS;
	 switch (Enc3State) {
		 case 2: {
	if (digitalRead(ENC3_CH_B) == HIGH && New==3) Enc3Data--;
	else if (digitalRead(ENC3_CH_B) == HIGH && New==0) Enc3Data++;
	else if (digitalRead(ENC3_CH_A) == HIGH && New==3) Enc3Data--;

		 }

		 case 0: {
	if (digitalRead(ENC3_CH_B) == HIGH && New==2) Enc3Data--;
	else if (digitalRead(ENC3_CH_B) == HIGH && New==1) Enc3Data++;
	else if (digitalRead(ENC3_CH_A) == HIGH && New==2) Enc3Data--;

		 }

		 case 1: {
	if (digitalRead(ENC3_CH_B) == HIGH && New==0) Enc3Data--;
	else if (digitalRead(ENC3_CH_B) == HIGH && New==3) Enc3Data++;
	else if (digitalRead(ENC3_CH_A) == HIGH && New==0) Enc3Data--;

		 }

		 case 3: {
	if (digitalRead(ENC3_CH_B) == HIGH && New==1) Enc3Data--;
	else if (digitalRead(ENC3_CH_B) == HIGH && New==2) Enc3Data++;
	else if (digitalRead(ENC3_CH_A) == HIGH && New==1) Enc3Data--;

		 }
 }
 Enc3State = New;
 }

 void Encoder4Count() {
	static int Enc4State = ENC4_STATUS;
     int New = ENC4_STATUS;
	 switch (Enc4State) {
		 case 2: {
	if (digitalRead(ENC4_CH_B) == HIGH && New==3) Enc4Data--;
	else if (digitalRead(ENC4_CH_B) == HIGH && New==0) Enc4Data++;
	else if (digitalRead(ENC4_CH_A) == HIGH && New==3) Enc4Data--;

		 }

		 case 0: {
	if (digitalRead(ENC4_CH_B) == HIGH && New==2) Enc4Data--;
	else if (digitalRead(ENC4_CH_B) == HIGH && New==1) Enc4Data++;
	else if (digitalRead(ENC4_CH_A) == HIGH && New==2) Enc4Data--;

		 }

		 case 1: {
	if (digitalRead(ENC4_CH_B) == HIGH && New==0) Enc4Data--;
	else if (digitalRead(ENC4_CH_B) == HIGH && New==3) Enc4Data++;
	else if (digitalRead(ENC4_CH_A) == HIGH && New==0) Enc4Data--;

		 }

		 case 3: {
	if (digitalRead(ENC4_CH_B) == HIGH && New==1) Enc4Data--;
	else if (digitalRead(ENC4_CH_B) == HIGH && New==2) Enc4Data++;
	else if (digitalRead(ENC4_CH_A) == HIGH && New==1) Enc4Data--;

		 }
 }
 Enc4State = New;
 }

 void setupEnc(int encoder, int CH_A, int CH_B) {
     // attachInterrupt(digitalPinToInterrupt(ENC_CH_A_INT), EncoderScan, CHANGE);
     // attachInterrupt(digitalPinToInterrupt(ENC_CH_B_INT), EncoderScan, CHANGE);
     switch (encoder) {
         case 1: {
             ENC1_CH_A = CH_A;
             ENC1_CH_B = CH_B;
             // attachPinChangeInterrupt(ENC1_CH_A,Encoder1Scan,CHANGE);
             // attachPinChangeInterrupt(ENC1_CH_B,Encoder1Scan,CHANGE);
             enableInterrupt(ENC1_CH_A, Encoder1Scan, CHANGE);
             enableInterrupt(ENC1_CH_B, Encoder1Scan, CHANGE);

             break;
         }
         case 2: {
             ENC2_CH_A = CH_A;
             ENC2_CH_B = CH_B;
             // attachPinChangeInterrupt(ENC2_CH_A,Encoder2Scan,CHANGE);
             // attachPinChangeInterrupt(ENC2_CH_B,Encoder2Scan,CHANGE);
             enableInterrupt(ENC2_CH_A, Encoder2Scan, CHANGE);
             enableInterrupt(ENC2_CH_B, Encoder2Scan, CHANGE);
             break;
         }
         case 3: {
             ENC3_CH_A = CH_A;
             ENC3_CH_B = CH_B;
             // attachPinChangeInterrupt(ENC3_CH_A,Encoder3Scan,CHANGE);
             // attachPinChangeInterrupt(ENC3_CH_B,Encoder3Scan,CHANGE);
             enableInterrupt(ENC3_CH_A, Encoder3Scan, CHANGE);
             enableInterrupt(ENC3_CH_B, Encoder3Scan, CHANGE);
             break;
         }
         case 4: {
             ENC4_CH_A = CH_A;
             ENC4_CH_B = CH_B;
             // attachPinChangeInterrupt(ENC4_CH_A,Encoder4Scan,CHANGE);
             // attachPinChangeInterrupt(ENC4_CH_B,Encoder4Scan,CHANGE);
             enableInterrupt(ENC4_CH_A, Encoder4Scan, CHANGE);
             enableInterrupt(ENC4_CH_B, Encoder4Scan, CHANGE);
             break;
         }
     }
 }

 void SetupMRTEncoder(int encoder, int CH_A, int CH_B) {
     switch (encoder) {
         case 1: {
             ENC1_CH_A = CH_A;
             ENC1_CH_B = CH_B;
             // attachPinChangeInterrupt(ENC1_CH_A,Encoder1Scan,CHANGE);
             // attachPinChangeInterrupt(ENC1_CH_B,Encoder1Scan,CHANGE);
			 //ENC_STATUS, ENC_CH_B, ENC_CH_A, *ENC_DATA
             enableInterrupt(ENC1_CH_A, Encoder1Count, CHANGE);
             enableInterrupt(ENC1_CH_B, Encoder1Count, CHANGE);

             break;
         }
         case 2: {
             ENC2_CH_A = CH_A;
             ENC2_CH_B = CH_B;
             // attachPinChangeInterrupt(ENC2_CH_A,Encoder2Scan,CHANGE);
             // attachPinChangeInterrupt(ENC2_CH_B,Encoder2Scan,CHANGE);
             enableInterrupt(ENC2_CH_A, Encoder2Count, CHANGE);
             enableInterrupt(ENC2_CH_B, Encoder2Count, CHANGE);
             break;
         }
         case 3: {
             ENC3_CH_A = CH_A;
             ENC3_CH_B = CH_B;
             // attachPinChangeInterrupt(ENC3_CH_A,Encoder3Scan,CHANGE);
             // attachPinChangeInterrupt(ENC3_CH_B,Encoder3Scan,CHANGE);
             enableInterrupt(ENC3_CH_A, Encoder3Count, CHANGE);
             enableInterrupt(ENC3_CH_B, Encoder3Count, CHANGE);
             break;
         }
         case 4: {
             ENC4_CH_A = CH_A;
             ENC4_CH_B = CH_B;
             // attachPinChangeInterrupt(ENC4_CH_A,Encoder4Scan,CHANGE);
             // attachPinChangeInterrupt(ENC4_CH_B,Encoder4Scan,CHANGE);
             enableInterrupt(ENC4_CH_A, Encoder4Count, CHANGE);
             enableInterrupt(ENC4_CH_B, Encoder4Count, CHANGE);
             break;
         }
     }
 }

 //============== Motors stuff========================

 volatile int newMotorSpeed[4] = {0, 0, 0, 0};  //сюда записываются "целевые" значения скорости
 volatile int currentMotorSpeed[4] = {0, 0, 0, 0};  //текущие значения скорости, первый элемент не используется

 void motorsAccelerationSetup(unsigned int millisToFullSpeed) {
     unsigned long newTimerValue = (millisToFullSpeed * 10);
     Timer3.initialize(newTimerValue);
 }

 void motorsHandler() {
     for (int i = 0; i < 4; i++) {
         if (newMotorSpeed[i] > currentMotorSpeed[i]) {
             currentMotorSpeed[i]++;
             changeMotorSpeed(i, currentMotorSpeed[i]);
         }

         else if (newMotorSpeed[i] < currentMotorSpeed[i]) {
             currentMotorSpeed[i]--;
             changeMotorSpeed(i, currentMotorSpeed[i]);
         }

         else if (newMotorSpeed[i] = currentMotorSpeed[i]) {
             // do nothing
         }

         // Serial.print(currentMotorSpeed[i]);
         // Serial.print("/");
         // Serial.print(newMotorSpeed[i]);
         // Serial.print("\t");
     }
     // Serial.println("");
 }

 void changeMotorSpeed(int motorPort, int speed) {
     if (speed >= 0) {
         changeMotorDirection(motorPort, 0);
     } else if (speed < 0) {
         changeMotorDirection(motorPort, 1);
     }

     switch (motorPort) {
         case 0:
             analogWrite(MOTOR1_EN, abs(map(speed, 0, 100, 0, 255)));
             break;

         case 1:
             analogWrite(MOTOR2_EN, abs(map(speed, 0, 100, 0, 255)));
             break;

         case 2:
             analogWrite(MOTOR3_EN, abs(map(speed, 0, 100, 0, 255)));
             break;

         case 3:
             analogWrite(MOTOR4_EN, abs(map(speed, 0, 100, 0, 255)));
             break;
     }
     // Serial.println(speed);
 }

 void changeMotorDirection(int motorPort, bool direction) {
     switch (motorPort) {
         case 0:
             switch (direction) {
                 case 0:
                     digitalWrite(MOTOR1_IN1, HIGH);
                     digitalWrite(MOTOR1_IN2, LOW);
                     break;

                 case 1:
                     digitalWrite(MOTOR1_IN1, LOW);
                     digitalWrite(MOTOR1_IN2, HIGH);
                     break;
                     break;
             }

         case 1:
             switch (direction) {
                 case 0:
                     digitalWrite(MOTOR2_IN1, HIGH);
                     digitalWrite(MOTOR2_IN2, LOW);
                     break;

                 case 1:
                     digitalWrite(MOTOR2_IN1, LOW);
                     digitalWrite(MOTOR2_IN2, HIGH);
                     break;
                     break;
             }

         case 2:
             switch (direction) {
                 case 0:
                     digitalWrite(MOTOR3_IN1, HIGH);
                     digitalWrite(MOTOR3_IN2, LOW);
                     break;

                 case 1:
                     digitalWrite(MOTOR3_IN1, LOW);
                     digitalWrite(MOTOR3_IN2, HIGH);
                     break;
                     break;
             }

         case 3:
             switch (direction) {
                 case 0:
                     digitalWrite(MOTOR4_IN1, HIGH);
                     digitalWrite(MOTOR4_IN2, LOW);
                     break;

                 case 1:
                     digitalWrite(MOTOR4_IN1, LOW);
                     digitalWrite(MOTOR4_IN2, HIGH);
                     break;
                     break;
             }
     }
 }

  void setDate(short day, short month, short year) {
Wire.begin();
	 Wire.beginTransmission(ADR_TIMESENSOR); // адрес устройства ADR_TIMESENSOR
  Wire.write(REG_SETDATE);        // Передаем команду установки даты
  Wire.write(day);        // День (1-31)
  Wire.write(month);        // Месяц (1-12)
  Wire.write(year);        // Год (0-99)
  Wire.endTransmission();
  }

  void setTime(short hours, short minutes, short seconds) {
	  Wire.begin();
	  Wire.beginTransmission(ADR_TIMESENSOR); // адрес устройства ADR_TIMESENSOR
  Wire.write(REG_SETTIME);        // Передаем команду установки времени
  Wire.write(hours);        // Час (0-23)
  Wire.write(minutes);        // Минута (0-59)
  Wire.write(seconds);        // Секунда (0-59)
  Wire.endTransmission();
  }

  int getMonth() {
	  Wire.begin();
	  Wire.beginTransmission(ADR_TIMESENSOR);
  Wire.write(REG_FIRST);
  Wire.endTransmission();
  Wire.requestFrom(ADR_TIMESENSOR, 6);
  int month;

 for (int i=0;i<2;i++) {
	 month = Wire.read();
 }
 return month;
  }

  int getDay() {
	  Wire.begin();
	  Wire.beginTransmission(ADR_TIMESENSOR);
  Wire.write(REG_FIRST);
  Wire.endTransmission();
  Wire.requestFrom(ADR_TIMESENSOR, 6);

 return (int)Wire.read();
  }

  int getYear() {
	  Wire.begin();
	  Wire.beginTransmission(ADR_TIMESENSOR);
  Wire.write(REG_FIRST);
  Wire.endTransmission();
  Wire.requestFrom(ADR_TIMESENSOR, 6);
  int year;

 for (int i=0;i<3;i++) {
	 year = Wire.read();
 }
 return year+2000;
  }

  int getHour() {
	  Wire.begin();
	  Wire.beginTransmission(ADR_TIMESENSOR);
  Wire.write(REG_FIRST);
  Wire.endTransmission();
  Wire.requestFrom(ADR_TIMESENSOR, 6);
  int hours;

 for (int i=0;i<4;i++) {
	 hours = Wire.read();
 }
 return hours;
  }

  int getMinute() {
	  Wire.begin();
	  Wire.beginTransmission(ADR_TIMESENSOR);
  Wire.write(REG_FIRST);
  Wire.endTransmission();
  Wire.requestFrom(ADR_TIMESENSOR, 6);
  int mins;

 for (int i=0;i<5;i++) {
	 mins = Wire.read();
 }
 return mins;
  }

  int getSecond() {
	  Wire.begin();
	  Wire.beginTransmission(ADR_TIMESENSOR);
  Wire.write(REG_FIRST);
  Wire.endTransmission();
  Wire.requestFrom(ADR_TIMESENSOR, 6);
  int secs;

 for (int i=0;i<6;i++) {
	 secs = Wire.read();
 }
 return secs;
  }
  
  int convertStrToInt(char* str) {
    int result = atoi(str);
    if (result == 0 && str[0] != '0') {
        return -1;
    }
    return result;
}
 
  bool compareStrings(char* str1, char* str2) {
	  return strcmp(str1, str2) == 0;
  }
  
  int n = 128;

int pos(char *s, char *c, int n)
{
 int i, j; // Счетчики для циклов
 int lenC, lenS; // Длины строк
 
 //Находим размеры строки исходника и искомого
 for (lenC = 0; c[lenC]; lenC++);
 for (lenS = 0; s[lenS]; lenS++);
 
 for (i = 0; i <= lenS - lenC; i++) // Пока есть возможность поиска
 {
 for (j = 0; s[i + j] == c[j]; j++); // Проверяем совпадение посимвольно
 // Если посимвольно совпадает по длине искомого
 // Вернем из функции номер ячейки, откуда начинается совпадение
 // Учитывать 0-терминатор  ( '\0' )
 if (j - lenC == 1 && i == lenS - lenC && !(n - 1)) return i;
 if (j == lenC)
 if (n - 1) n--; 
 else return i;
 }
 //Иначе вернем -1 как результат отсутствия подстроки
 return -1;
}

bool findSubstring(char* str1, char* str2) {
	int i, n = 0;
for (i = 1; n != -1; i++)
{
 n = pos(str2, str1, i);
 
 if (n >= 0)
 return true;
}

return false;
}
  
  int CURRENT_MASK = 0;
  
  bool startCamera() {
      #ifdef SENTRY_I2C
          Wire.begin();
          if (SENTRY_OK != sen.begin(&Wire)) {
              // Обработка ошибки: выводим сообщение или предпринимаем другие действия
              Serial.println("Ошибка инициализации камеры");
              return false;
          }
      #else
          // Если SENTRY_I2C не определено, выводим предупреждение или предпринимаем другие действия
          Serial.println("SENTRY_I2C не определен");
          return false;
      #endif

     Serial.println("Успешное инициализирование камеры");

      return true; // Возвращаем true, если камера запущена успешно
  }
  
  void StartSmartCameraMode(int mode) {
			
		while (!startCamera()) {
		builtInRGB(RED);
		}
		builtInRGB(OFF);

		switch(mode) {
		case 0:
		{
			if (CURRENT_MASK!=VISION_MASK_FACE || !sen.VisionGetStatus(VISION_MASK_FACE)) {
			sen.VisionEnd(CURRENT_MASK);
			sen.VisionBegin(VISION_MASK_FACE);
			CURRENT_MASK = VISION_MASK_FACE;
			}
			break;
		}
		case 1:
		{
			if (CURRENT_MASK!=VISION_MASK_BLOB || !sen.VisionGetStatus(VISION_MASK_BLOB)) {
			sen.VisionEnd(CURRENT_MASK);
			sen.CameraSetAwb(kLockWhiteBalance);
			 sen.VisionBegin(VISION_MASK_BLOB);
			CURRENT_MASK = VISION_MASK_BLOB; 
			}
			break;
		}
	    case 2:
	    {
			if (CURRENT_MASK!=VISION_MASK_CARD || !sen.VisionGetStatus(VISION_MASK_CARD)) {
			sen.VisionEnd(CURRENT_MASK);
			sen.VisionBegin(VISION_MASK_CARD);
			CURRENT_MASK = VISION_MASK_CARD; 
			}
			break;
		}
		case 3:
		{
			if (CURRENT_MASK!=VISION_MASK_QR || !sen.VisionGetStatus(VISION_MASK_QR)) {
			sen.VisionEnd(CURRENT_MASK);
			sen.VisionBegin(VISION_MASK_QR);
			CURRENT_MASK = VISION_MASK_QR; 
			}
			break;
		}
		case 4:
		{
			if (CURRENT_MASK!=VISION_MASK_LINE || !sen.VisionGetStatus(VISION_MASK_LINE)) {

				sen.VisionEnd(CURRENT_MASK);
				sen.VisionBegin(VISION_MASK_LINE);
			    CURRENT_MASK = VISION_MASK_LINE;
			}
			break;
		}
		case 5:
		{
			if (CURRENT_MASK!=VISION_MASK_20CLASSES || !sen.VisionGetStatus(VISION_MASK_20CLASSES)) {

				sen.VisionEnd(CURRENT_MASK);
				sen.VisionBegin(VISION_MASK_20CLASSES);
				CURRENT_MASK = VISION_MASK_20CLASSES;
			}
			break;
		}
		case 6:
		{
			if (CURRENT_MASK!=VISION_MASK_APRILTAG || !sen.VisionGetStatus(VISION_MASK_APRILTAG)) {

				sen.VisionEnd(CURRENT_MASK);
				sen.VisionBegin(VISION_MASK_APRILTAG);
				CURRENT_MASK = VISION_MASK_APRILTAG;
			}
			break;
		}
		case 7:
		{
			if (CURRENT_MASK!=VISION_MASK_COLOR || !sen.VisionGetStatus(VISION_MASK_COLOR)) {

				sen.VisionEnd(CURRENT_MASK);
				sen.SeneorSetCoordinateType(kAbsoluteCoordinate);
				sen.VisionBegin(VISION_MASK_COLOR);
				CURRENT_MASK = VISION_MASK_COLOR;
			}
			break;
		}
		case 8:
		{
			if (CURRENT_MASK!=VISION_MASK_LEARNING || !sen.VisionGetStatus(VISION_MASK_LEARNING)) {

				sen.VisionEnd(CURRENT_MASK);
				sen.VisionBegin(VISION_MASK_LEARNING);
				CURRENT_MASK = VISION_MASK_LEARNING;
			}
			break;
		}
		case 9:
		{
			if (CURRENT_MASK!=VISION_MASK_MOTION || !sen.VisionGetStatus(VISION_MASK_MOTION)) {

				sen.VisionEnd(CURRENT_MASK);
				sen.VisionBegin(VISION_MASK_MOTION);
				CURRENT_MASK = VISION_MASK_MOTION;
			}
			break;
		} 		
  }
  }

  short countParam = 1;
  void setBlobParam(int width, int height, int color) {
	  
	  
		  StartSmartCameraMode(1);
	  
		  sentry_object_t param = {0};
		  sen.SetParamNum(VISION_MASK_BLOB, countParam);
			param.width = width;
			param.height = height;
			param.label = color;
			sen.SetParam(VISION_MASK_BLOB, &param, countParam);
			 countParam++;
			
	  
  }
  
  short colorParam = 1;
  void setColorParam(int w, int h, int x, int y) {
	  
	  
		  StartSmartCameraMode(7);
	
		  sentry_object_t param;
		  sen.SetParamNum(VISION_MASK_COLOR, colorParam);
		    param.x_value = x;
			param.y_value = y;
			param.width = w;
			param.height = h;
			sen.SetParam(VISION_MASK_COLOR, &param, colorParam);
			colorParam++;
	  
  }
  
  int blobColorRecognition(int num) {
	  
	  if (num && num<=objectsAmountInFrame()) {
		if (CURRENT_MASK==VISION_MASK_BLOB) 
			return sen.GetValue(VISION_MASK_BLOB, kLabel, num);
	  }
	return -1;
  }
  
    int colorRecognition(int num) {
		
	  if (num && num<=objectsAmountInFrame()) {
		if (CURRENT_MASK==VISION_MASK_COLOR) 
			return sen.GetValue(VISION_MASK_COLOR, kLabel, num);
	  }
	return -1;
  }
  
  int classNumberRecognition(int num) {
	  
	  if (num && num<=objectsAmountInFrame()) {
		if (CURRENT_MASK==VISION_MASK_20CLASSES) 
			return sen.GetValue(VISION_MASK_20CLASSES, kLabel, num);
	  }
	return -1;
  }
  
  int apriltagNumberRecognition(int num) {
	  
	  if (num && num<=objectsAmountInFrame()) {
		if (CURRENT_MASK==VISION_MASK_APRILTAG) 
			return sen.GetValue(VISION_MASK_APRILTAG, kLabel, num);
	  }
	return -1;
  }
  
    int cardClassRecognition(int num) {
		
		if (num && num<=objectsAmountInFrame()) {
		if (CURRENT_MASK==VISION_MASK_CARD) 
			return sen.GetValue(VISION_MASK_CARD, kLabel, num);
	  }
	return -1;
	}

	int objectsAmountInFrame() {
		
		if (CURRENT_MASK==0) return 0;
			int obj_num = sen.GetValue(CURRENT_MASK, kStatus);
			
			return obj_num;
	}
	
	  bool isBlobRecognized(int color) {
		  
		  if (CURRENT_MASK==VISION_MASK_BLOB) {
			 for (int i = 1; i <= objectsAmountInFrame(); ++i) {
				 if (sen.GetValue(VISION_MASK_BLOB, kLabel, i) == color) return true;
			 }
		 }
		 return false;
	  }
	  
	  bool isClassRecognized (int obj) {
		  
		  if (CURRENT_MASK==VISION_MASK_20CLASSES) {
			 for (int i = 1; i <= objectsAmountInFrame(); ++i) {
				 if (sen.GetValue(VISION_MASK_20CLASSES, kLabel, i) == obj) return true;
			 }
		 }
		 return false;
	  }
	
	 int getXCoordinate(int obj_num) {
		 
	 if (objectsAmountInFrame() && objectsAmountInFrame()>=obj_num) 
		 return sen.GetValue(CURRENT_MASK, kXValue, obj_num);
	 return -1;
	 }
	 
	 int getX1Coordinate(int obj_num) {
		 
	 if (objectsAmountInFrame() && objectsAmountInFrame()>=obj_num && CURRENT_MASK==VISION_MASK_LINE) 
		 return sen.GetValue(VISION_MASK_LINE, kXValue, obj_num);
	 return -1;
	 }
	 
	 int getX2Coordinate(int obj_num) {
		 
	 if (objectsAmountInFrame() && objectsAmountInFrame()>=obj_num && CURRENT_MASK==VISION_MASK_LINE) 
		 return sen.GetValue(VISION_MASK_LINE, kWidthValue, obj_num);
	 return -1;
	 }
	 
	 int getY1Coordinate(int obj_num) {
		 
	 if (objectsAmountInFrame() && objectsAmountInFrame()>=obj_num && CURRENT_MASK==VISION_MASK_LINE) 
		 return sen.GetValue(VISION_MASK_LINE, kYValue, obj_num);
	 return -1;
	 }
	 
	 int getY2Coordinate(int obj_num) {
		 
	 if (objectsAmountInFrame() && objectsAmountInFrame()>=obj_num && CURRENT_MASK==VISION_MASK_LINE) 
		 return sen.GetValue(VISION_MASK_LINE, kHeightValue, obj_num);
	 return -1;
	 }
	 
	 int getYCoordinate(int obj_num) {
		 
	 if (objectsAmountInFrame() && objectsAmountInFrame()>=obj_num) 
		 return sen.GetValue(CURRENT_MASK, kYValue, obj_num);
	 return -1;
	 }
	 
	 int getWidthParam(int obj_num) {
		 
	 if (objectsAmountInFrame() && objectsAmountInFrame()>=obj_num) 
		 return sen.GetValue(CURRENT_MASK, kWidthValue, obj_num);
	 return -1;
	 }
	 
	 int getHeightParam(int obj_num) {
		 
	 if (objectsAmountInFrame() && objectsAmountInFrame()>=obj_num) 
		 return sen.GetValue(CURRENT_MASK, kHeightValue, obj_num);
	 return -1;
	 }
	 
	 void saveFace(uint8_t object_id) {
		 
		  deleteFace(object_id);

		 if (CURRENT_MASK==VISION_MASK_FACE && object_id>0 && object_id<=25) {
  sentry_object_t param = {0, 0, 0, 0, 0};
  param.label = 100;
  sen.SetParamNum(VISION_MASK_FACE, 5);
  sen.SetParam(VISION_MASK_FACE, &param, object_id);
}
	 }

	 void saveObject(uint8_t object_id) {

		 deleteObject(object_id);

		 if (CURRENT_MASK==VISION_MASK_LEARNING && object_id>0 && object_id<=25) {
  sentry_object_t param = {0, 0, 0, 0, 0};
  param.label = 100;
  sen.SetParamNum(VISION_MASK_LEARNING, 5);
  sen.SetParam(VISION_MASK_LEARNING, &param, object_id);
	 }
	 }

	 void deleteObject(uint8_t object_id) {
    if (CURRENT_MASK == VISION_MASK_LEARNING && object_id > 0 && object_id <= 25) {
        sentry_object_t empty_object = {}; // Создаем пустую структуру sentry_object_t
        sen.SetParam(VISION_MASK_LEARNING, &empty_object, object_id);
    }
}

	 void deleteFace(uint8_t object_id) {

		 if (CURRENT_MASK==VISION_MASK_FACE && object_id>0 && object_id<=25) {
			 sentry_object_t empty_object = {}; // Создаем пустую структуру sentry_object_t
        sen.SetParam(VISION_MASK_FACE, &empty_object, object_id);
	 }
	 }

	 void deleteAllObjects() {

		 if (CURRENT_MASK==VISION_MASK_LEARNING) {
			 for (int i=1; i<=25; i++) {
				 deleteObject(i);
			 }

		 }
	 }

	 void deleteAllFaces() {

		 if (CURRENT_MASK==VISION_MASK_FACE) {
			 for (int i=1; i<=25; i++) {
				 deleteFace(i);

		 }
		 	 }
	 }
	 
	 bool isFaceRecognized(int id) {
		 
		 if (id && id<=25 && CURRENT_MASK==VISION_MASK_FACE) {
			 for (int i = 1; i <= objectsAmountInFrame(); ++i) {
				 if (sen.GetValue(VISION_MASK_FACE, kLabel, i) == id) return true;
			 }
		 }
		 return false;
	 }
	 
	 bool isObjectRecognized(int id) {
		 
		 if (id && id<=25 && CURRENT_MASK==VISION_MASK_LEARNING) {
			 for (int i = 1; i <= objectsAmountInFrame(); ++i) {
				 if (sen.GetValue(VISION_MASK_LEARNING, kLabel, i) == id) return true;
			 }
		 }
		 return false;
	 }
	 
	 int trafficCard(int card) {
		 return card;
	 }
	 
	 int shapeCard(int card) {
		 return card;
	 }
	 
	 int numberCard(int card) {
		 return card;
	 }
	 
	 char* QrCodeRec() {
		 
		  if (CURRENT_MASK==VISION_MASK_QR) {
		 int obj_num = sen.GetValue(VISION_MASK_QR, kStatus);
		 if (obj_num)  return sen.GetQrCodeValue(); 
		  }
	 }
	 
	 int classDef(int obj) {
		 return obj;
	 }

  
  
   //void writeAngle(int angle, char portnum[]) {
//		servo.attach(portnum);
//		servo.write(angle/2);
//   }

 /* void startMotor (int motorPort, int speed)
 {
   //newMotorSpeed[(motorPort-1)] = constrain(speed,-100,100);
   newMotorSpeed[(motorPort-1)] = speed;
 } */

 // -----------1st-level Motor control functions------------
 void startMotor(int motorPort, int speed) {
     if (speed > 100) {
         speed = 100;
     }
     if (speed < -100) {
         speed = -100;
     }

     if (speed > 0) {
         speed = abs(speed);
         speed = map(speed, 0, 100, 0, 255);

         switch (motorPort) {
             case 1:
                 digitalWrite(MOTOR1_IN1, HIGH);
                 digitalWrite(MOTOR1_IN2, LOW);
                 analogWrite(MOTOR1_EN, speed);
                 break;

             case 2:
                 digitalWrite(MOTOR2_IN1, HIGH);
                 digitalWrite(MOTOR2_IN2, LOW);
                 analogWrite(MOTOR2_EN, speed);
                 break;

             case 3:
                 digitalWrite(MOTOR3_IN1, HIGH);
                 digitalWrite(MOTOR3_IN2, LOW);
                 analogWrite(MOTOR3_EN, speed);
                 break;

             case 4:
                 digitalWrite(MOTOR4_IN1, HIGH);
                 digitalWrite(MOTOR4_IN2, LOW);
                 analogWrite(MOTOR4_EN, speed);
                 break;
         }
     } else if (speed <= 0) {
         speed = abs(speed);
         speed = map(speed, 0, 100, 0, 255);

         switch (motorPort) {
             case 1:
                 digitalWrite(MOTOR1_IN1, LOW);
                 digitalWrite(MOTOR1_IN2, HIGH);
                 analogWrite(MOTOR1_EN, speed);
                 break;

             case 2:
                 digitalWrite(MOTOR2_IN1, LOW);
                 digitalWrite(MOTOR2_IN2, HIGH);
                 analogWrite(MOTOR2_EN, speed);
                 break;

             case 3:
                 digitalWrite(MOTOR3_IN1, LOW);
                 digitalWrite(MOTOR3_IN2, HIGH);
                 analogWrite(MOTOR3_EN, speed);
                 break;

             case 4:
                 digitalWrite(MOTOR4_IN1, LOW);
                 digitalWrite(MOTOR4_IN2, HIGH);
                 analogWrite(MOTOR4_EN, speed);
                 break;
         }
     }
 }
 
 void electromagnet(int port, bool mode) {
	if (mode) startMotor(port, 100); else stopMotor(port);
 }

 void rotateMRT(int motorPort, int speed) {startMotor(motorPort,speed);}
 
 
int angle_dec = 0; 

 void rotateMRT_to_angle(bool accuracy, int motor, int speed,  int angle) {

     double state = getMRTState(motor); //state of enc for beginning
     double res; //result angle
     angle = angle<0 ? -angle : angle; // |angle|
     bool fStop = false;
     bool bStop = false;

     //accuracy is high
     if (accuracy) {

         if (speed>0) {
             res = state+angle;

             //rotate the motor to needed angle
             while (res!=getMRTState(motor) || speed>15) {
                 if (getMRTState(motor)<res) {
                     rotateMRT(motor, speed);
                 } else {
                     stopMotorHard(motor);
                     fStop = true;
                     bStop = true;
                 }

                 if (getMRTState(motor)>res)
                     rotateMRT(motor, -speed);
                 else if (bStop) {
                     stopMotorHard(motor);
                     bStop = false;
                }
                if (fStop) {
                    speed>0 ? speed-- : speed++;
                    fStop = false;
                }
                delay(10);

//              check for min value
                 if (speed==10) break;
             }
         } else if (speed<0) {
             res = state-angle;
             while (res!=getMRTState(motor) || speed<-15) {
                 if (getMRTState(motor)>res) {
                     rotateMRT(motor, speed);
                 } else {
                     stopMotorHard(motor);
                     fStop = true;
                     bStop = true;
                 }
                 if (getMRTState(motor)<res)
                     rotateMRT(motor, -speed);
                 else if (bStop) {
                     stopMotorHard(motor);
                     bStop = false;
                 }
                 if (fStop) {
                     speed>0 ? speed-- : speed++;
                     fStop = false;
                     bStop = false;
                 }
                 delay(10);
                 if (speed==-10) break;
             }
         }
	}
	
	//accuracy is moderate
	else {
	    if (speed>0) {
	        res = state+angle;
	        while (getMRTState(motor)<res) rotateMRT(motor,speed);
			stopMotorHard(motor);
		} else {
			res = state-angle; 
			while (getMRTState(motor)>res) rotateMRT(motor,speed); 
			stopMotorHard(motor);
		}
	}
	
	//stop motor in the end of the function
	stopMotorHard(motor);

 }
 
 void rotateMRTPair_to_angle(bool accuracy, int port1, int port2, int speed1, int speed2, int angle1, int angle2) {
	double state1=getMRTState(port1); double state2 = getMRTState(port2); // states for beginning
	double res1, res2; // result angles for motors
	bool fms = false; bool sms = false; //first motor stopped and second
	bool fms_back = false; bool sms_back = false; //first and second motor stop backwards
	angle1 = angle1<0 ? -angle1 : angle1; // |angle|
	angle2 = angle2<0 ? -angle2 : angle2; // |angle|

	
	//check the accuracy is moderate
	if (!accuracy) {
	//if speeds of 2 motors	are positive
        if (speed1>0 && speed2>0) {
            res1 = state1+angle1;
            res2 = state2+angle2;

            while (getMRTState(port1)<res1 || getMRTState(port2)<res2) {
                if (getMRTState(port1)<res1) rotateMRT(port1,speed1); else if (!fms) {stopMotorHard(port1); fms = true;}
                if (getMRTState(port2)<res2) rotateMRT(port2,speed2); else if (!sms) {stopMotorHard(port2); sms = true;}

            }

        } else if (speed1<0 && speed2<0) {
            res1 = state1-angle1;
            res2 = state2-angle2;

            while (getMRTState(port1)>res1 || getMRTState(port2)>res2) {
                if (getMRTState(port1)>res1) rotateMRT(port1,speed1); else if (!fms) {stopMotorHard(port1); fms = true;}
                if (getMRTState(port2)>res2) rotateMRT(port2,speed2); else if (!sms) {stopMotorHard(port2); sms = true;}

            }
        } else if (speed1<0 && speed2>0) {
            res1 = state1-angle1;
            res2 = state2+angle2;

            while (getMRTState(port1)>res1 || getMRTState(port2)<res2) {
                if (getMRTState(port1)>res1) rotateMRT(port1,speed1); else if (!fms) {stopMotorHard(port1); fms = true;}
                if (getMRTState(port2)<res2) rotateMRT(port2,speed2); else if (!sms) {stopMotorHard(port2); sms = true;}

            }
        } else if (speed1>0 && speed2<0) {
            res1 = state1+angle1;
            res2 = state2-angle2;

            while (getMRTState(port1)<res1 || getMRTState(port2)>res2) {
                if (getMRTState(port1)<res1) rotateMRT(port1,speed1); else if (!fms) {stopMotorHard(port1); fms = true;}
                if (getMRTState(port2)>res2) rotateMRT(port2,speed2); else if (!sms) {stopMotorHard(port2); sms = true;}

            }
        }
        if (!fms) stopMotorHard(port1);
        if (!sms) stopMotorHard(port2);
			
	//if accuracy is high	
 } else {
        if (speed1>0 && speed2>0) {
            res1 = state1+angle1;
            res2 = state2+angle2;

            while (getMRTState(port1)!=res1 || getMRTState(port2)!=res2) {
                if (getMRTState(port1)<res1) {
                    rotateMRT(port1, speed1);
                } else {
                    stopMotorHard(port1);
                    fms = true;
                    fms_back = true;
                }

                if (getMRTState(port2)<res2) {
                    rotateMRT(port2, speed2);
                } else {
                    stopMotorHard(port2);
                    sms = true;
                    sms_back = true;
                }

                if (getMRTState(port1)>res1)
                    rotateMRT(port1, -speed1);
                else if (fms_back) {
                    stopMotorHard(port1);
                    fms_back = false;
                }

                if (getMRTState(port2)>res2)
                    rotateMRT(port2, -speed2);
                else if (sms_back) {
                    stopMotorHard(port2);
                    sms_back = false;
                }

                if (fms) {
                    speed1>0 ? speed1-- : speed1++;
                    fms = false;
                }

                if (sms) {
                    speed2>0 ? speed2-- : speed2++;
                    sms = false;
                }
                delay(10);

//              check for min value
                if (speed1<10 && speed2<10) break;
            }

        } else if (speed1<0 && speed2<0) {
            res1 = state1-angle1;
            res2 = state2-angle2;

            while (getMRTState(port1)!=res1 || getMRTState(port2)!=res2) {
                if (getMRTState(port1)>res1) {
                    rotateMRT(port1, speed1);
                } else {
                    stopMotorHard(port1);
                    fms = true;
                    fms_back = true;
                }

                if (getMRTState(port2)>res2) {
                    rotateMRT(port2, speed2);
                } else {
                    stopMotorHard(port2);
                    sms = true;
                    sms_back = true;
                }

                if (getMRTState(port1)<res1)
                    rotateMRT(port1, -speed1);
                else if (fms_back) {
                    stopMotorHard(port1);
                    fms_back = false;
                }

                if (getMRTState(port2)<res2)
                    rotateMRT(port2, -speed2);
                else if (sms_back) {
                    stopMotorHard(port2);
                    sms_back = false;
                }

                if (fms) {
                    speed1>0 ? speed1-- : speed1++;
                    fms = false;
                }

                if (sms) {
                    speed2>0 ? speed2-- : speed2++;
                    sms = false;
                }
                delay(10);

//              check for min value
                if (speed1>-10 && speed2>-10) break;
            }
        } else if (speed1>0 && speed2<0) {
            res1 = state1+angle1;
            res2 = state2-angle2;

            while (getMRTState(port1)!=res1 || getMRTState(port2)!=res2) {
                if (getMRTState(port1)<res1) {
                    rotateMRT(port1, speed1);
                } else {
                    stopMotorHard(port1);
                    fms = true;
                    fms_back = true;
                }

                if (getMRTState(port2)>res2) {
                    rotateMRT(port2, speed2);
                } else {
                    stopMotorHard(port2);
                    sms = true;
                    sms_back = true;
                }

                if (getMRTState(port1)>res1)
                    rotateMRT(port1, -speed1);
                else if (fms_back) {
                    stopMotorHard(port1);
                    fms_back = false;
                }

                if (getMRTState(port2)<res2)
                    rotateMRT(port2, -speed2);
                else if (sms_back) {
                    stopMotorHard(port2);
                    sms_back = false;
                }

                if (fms) {
                    speed1>0 ? speed1-- : speed1++;
                    fms = false;
                    fms_back = false;
                }

                if (sms) {
                    speed2>0 ? speed2-- : speed2++;
                    sms = false;
                    sms_back = false;
                }
                delay(10);

//              check for min value
                if (speed1<10 && speed2>-10) break;
            }
        } else if (speed1<0 && speed2>0) {
            res1 = state1-angle1;
            res2 = state2+angle2;

            while (getMRTState(port1)!=res1 || getMRTState(port2)!=res2) {
                if (getMRTState(port1)>res1) {
                    rotateMRT(port1, speed1);
                } else {
                    stopMotorHard(port1);
                    fms = true;
                    fms_back = true;
                }

                if (getMRTState(port2)<res2) {
                    rotateMRT(port2, speed2);
                } else {
                    stopMotorHard(port2);
                    sms = true;
                    sms_back = true;
                }

                if (getMRTState(port1)<res1)
                    rotateMRT(port1, -speed1);
                else if (fms_back) {
                    stopMotorHard(port1);
                    fms_back = false;
                }

                if (getMRTState(port2)>res2)
                    rotateMRT(port2, -speed2);
                else if (sms_back) {
                    stopMotorHard(port2);
                    sms_back = false;
                }

                if (fms) {
                    speed1>0 ? speed1-- : speed1++;
                    fms = false;
                    fms_back = false;
                }

                if (sms) {
                    speed2>0 ? speed2-- : speed2++;
                    sms = false;
                    sms_back = false;
                }
                delay(10);

//              check for min value
                if (speed1>-10 && speed2<10) break;
            }
        }
        stopMotorHard(port1);
        stopMotorHard(port2);
	}
 }
 
 void rotateMRTPair_to_state(bool accuracy, int port1, int port2, int speed1, int speed2, int angle1, int angle2) {
     if (angle1>getMRTState(port1) && angle2>getMRTState(port2)) {
         if (speed1<0) speed1 = -speed1;
         if (speed2<0) speed2 = -speed2;
         rotateMRTPair_to_angle(accuracy, port1, port2, speed1, speed2,
                                angle1-getMRTState(port1), angle2-getMRTState(port2));

     } else if (angle1<getMRTState(port1) && angle2<getMRTState(port2)) {
         if (speed1>0) speed1 = -speed1;
         if (speed2>0) speed2 = -speed2;
         rotateMRTPair_to_angle(accuracy, port1, port2, speed1, speed2,
                                getMRTState(port1)-angle1, getMRTState(port2)-angle2);

     } else if (angle1<getMRTState(port1) && angle2>getMRTState(port2)) {
         if (speed1>0) speed1 = -speed1;
         if (speed2<0) speed2 = -speed2;
         rotateMRTPair_to_angle(accuracy, port1, port2, speed1, speed2,
                                getMRTState(port1)-angle1, angle2-getMRTState(port2));

     } else if (angle1>getMRTState(port1) && angle2<getMRTState(port2)) {
         if (speed1<0) speed1 = -speed1;
         if (speed2>0) speed2 = -speed2;
         rotateMRTPair_to_angle(accuracy, port1, port2, speed1, speed2,
                                angle1-getMRTState(port1), getMRTState(port2)-angle2);

     }
 }

 void rotateMRT_to_state(bool accuracy, int motorPort, int speed,  int angle) {
	 if (angle<getMRTState(motorPort)) {
	 if (speed<0)rotateMRT_to_angle(accuracy, motorPort,speed,getMRTState(motorPort)-angle);
	 else if (speed>0)rotateMRT_to_angle(accuracy, motorPort,-speed,getMRTState(motorPort)-angle);
	 }
	 else if (angle>getMRTState(motorPort)) {
		if (speed<0)rotateMRT_to_angle(accuracy, motorPort,-speed,angle-getMRTState(motorPort));
	 else if (speed>0)rotateMRT_to_angle(accuracy, motorPort,speed,angle-getMRTState(motorPort));
	 }
 }

 void stopMotor(int motorPort) {
     newMotorSpeed[(motorPort - 1)] = 0;
     currentMotorSpeed[(motorPort - 1)] = 0;

     switch (motorPort) {
         case 1:
             analogWrite(MOTOR1_EN, 0);
             break;

         case 2:
             analogWrite(MOTOR2_EN, 0);
             break;

         case 3:
             analogWrite(MOTOR3_EN, 0);
             break;

         case 4:
             analogWrite(MOTOR4_EN, 0);
             break;
     }
 }

 void stopMotorHard(int motorPort) {
     newMotorSpeed[(motorPort - 1)] = 0;
     currentMotorSpeed[(motorPort - 1)] = 0;
     switch (motorPort) {
         case 1:
             digitalWrite(MOTOR1_IN1, LOW);
             digitalWrite(MOTOR1_IN2, LOW);
             analogWrite(MOTOR1_EN, 255);
             break;
         case 2:
             digitalWrite(MOTOR2_IN1, LOW);
             digitalWrite(MOTOR2_IN2, LOW);
             analogWrite(MOTOR2_EN, 255);
             break;
         case 3:
             digitalWrite(MOTOR3_IN1, LOW);
             digitalWrite(MOTOR3_IN2, LOW);
             analogWrite(MOTOR3_EN, 255);
             break;
         case 4:
             digitalWrite(MOTOR4_IN1, LOW);
             digitalWrite(MOTOR4_IN2, LOW);
             analogWrite(MOTOR4_EN, 255);
             break;
     }
 }

 // -----------2nd-level control functions------------

 // -----------RGB LED functions------------
// 0-off, 1-white, 2-red, 3-green, 4-blue
// 0-off, 1-white, 2-red, 3-green, 4-blue, 6-yellow 8-purple 9-aqua
 void builtInRGB(int color) {
     switch (color) {
         case 0:
             digitalWrite(RGB_RED, LOW);
             digitalWrite(RGB_GREEN, LOW);
             digitalWrite(RGB_BLUE, LOW);
             break;

         case 1:
             digitalWrite(RGB_RED, HIGH);
             digitalWrite(RGB_GREEN, HIGH);
             digitalWrite(RGB_BLUE, HIGH);
             break;

         case 2:
             digitalWrite(RGB_RED, HIGH);
             digitalWrite(RGB_GREEN, LOW);
             digitalWrite(RGB_BLUE, LOW);
             break;

         case 3:
             digitalWrite(RGB_RED, LOW);
             digitalWrite(RGB_GREEN, HIGH);
             digitalWrite(RGB_BLUE, LOW);
             break;

         case 4:
             digitalWrite(RGB_RED, LOW);
             digitalWrite(RGB_GREEN, LOW);
             digitalWrite(RGB_BLUE, HIGH);
             break;

		case 6:
            digitalWrite(RGB_RED, HIGH);
            digitalWrite(RGB_GREEN, HIGH);

            digitalWrite(RGB_BLUE, LOW);
            break;

        case 8:
             digitalWrite(RGB_RED, HIGH);
             digitalWrite(RGB_GREEN, LOW);
             digitalWrite(RGB_BLUE, HIGH);
             break;
		case 9:
             digitalWrite(RGB_RED, LOW);
             digitalWrite(RGB_GREEN, HIGH);
             digitalWrite(RGB_BLUE, HIGH);
             break;

     }
 }

 //-------------Sensors read functions----------

 #define ULTRASONIC_DISTANCE_SENSOR_MIN_DISTANCE_IN_CM 2
 #define ULTRASONIC_DISTANCE_SENSOR_MAX_DISTANCE_IN_CM 300

 bool buttonRead(int port) { return !digitalRead(port); }
 int distanceCM(int port) {
     unsigned long duration;
     unsigned int distance;

     pinMode(port, OUTPUT);
     digitalWrite(port, LOW);
     delayMicroseconds(2);
     digitalWrite(port, HIGH);
     delayMicroseconds(10);
     digitalWrite(port, LOW);
     pinMode(port, INPUT);
     duration = pulseIn(port, HIGH, 20000);

     if (duration == 0) {
         return ULTRASONIC_DISTANCE_SENSOR_MAX_DISTANCE_IN_CM;
     }

     distance = duration / 58;

     return constrain(distance, ULTRASONIC_DISTANCE_SENSOR_MIN_DISTANCE_IN_CM, ULTRASONIC_DISTANCE_SENSOR_MAX_DISTANCE_IN_CM);
 }

 bool irWhite(int port, int threshold) {
     int reading = analogRead(port);
     if (reading >= threshold) {
         return true;
     } else {
         return false;
     }
 }

 bool irBlack(int port, int threshold) {
     int reading = analogRead(port);
     if (reading < threshold) {
         return true;
     } else {
         return false;
     }
 }

 int irAnalog(int port) {
     int reading = analogRead(port);

     return reading;
 }

 bool flame(int port) {
     int value = analogRead(port);
     if (value >= 900) {
         return true;
     } else {
         return false;
     }
 }

 bool shock(int port) {
     pinMode(port, INPUT_PULLUP);
     int reading = analogRead(port);
     if (reading < 200) {
         return true;
     } else {
         return false;
     }
     pinMode(port, INPUT);
 }

 int temperature(int port) {
     OneWire ds(port);
     byte data[2];
     ds.reset();
     ds.write(0xCC);
     ds.write(0x44);
     delay(750);
     ds.reset();
     ds.write(0xCC);
     ds.write(0xBE);
     data[0] = ds.read();
     data[1] = ds.read();
     int Temp = (data[1] << 8) + data[0];
     Temp = Temp >> 4;
     return Temp;
 }

 bool vibration(int port) {
     int reading = analogRead(port);
     if (reading > 1000) {
         return true;
     } else {
         return false;
     }
 }

 bool mic(int port) {
     int threshold = 100;
     pinMode(port, INPUT_PULLUP);
     int reading = analogRead(port);
     if (reading >= threshold) {
         return false;
     } else {
         return true;
     }
 }

 int micAnalog(int port) {
     analogReference(INTERNAL1V1);

     int oldvalue = 0;
     int newvalue = 0;

     for (int i = 0; i < 150; i++) {
         newvalue = analogRead(port);
         if (newvalue > oldvalue) {
             oldvalue = newvalue;
         }
     }
     analogReference(DEFAULT);
     return oldvalue;
 }

 bool cdsLight(int port, int threshold) {
     int reading = analogRead(port);

     if (reading >= threshold) {
         return true;
     } else {
         return false;
     }
 }

 bool cdsDark(int port, int threshold) {
     int reading = analogRead(port);
     if (reading < threshold) {
         return true;
     } else {
         return false;
     }
 }

 int cdsAnalog(int port) {
     int reading = analogRead(port);

     return reading;
 }

 bool metal(int port) {
     pinMode(port, INPUT_PULLUP);
     int reading = analogRead(port);
     if (reading > 1000)
         return false;
     else
         return true;
     pinMode(port, INPUT);
 }

 bool tilt(int port) { return digitalRead(port); }
 bool magnetic(int port) { return digitalRead(port); }

 int bend(int port) { return analogRead(port); }

 int skin(int port) { return analogRead(port); }

 //------------Actuators funcrions------------

 void led(int ledPort, bool state) {
     pinMode(ledPort, OUTPUT);
     digitalWrite(ledPort, state);
 }

 void ledPWM(int ledPort, int PWM) {
     pinMode(ledPort, OUTPUT);
     analogWrite(ledPort, PWM);
 }

void relay(int relayPort, bool state) {
	pinMode(relayPort, OUTPUT);
    digitalWrite(relayPort, state);
}

double currentTemp() {

	double data;
	Wire.begin();

	Wire.beginTransmission(ADR_METEO); // адрес устройства ADR_METEO
  Wire.write(REG_TEMPVALUE_L);        // Передаем адрес регистра REG_TEMPVALUE_L
  Wire.endTransmission();

  Wire.requestFrom(ADR_METEO, 2);    // Запрашиваем 2 байта

  if (Wire.available()) {data = Wire.read(); data = data + (Wire.read() << 8); }

  return data/10;

}

double currentHumidity() {
	 double data;
	 Wire.begin();
	 Wire.beginTransmission(ADR_METEO); // адрес устройства ADR_METEO
  Wire.write(REG_HUMVALUE_L);        // Передаем адрес регистра REG_HUMVALUE_L
  Wire.endTransmission();

  Wire.requestFrom(ADR_METEO, 2);    // Запрашиваем 2 байта

  if (Wire.available()) {data = Wire.read(); data = data + (Wire.read() << 8);}

  return data/10;

}

double current_kPa_preasure() {
	double data;
	Wire.begin();
	Wire.beginTransmission(ADR_METEO); // адрес устройства ADR_METEO
  Wire.write(REG_PRESSKPAVALUE_L);        // Передаем адрес регистра REG_PRESSKPAVALUE_L
  Wire.endTransmission();

  Wire.requestFrom(ADR_METEO, 2);    // Запрашиваем 2 байта

  if (Wire.available()) {data = Wire.read(); data = data + (Wire.read() << 8);}

  return data/10;
}

double current_mm_preasure() {
	double data;
	Wire.begin();
	Wire.beginTransmission(ADR_METEO); // адрес устройства ADR_METEO
  Wire.write(REG_PRESSHGVALUE_L);        // Передаем адрес регистра REG_PRESSHGVALUE_L
  Wire.endTransmission();

  Wire.requestFrom(ADR_METEO, 2);    // Запрашиваем 2 байта

  if (Wire.available()) {data = Wire.read(); data = data + (Wire.read() << 8);}

  return data/10;
}

double soil_moisture() {
	double data;
	Wire.begin();
	Wire.beginTransmission(ADR_MOISTURE); // адрес устройства ADR_MOISTURE
  Wire.write(REG_MOISTUREVALUE);        // Передаем адрес регистра REG_MOISTUREVALUE
  Wire.endTransmission();

  Wire.requestFrom(ADR_MOISTURE, 1);

	return Wire.read();
}


 // -----------Color sensor------------
 int colorSensorCDSpin;  // default pin, changes in setup
 int ledArrayOfcolorArray[3];
 int colorArray[] = {0, 0, 0};
 int whiteArray[] = {0, 0, 0};
 int blackArray[] = {0, 0, 0};
 float greyDiff[] = {0, 0, 0};

 void setupColorSensor(int Rpin, int Bpin, int Gpin, int CDSpin) {
     // setup the outputs for the Color sensor на всякий случай
     pinMode(Rpin, OUTPUT);
     pinMode(Gpin, OUTPUT);
     pinMode(Bpin, OUTPUT);
     pinMode(colorSensorCDSpin, INPUT);

     ledArrayOfcolorArray[0] = Rpin;
     ledArrayOfcolorArray[1] = Gpin;
     ledArrayOfcolorArray[2] = Bpin;

     colorSensorCDSpin = CDSpin;

     //мигаем 3 раза белым о необходимости произвести калибровку
     for (int i = 0; i < 3; i++) {
         digitalWrite(Rpin, HIGH);
         digitalWrite(Gpin, HIGH);
         digitalWrite(Bpin, HIGH);
         delay(150);
         digitalWrite(Rpin, LOW);
         digitalWrite(Gpin, LOW);
         digitalWrite(Bpin, LOW);
         delay(70);
     }

     analogReference(INTERNAL2V56);
     // set white balance
     delay(3000);  // delay, this gives us time to get a white sample in front of our sensor
     // scan the white sample.
     // go through each light, get a reading, set the base reading for each Color red, green, and blue to the white array
     for (int i = 0; i <= 2; i++) {
         digitalWrite(ledArrayOfcolorArray[i], HIGH);
         delay(50);
         // number is the number of scans to take for average, this whole function is redundant, one reading works just as well.
         whiteArray[i] = getCDSReading(colorSensorCDSpin, 5);
         digitalWrite(ledArrayOfcolorArray[i], LOW);
         delay(100);
     }
     // set black balance
     delay(3000);  // wait for five seconds so we can position our black sample
     // go ahead and scan, sets the Color values for red, green, and blue when exposed to black
     for (int i = 0; i <= 2; i++) {
         digitalWrite(ledArrayOfcolorArray[i], HIGH);
         delay(100);
         blackArray[i] = getCDSReading(colorSensorCDSpin, 5);
         digitalWrite(ledArrayOfcolorArray[i], LOW);
         delay(100);
     }
     // расчитываем диапазон возможных значений между черным и белим
     for (int i = 0; i <= 2; i++) {
         greyDiff[i] = whiteArray[i] - blackArray[i];
     }

     // DEBUG
     //  for (int i = 0; i <= 2; i++) {
     //   Serial.print(whiteArray[i]);
     //   Serial.print("--");
     //   Serial.print(blackArray[i]);
     //   Serial.print("--");
     //   Serial.println(greyDiff[i]);
     // }

     // delay another 3 seconds to allow the human to catch up to what is going on
     delay(3000);
     analogReference(DEFAULT);
 }

 int getColor(void) {
     for (int i = 0; i <= 2; i++) {
         digitalWrite(ledArrayOfcolorArray[i], LOW);
     }

     analogReference(INTERNAL2V56);
     for (int i = 0; i <= 2; i++) {
         digitalWrite(ledArrayOfcolorArray[i], HIGH);  // turn on the LED, red, green or blue depending which iteration
         delay(70);                                    // delay to allow CdS to stabalize, they are slow

         colorArray[i] = getCDSReading(colorSensorCDSpin, 5);  // set the current Color in the array to the average reading

         colorArray[i] = (colorArray[i] - blackArray[i]) / (greyDiff[i]) * 100;  //выражаем прочитанный цвет в % между черным и белым
         digitalWrite(ledArrayOfcolorArray[i], LOW);                             // turn off the current LED
         delay(70);
         //  округление в рамки 0 - 100%
         for (int i = 0; i <= 2; i++) {
             if (colorArray[i] < 0) {
                 colorArray[i] = 0;
             }
             if (colorArray[i] > 100) {
                 colorArray[i] = 100;
             }
         }
     }

     //  -------DEBUG---------
     // Serial.print("R = ");
     // Serial.println(int(colorArray[0]));
     // Serial.print("G = ");
     // Serial.println(int(colorArray[1]));
     // Serial.print("B = ");
     // Serial.println(int(colorArray[2]));
     // ------/DEBUG----------

     int deltaW = 20;
     int deltaB = 7;
     int deltaColor = 4;

     // 0-black 1-white 2-red 3-green 4-blue
     //белый: все три компоненты > 50, разница между ними МЕНЬШЕ deltaW
     if ((colorArray[0] > 50) && (colorArray[1] > 50) && (colorArray[2] > 50) && (abs(colorArray[0] - colorArray[1]) < deltaW) &&
         (abs(colorArray[0] - colorArray[2]) < deltaW) && (abs(colorArray[1] - colorArray[2]) < deltaW)) {
         return 1;
     }
     //черный: все три компоненты < 10, разница между ними МЕНЬШЕ deltaB
     if ((colorArray[0] < 20) && (colorArray[1] < 20) && (colorArray[2] < 20) && (abs(colorArray[0] - colorArray[1]) < deltaB) &&
         (abs(colorArray[0] - colorArray[2]) < deltaB) && (abs(colorArray[1] - colorArray[2]) < deltaB)) {
         return 0;
     }
     //КРАСНЫЙ: если красная компонента выбивается на deltaColor над любой из остальных
     if (((colorArray[0] - colorArray[1]) > deltaColor) && ((colorArray[0] - colorArray[2]) > deltaColor)) {
         return 2;
     }
     //ЗЕЛЕНЫЙ: если зеленая компонента выбивается на deltaColor над любой из остальных
     if (((colorArray[1] - colorArray[0]) > deltaColor) && ((colorArray[1] - colorArray[2]) > deltaColor)) {
         return 3;
     }
     //СИНИЙ: если синяя компонента выбивается хотя бы на на deltaColor над любой из остальных
     if (((colorArray[2] - colorArray[1]) > deltaColor) && ((colorArray[2] - colorArray[1]) > deltaColor)) {
         return 4;
     }

     return 9;  //неопределенный результат

     analogReference(DEFAULT);
 }

/*
Цифровой датчик цвета

0-Непонятный цвет
1-Черный
2-Серый
3-Белый
4-Красный
5-Желтый
6-Зеленый
7-Голубой
8-Синий
9-Фиолетовый
*/
 int defineColor(void) {
	unsigned short  data, red, green, blue, clear, hue, sat, val;
	char color;
	Wire.begin();
	Wire.beginTransmission(ADR_COLOR); // адрес устройства ADR_COLOR
      Wire.write(REG_LIGHT);        // Передаем адрес регистра REG_LIGHT
      Wire.write(1);        // Передаем "1" адрес регистра REG_LIGHT (0-выключить подсветку, 1 - включить подсветку)
      Wire.endTransmission();

      //delay (10); //Ждем немного

      Wire.beginTransmission(ADR_COLOR); // адрес устройства ADR_COLOR
        Wire.write(REG_FIRST);        // Передаем адрес регистра REG_FIRST
        Wire.endTransmission();

        Wire.requestFrom(ADR_COLOR, 15);    // Запрашиваем 15 байт

		 if (Wire.available()) {
			red = Wire.read();
			red = red + (Wire.read() << 8);
			green = Wire.read();
			green = green + (Wire.read() << 8);
			blue = Wire.read();
			blue = blue + (Wire.read() << 8);
			clear = Wire.read();
			clear = clear + (Wire.read() << 8);
			hue = Wire.read();
			hue = hue + (Wire.read() << 8);
			sat = Wire.read();
			sat = sat + (Wire.read() << 8);
			val = Wire.read();
			val = val + (Wire.read() << 8);
			color = Wire.read();
		 }

		 return (int)color;
 }

 int getCDSReading(int colorSensorCDSpin, int times) {
     int reading = 0;
     //  int tally = 0;
     // take the reading however many times was requested and add them up
     for (int i = 0; i < times; i++) {
         reading = reading + analogRead(colorSensorCDSpin);
         //    tally = reading + tally;
         delay(10);
     }
     // calculate the average and set it
     reading = reading / times;
     return reading;
 }

 //====================IR RC Functions=================

 //
 // local var
 //
 byte impuls = 1;

 unsigned long time, dtime;

 unsigned long timeout_mark;

 unsigned long res = 0;
 unsigned int buf = 0;

 byte Channl_ID = 1;
 unsigned long Button_ID = 0;

 byte Button_en = 0;

 byte state = 0;
 byte Channl_buf = 0;

 //
 // функция для конструкци if ( RC(кнопка)){}
 //

 bool RC(long BT_ID) {
     if (Timeout < millis() - timeout_mark) {
         Button_ID = OFF_KEY;
     }

     if (BT_ID == Button_ID) {
         return 1;
     }

     return 0;
 }

 void IRRC_setup(int pin, uint8_t Channl) {
     pinMode(pin, INPUT);      // set the pin to input
     digitalWrite(pin, HIGH);  // use the internal pullup resistor

     time = micros();
     // PCintPort::attachInterrupt(pin, change, CHANGE); // attach a PinChange Interrupt to our first pin
     enableInterrupt(pin, change, CHANGE);

     switch (Channl) {
         case 1:
             Channl_ID = Channl_1;
             return;
         case 2:
             Channl_ID = Channl_2;
             return;
         case 3:
             Channl_ID = Channl_3;
             return;
         case 4:
             Channl_ID = Channl_4;
             return;
         case 5:
             Channl_ID = Channl_5;
             return;
         case 6:
             Channl_ID = Channl_6;
             return;
         case 7:
             Channl_ID = Channl_7;
             return;
         case 8:
             Channl_ID = Channl_8;
             return;
             // default:
     }
 }

 //
 // Inerupt function
 //

 void change() {
     dtime = micros();
     buf = dtime - time;
     time = dtime;

     // обработка длины импульса (по 200 с округлением в большую сторону при buf % 200 > 101
     //и(или) если импульс < 101 присвоение единичной длинны)
     if (buf % 200 > 101) {
         buf = (buf / 200) + 1;
     } else {
         buf = buf / 200;
     }
     if (buf == 0) buf = 1;

     if (state == 0)  // начальное состояние
     {
         if (!impuls)  //проверка на первые знаяащие биты (000)
         {
             state++;
             res = 0;
             res = res << buf;
             // Serial.println("Start");
         }
     } else {
         if (impuls) {
             res = res << buf;
             for (int i = 0; i < buf; i++) {
                 res |= 1 << i;
             }
         } else {
             res = res << buf;

             if ((byte)(res & 0x7F) == 0x38) {
                 buf = 1;
                 for (int i = 1; i < Shift; i++) buf = (buf << 1) + 1;

                 Channl_buf = (res >> 6) & buf;

                 if (Channl_buf == Channl_ID) {
                     Button_ID = res >> (6 + Shift);
                     while (!(Button_ID & 1)) Button_ID = Button_ID >> 1;
                     timeout_mark = millis();
                     // Serial.println (Button_ID, HEX);
                 }
                 state = 0;
                 res = 0;
             }
         }
     }
     impuls = !impuls;
 }

 // //======================BLUETOOTH RC==========================

 int count = 0;
 static const int commandSize = 12;
 byte command[commandSize];

 boolean f[6];
 int value[10];

 int bluetooth_X, bluetooth_Y, bluetooth_Z, bluetooth_W, bluetooth_aX, bluetooth_aY, bluetooth_aZ, bluetooth_pot_1, bluetooth_pot_2, bluetooth_pot_3;

 bool bluetooth_F1, bluetooth_F2, bluetooth_F3, bluetooth_F4, bluetooth_F5, bluetooth_F6;

 // from main.c
 extern bool BTenable;
 void BTinit(void) {
     Serial.begin(115200);
     clearValues();
     BTenable = 1;
 }

 void clearValues() {
     // Обнулим массив клавиш F
     for (int i = 0; i < 6; i++) {
         f[i] = false;
     }
     for (int i = 0; i < 10; i++) {
         value[i] = 0;
     }
     for (int i = 0; i < commandSize; i++) {
         command[i] = 0x00;
     }
 }

 void calculateValues() {
     int fData = (int)command[7];

     // Найдем все включенные клавиши
     // Зафиксируем, включен ли F6
     if (fData - 64 >= 0) {
         f[5] = true;
         fData -= 64;
     }
     // Зафиксируем, включен ли F5
     if (fData - 32 >= 0) {
         f[4] = true;
         fData -= 32;
     }
     // Зафиксируем, включен ли F4
     if (fData - 16 >= 0) {
         f[3] = true;
         fData -= 16;
     }
     // Зафиксируем, включен ли F3
     if (fData - 8 >= 0) {
         f[2] = true;
         fData -= 8;
     }
     // Зафиксируем, включен ли F2
     if (fData - 4 >= 0) {
         f[1] = true;
         fData -= 4;
     }
     // Зафиксируем, включен ли F1
     if (fData - 2 >= 0) {
         f[0] = true;
         fData -= 2;
     }

     int buffer;
     for (int i = 0; i < 7; i++) {
         buffer = (int)command[i];
         // Переведём знаковый байт в целое число с учетом знака
         if (buffer > 128) {
             buffer -= 256;
         }
         value[i] = buffer;
     }
     for (int i = 7; i < 10; i++) {
         value[i] = (int)command[i + 2];
     }
 }

 void BTEventRun(void) {
     if (Serial.available() >= commandSize) {
         for (int i = 0; i < commandSize; i++) {
             // Save command
             command[i] = (byte)Serial.read();
         }

         while (Serial.available()) {
             Serial.read();
         }

         // Обнулим массив клавиш F
         for (int i = 0; i < 6; i++) {
             f[i] = false;
         }

         calculateValues();

         bluetooth_X = value[0];
         bluetooth_Y = value[1];
         bluetooth_Z = value[2];
         bluetooth_W = value[3];
         bluetooth_aX = value[4];
         bluetooth_aY = value[5];
         bluetooth_aZ = value[6];

         bluetooth_F1 = f[0];
         bluetooth_F2 = f[1];
         bluetooth_F3 = f[2];
         bluetooth_F4 = f[3];
         bluetooth_F5 = f[4];
         bluetooth_F6 = f[5];

         bluetooth_pot_1 = value[7];
         bluetooth_pot_2 = value[8];
         bluetooth_pot_3 = value[9];

         /*
          * Согласно ТЗ расшифровка следующая:
          * 0 - x
          * 1 - y
          * 2 - z
          * 3 - w
          * 4 - ax
          * 5 - ay
          * 6 - az
          * 7 - f1..f6
          * 8 - голосовые команды. по умолчанию 0x00, 1-6 - номер команды
          * 9 - pot1
          * 10- pot2
          * 11- pot3
          */
     }
 }
 

	 
	// --------------------- Artintrack -----------------------------
	bool artintrackInited;
	int port;
	int _ABVAR_2_current_face_id =1;
	char _ABVAR_9_face_id_str[5];
	bool isFirstSave = true;
	
	bool comm_send(char data) { 
		if (port==3) return Serial3.write(data) == 1;
		else if (port==2) return Serial2.write(data) == 1;
		else if (port==1) return Serial1.write(data) == 1;
	}
	
	int32_t comm_recv() { 
		if (port==3) return (int32_t) Serial3.read();
		else if (port==2) return (int32_t) Serial2.read();
		else if (port==1) return (int32_t) Serial1.read();
	}
	
	
	void delay_ms(uint32_t value) { delay(value); }
	uint32_t TS_ms(void) { return (uint32_t)millis(); }
	
	void artintrackInit(int uart) {
		port = uart;
		artintrackInited = false;
		
		Serial.begin(115200);
		
		Trackcv_init(comm_recv, comm_send, 0);
		
	}
	
	void artintrackUpdate(int nn) {
		
		if (!artintrackInited) {
    if (trackcv_get_errno() != ERR_OK) {
      Serial.println("TrackCV... Fail");
      builtInRGB(RED);
      if (trackcv_check()) {
        Serial.println("TrackCV check... OK");
		Serial.println("TrackCV... OK");
      trackcv_neural_start(nn);
      artintrackInited = true;
      builtInRGB(OFF);
	  _ABVAR_2_current_face_id = trackcv_neural_class_count(0) + 1;
      } 
      else {
        Serial.println("TrackCV check... Fail");
      }
      delay(250);
      return;
    } 
    else {
      Serial.println("TrackCV... OK");
      trackcv_neural_start(nn);
      artintrackInited = true;
      builtInRGB(OFF);
	  _ABVAR_2_current_face_id = trackcv_neural_class_count(0) + 1;
    }
  } 
}

    void artintrackCheckForErrors() {
		 Errno errno = ERR_OK;
		 if ((errno = trackcv_get_errno()) != ERR_OK) {
			Serial.print("TrackCV Error ");
			Serial.println(errno);
			builtInRGB(OFF);
			artintrackInited = false;
  }
}
bool artintrackSaveFace()
{
  if (( ( trackcv_neural_count() ) != ( 1 ) ))
  {
    return false;
  }
  if (( ( _ABVAR_2_current_face_id ) > ( 5 ) ))
  {
    return false;
  }
  if (isFirstSave) {
	  _ABVAR_2_current_face_id = trackcv_neural_class_count(0) + 1;
	  isFirstSave = false;
  }
  itoa(_ABVAR_2_current_face_id, _ABVAR_9_face_id_str, 10);
  if (trackcv_neural_save_face(_ABVAR_9_face_id_str))
  {
    ++_ABVAR_2_current_face_id ;
	return true;
  }
  else
  {
	return false;
  }
}

int artintrackCheckFace()
{
  if (trackcv_neural_count() != 1)   {     
    return 0;   
  }    
  bool is_face = false;    
  int person_id = -1;   
  int person_max_p = 0;
  int res = 0;
  for (int class_id = 0; class_id < trackcv_neural_class_count(0); class_id++)   {            
      if ((strstr(trackcv_neural_class_meta(0, class_id), "1.png") != 0 ||            strstr(trackcv_neural_class_meta(0, class_id), "2.png") != 0  ||            strstr(trackcv_neural_class_meta(0, class_id), "3.png") != 0 ||            strstr(trackcv_neural_class_meta(0, class_id), "4.png") != 0 ||            strstr(trackcv_neural_class_meta(0, class_id), "5.png") != 0) &&           trackcv_neural_class_p(0, class_id) > 0)       {         
        is_face = true;         
        if (trackcv_neural_class_p(0, class_id) >= person_max_p)         {           
          person_max_p = trackcv_neural_class_p(0, class_id);           
          person_id = class_id;         
        }           
    }     
    switch (person_id)     {       
    case -1:       
      {           
        res = 0;
		break;
      }       
    case 0:       
      {         
        res = 1;
		break;
      }       
    case 1:       
      {         
        res = 2;
		break;
      }
      case 2:       
      {         
        res = 3;
		break;
      }
       case 3:       
      {         
        res = 4;
		break;		
      }
       case 4:       
      {         
        res = 5;
		break;		
      }      
    }
  }    
  return res;
}

bool artintrackClearFaces()
{
  if (trackcv_neural_clear_faces())
  {
	  _ABVAR_2_current_face_id = 1 ;
	  return true;
  }
  else
  {
	  return false;
    }
	
}


//Serial read

bool serialParseBool() {
	char inChar = Serial.read();

    if (int(inChar)>0 || inChar == 'T' || inChar == 't') {
      return true;
    } else {
      return false;
    }
  }
  
char serialReadChar() {
	return Serial.readString()[0];
}

// U8Glib

void oledPoint(int x, int y) {
    ubg.drawPixel(x, y);
}

void oledLine(int x, int y, int x2, int y2) {
    ubg.drawLine(x, y, x2, y2);
}

void oledBox(int x, int y, int w, int h) {
    ubg.drawBox(x, y, w, h);
}

void oledFrame(int x, int y, int w, int h) {
    ubg.drawFrame(x, y, w, h);
}

void oledCircle(int x, int y, int r) {
    ubg.drawCircle(x, y, r);
}

void oledTriangle(int x, int y, int x2, int y2, int x3, int y3) {
    ubg.drawTriangle(x, y, x2, y2, x3, y3);
}

void oledClear() {
    ubg.firstPage();
}

uint8_t nextPageAvailable() {
    return ubg.nextPage();
}

int liveGraphBuffer[GRAPH_LEN];

void oledGraph(int newValue, int minValue, int maxValue, int graphHeight) {
  int baseY = 63;  // нижняя граница графика (для 128x64 экрана)

  // сдвиг значений в буфере
  for (int i = 0; i < GRAPH_LEN - 1; i++) {
    liveGraphBuffer[i] = liveGraphBuffer[i + 1];
  }

  // масштабируем и добавляем новое значение
  liveGraphBuffer[GRAPH_LEN - 1] = map(newValue, minValue, maxValue, 0, graphHeight);

  // отрисовка линий графика снизу вверх
  for (int x = 0; x < GRAPH_LEN - 1; x++) {
    int y1 = baseY - liveGraphBuffer[x];
    int y2 = baseY - liveGraphBuffer[x + 1];
    ubg.drawLine(x, y1, x + 1, y2);
  }
}

void oledText(int x, int y, char* text, int font) {
 //#include "1rus4x6.h"
 //#include "2rus5x7.h"
 //#include "3rus7x13.h"
 //#include "4rusUnifont.h"
 //#include "5rus9x15.h"
 //#include "6rus10x20.h"

 int current_font = 0;   
 if (font<1)  current_font = 1;
 else if (font>6) current_font = 6;
 else current_font = font;

 switch (current_font) {
         case 1: {
             ubg.setFont(rus4x6);
             break;
         }
         case 2: {
             ubg.setFont(rus5x7);
             break;
         }
         case 3: {
             ubg.setFont(rus7x13);
             break;
         }
         case 4: {
             ubg.setFont(rusUnifont);
             break;
         }
          case 5: {
             ubg.setFont(rus9x15);
             break;
         }
          case 6: {
             ubg.setFont(rus10x20);
             break;
         }
     }

     ubg.drawStr(x,y,text);


}