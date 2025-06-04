 #include <Arduino.h>
 //#include <TimerOne.h>
 

 
 
 #define OFF 0
 #define ON 1
 
 #define BLACK 0
 #define WHITE 1
 #define RED 2
 #define GREEN 3
 #define BLUE 4
 #define BROWN 5
 #define YELLOW 6
 #define GREY 7
 #define PURPLE 8
 #define AQUA 9
 
 
 //gyro-accelerometer reading modes
 #define RAW_GYRO 0 
 #define RAW_ACCEL 1 
 #define FILTERED 2 
 
 #define SMALL_SERVO 544,2050
 #define BIG_SERVO 1000,2000

 //Адрес устройства на шине I2C
 #define ADR_COLOR 0x26

 //Регистры датчика
 #define REG_ID 0x10  //Регистр идентификатора датчика, должен быть 0x55
 #define REG_LIGHT 0x11 //Регистр включения (записать 1) выключения (записать 0) подсветки (по умолчанию включена)
 #define REG_FIRST 0x20 //Первый регистр блока регистров (см. далее)
 #define REG_REDVALUE_L 0x20 //Регистр сенсора RED младший байт
 #define REG_REDVALUE_H 0x21 //Регистр сенсора RED старший байт
 #define REG_GREENVALUE_L 0x22 //Регистр сенсора GREEN младший байт
 #define REG_GREENVALUE_H 0x23 //Регистр сенсора GREEN старший байт
 #define REG_BLUEVALUE_L 0x24 //Регистр сенсора BLUE младший байт
 #define REG_BLUEVALUE_H 0x25 //Регистр сенсора BLUE старший байт
 #define REG_CLRVALUE_L 0x26 //Регистр сенсора CLEAR младший байт
 #define REG_CLRVALUE_H 0x27 //Регистр сенсора CLEAR старший байт
 #define REG_HUEVALUE_L 0x28 //Регистр значения HUE (модель HSV) младший байт
 #define REG_HUEVALUE_H 0x29 //Регистр значения HUE (модель HSV) старший байт
 #define REG_SATVALUE_L 0x2A //Регистр значения SATURATION (модель HSV) младший байт
 #define REG_SATVALUE_H 0x2B //Регистр значения SATURATION (модель HSV) старший байт
 #define REG_VALVALUE_L 0x2C //Регистр значения VALUE (модель HSV) младший байт
 #define REG_VALVALUE_H 0x2D //Регистр значения VALUE (модель HSV) старший байт
 #define REG_COLOR 0x2E //Регистр номера цвета

 //Адрес устройства на шине I2C
#define ADR_TIMESENSOR      0x27

//Регистры датчика
#define REG_ID              0x10  //Регистр идентификатора датчика, должен быть 0x56
#define REG_SETDATE         0x11 //Команда установки даты (год (0-99), месяц (1-12), день (1-31))
#define REG_SETTIME         0x12 //Команда установки времени (час (0-23), мин (0-59), сек (0-59))
#define REG_INDICATOR       0x13 //Команда включения (записать 1) выключения (записать 0) индикатора (по умолчанию включена)
#define REG_FIRST           0x20 //Первый регистр блока регистров (см. далее)
#define REG_DAYVALUE        0x21 //Регистр дня (1-31) байт
#define REG_MONTHVALUE      0x22 //Регистр месяца (1-12) байт
#define REG_YEARVALUE       0x23 //Регистр года (0-99) байт
#define REG_HOURVALUE       0x23 //Регистр часа (0-23) байт
#define REG_MINVALUE        0x24 //Регистр минуты (0-59) байт
#define REG_SECVALUE        0x25 //Регистр секунды (0-59) байт

//Регистры датчика
#define REG_ID              0x10  //Регистр идентификатора датчика, должен быть 0x53
#define REG_FIRST           0x20  //Первый регистр блока (температура, влажность, давление)
#define REG_TEMPVALUE_L     0x20 //Регистр Температуры в градусах Цельсия младший байт
#define REG_TEMPVALUE_H     0x21 //Регистр Температуры в градусах Цельсия старший байт
#define REG_HUMVALUE_L      0x22 //Регистр Относительной Влажности в % младший байт
#define REG_HUMVALUE_H      0x23 //Регистр Относительной Влажности в % старший байт
#define REG_PRESSKPAVALUE_L 0x24 //Регистр Атмосферного Давления в кПа младший байт
#define REG_PRESSKPAVALUE_H 0x25 //Регистр Атмосферного Давления в кПа  старший байт
#define REG_PRESSHGVALUE_L  0x26 //Регистр Атмосферного Давления в мм.рт.ст младший байт
#define REG_PRESSHGVALUE_H  0x27 //Регистр Атмосферного Давления в мм.рт.ст старший байт

//Адрес устройства на шине I2C
#define ADR_METEO           0x24

#define ADR_MOISTURE      0x28

//Регистры датчика
#define REG_ID       0x10  //Регистр идентификатора датчика, должен быть 0x57
#define REG_MOISTUREVALUE  0x20 //Влажность почвы в % байт

 void setEncToZero(int encoder);
 long getEncDegrees(int encoder); double getMRTState(int encoder);
 void setupEnc(int encoder, int CH_A, int CH_B);
 void SetupMRTEncoder(int encoder, int CH_A, int CH_B);



 //void test();

 typedef enum{
 	GYRO_X_AXIS = 0,
 	GYRO_Y_AXIS = 1,
 	GYRO_Z_AXIS = 2,
 } gyroAxis;
 

 void setupGyro(void);
 int GyroScan(gyroAxis);

 void trackduinoInit(void);

 void stopProgram();

 // -----------1st-level control functions------------
 void motorsAccelerationSetup(unsigned int millisToFullSpeed);
 void motorsHandler();

 void changeMotorSpeed(int motorPort, int speed);
 void changeMotorDirection(int motorPort, bool direction);

 void startMotor(int motorPort, int speed);
 void rotateMRT(int motorPort, int speed);
 void rotateMRT_to_angle(bool accuracy, int motorPort, int speed,  int angle);
 void rotateMRT_to_state(bool accuracy, int motorPort, int speed,  int angle);
 int objectsAmountInFrame();
 void rotateMRTPair_to_angle(bool accuracy, int port1, int port2, int speed1, int speed2, int angle1, int angle2);
 void rotateMRTPair_to_state(bool accuracy, int port1, int port2, int speed1, int speed2, int angle1, int angle2);
 int getXCoordinate(int obj_num);  int getYCoordinate(int obj_num);
 int getX1Coordinate(int obj_num); int getX2Coordinate(int obj_num); int getY1Coordinate(int obj_num); int getY2Coordinate(int obj_num);
 int getWidthParam(int obj_num);  int getHeightParam(int obj_num);
 void saveFace(uint8_t object_id); void saveObject(uint8_t object_id);
 void deleteObject(uint8_t object_id); void deleteFace(uint8_t object_id); void deleteAllObjects(); void deleteAllFaces();
  bool isFaceRecognized(int id); bool isObjectRecognized(int id);
  void setBlobParam(int width, int height, int color);
  bool isBlobRecognized(int color);
  int blobColorRecognition(int num);
  int colorRecognition(int num);
  int cardClassRecognition(int num);
  int trafficCard(int card); int shapeCard(int card); int numberCard(int card);
   char* QrCodeRec();
   int classDef(int obj);
   bool isClassRecognized (int obj);
   int classNumberRecognition(int num);
   int apriltagNumberRecognition(int num);
   void setColorParam(int w, int h, int x, int y);
    int convertStrToInt(char* str);
   



 void stopMotor(int motorPort);
 void setDate(short day, short month, short year);  void setTime(short hours, short minutes, short seconds);
 int getMonth();
  int getDay();
   int getYear();
   int getHour();
   int getMinute();
   int getSecond();
  double currentTemp();
  double currentHumidity();
  double current_kPa_preasure();   double current_mm_preasure();
  double soil_moisture();
  void electromagnet(int, bool);
  bool serialParseBool();
  char serialReadChar();

 void stopMotorHard (int motorPort);

 void releaseServo(int outPort);
  void clearMRTEncoder(int encoder);
  
  void StartSmartCameraMode(int mode);
  bool startCamera();
 
 // -----------2nd-level control functions------------
 
 
 // -----------RGB LED functions------------
 void builtInRGB(int color);
 
 
 //-------------Sensors read functions----------
 bool buttonRead(int port);
 
 bool compareStrings(char* str1, char* str2);
 bool findSubstring(char* str1, char* str2);
 
 int distanceCM(int port);
 
 bool irWhite(int port, int threshold);
 bool irBlack(int port, int threshold);
 int irAnalog(int port);
 bool flame(int port); 
 bool shock(int port);
 int temperature(int port);
 
 bool mic(int port);
 int micAnalog(int port);
 
 bool cdsLight(int port, int threshold);
 bool cdsDark(int port, int threshold);
 int cdsAnalog(int port);
 //------------Actuators funcrions------------
 
 void led (int ledPort, bool state);
 
 void ledPWM (int ledPort, int PWM);
 
void relay(int relayPort, bool state);

 
 // -----------Color sensor------------
 void setupColorSensor(int Rpin, int Bpin, int Gpin, int CDSpin);
 int getColor(void);
 int defineColor(void);
 int getCDSReading(int colorSensorCDSpin, int times);
 bool metal (int port);
 bool tilt (int port);
 bool magnetic (int port);
 int bend (int port);
 int skin (int port);
 
 
 // --------------IR RC ----------------
 /*
 	IRDecode.h - Library IR controller.
 	Created by Nesterov D.A., October 5, 2015.
 */
 // #ifndef IRRC_h
 // #define IRRC_h
 
 // #if defined(ARDUINO) && ARDUINO >= 100
   // #include "Arduino.h"
 // #else
   // #include "WProgram.h"
 // #endif 
 
 
 
 
 
 // ---------------- Artintrack ----------------


 bool comm_send(char data);
int32_t comm_recv();
void delay_ms(uint32_t value); 
 uint32_t TS_ms(void);
 void artintrackInit(int uart);
 void artintrackUpdate(int nn);
 void artintrackCheckForErrors();
 int artintrackCheckFace();
 bool artintrackSaveFace();
 bool artintrackClearFaces();
 

 
 #define Timeout 500
 
 #define Channl_1 0xFC
 #define Channl_2 0x3C
 #define Channl_3 0xCC
 #define Channl_4 0x0C
 #define Channl_5 0xF0
 #define Channl_6 0x30
 #define Channl_7 0xC0
 #define Channl_8 0x00
 
 #define Shift 8
 
 #define UP_KEY 0x1FC3
 #define DOWN_KEY 0x1F
 #define LEFT_KEY 0x07
 #define RIGHT_KEY 0x73
 
 #define F1_KEY 0x7CF
 #define F2_KEY 0x1C3F
 #define F3_KEY 0x7F3
 #define F4_KEY 0x1CCF
 #define F5_KEY 0x1F0F
 #define F6_KEY 0x703
 
 #define OFF_KEY 0x733
 
 #define UP_AND_LEFT_KEY 0x7C3
 #define UP_AND_RIGHT_KEY 0x7F
 #define DOWN_AND_LEFT_KEY 0x70F
 #define DOWN_AND_RIGHT_KEY 0x1CF

 #define GRAPH_LEN 128
 	
 //#define  SoftPWMSet(x,y, z) analogWrite(x,y)	
 	
 	void IRRC_setup(int pin, uint8_t Channl);
 	void change();
 	//void Set_Channle(byte Channle);
 	bool RC(long BT_ID);
 	
 // #endif
 
 //======================BLUETOOTH RC==========================
 void BTinit(void);
 void clearValues();
 //String getString(String name, int value);
 //String getAnotherString(String name, boolean value);
 //void showValues();
 void calculateValues();
 void BTEventRun(void);
 
 void NEUROinit(void);
 void NEUROEventRun(void);
 // unsigned int readNeuroInterface(int requestedData);
 //bool containsNestedKey(const JsonObject& obj, const char* key);
 //void analogWrite (int port, int value);

  //======================U8GLIB==========================
  void oledPoint(int x, int y);
  void oledLine(int x, int y, int x2, int y2);
  void oledBox(int x, int y, int w, int h);
  void oledFrame(int x, int y, int w, int h);
  void oledCircle(int x, int y, int r);
  void oledTriangle(int x, int y, int x2, int y2, int x3, int y3);
  void oledClear();
  uint8_t nextPageAvailable();
  void oledGraph(int newValue, int minValue, int maxValue, int graphHeight);
  void oledText(int x, int y, char* text, int font);
