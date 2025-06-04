#include <NNTrack.h>
#include <Servo.h>

Servo digitalServoOUT1;
Servo digitalServoOUT2;
Servo digitalServoOUT3;

void sgat()
{
  relay ( OUT4 , HIGH );
  relay ( OUT6 , LOW );
  delay( 1000 );
  relay ( OUT4 , LOW );
  relay ( OUT6 , LOW );
}

void right()
{
  rasgat();
  digitalServoOUT3.write(145, 20);
  digitalServoOUT2.write(145, 20);
  delay( 2000 );
  sgat();
  delay( 500 );
  digitalServoOUT2.write(80, 20);
  digitalServoOUT3.write(90, 20);
  delay( 1000 );
  digitalServoOUT1.write(150, 20);
  delay( 2000 );
  digitalServoOUT2.write(118, 20);
  digitalServoOUT3.write(125, 20);
  delay( 1500 );
  rasgat();
  delay( 500 );
  digitalServoOUT2.write(80, 40);
  digitalServoOUT3.write(90, 40);
  delay( 500 );
  digitalServoOUT1.write(87, 40);
  delay( 1000 );
}

void left()
{
  rasgat();
  digitalServoOUT3.write(165, 20);
  digitalServoOUT2.write(163, 20);
  delay( 2000 );
  sgat();
  delay( 500 );
  digitalServoOUT2.write(80, 20);
  digitalServoOUT3.write(90, 20);
  delay( 1000 );
  digitalServoOUT1.write(27, 20);
  delay( 2000 );
  digitalServoOUT2.write(125, 20);
  digitalServoOUT3.write(130, 20);
  delay( 1500 );
  rasgat();
  delay( 500 );
  digitalServoOUT2.write(80, 40);
  digitalServoOUT3.write(90, 40);
  delay( 500 );
  digitalServoOUT1.write(87, 40);
  delay( 1000 );
}

void rasgat()
{
  relay ( OUT4 , LOW );
  relay ( OUT6 , HIGH );
  delay( 1000 );
  relay ( OUT4 , LOW );
  relay ( OUT6 , LOW );
}

void setup()
{
  NNTrack::begin();
  Serial.begin(115200);

  digitalServoOUT1.attach(OUT1);
  digitalServoOUT2.attach(OUT2);
  digitalServoOUT3.attach(OUT3);
  pinMode( OUT4 , OUTPUT );
  pinMode( OUT6 , OUTPUT );
  artintrackInit(1);
  digitalServoOUT1.write(87, 40);
  digitalServoOUT2.write(80, 40);
  digitalServoOUT3.write(90, 40);
  delay( 1000 );
}

void loop()
{
  NNTrack::update();
  if (( ( ( NNTrack::classNumber() ) == ( 0 ) ) && ( ( NNTrack::classConfidence() ) > ( 90 ) ) ))
  {
    right();
    Serial.println((String)"Распознан жест");
    Serial.println((String)NNTrack::className());
  }
  if (( ( ( NNTrack::classNumber() ) == ( 1 ) ) && ( ( NNTrack::classConfidence() ) > ( 90 ) ) ))
  {
    left();
    Serial.println((String)"Распознан жест");
    Serial.println((String)NNTrack::className());
  }
  if (( ( ( NNTrack::classNumber() ) == ( 2 ) ) && ( ( NNTrack::classConfidence() ) > ( 90 ) ) ))
  {
    sgat();
    Serial.println((String)"Распознан жест");
    Serial.println((String)NNTrack::className());
  }
  if (( ( ( NNTrack::classNumber() ) == ( 3 ) ) && ( ( NNTrack::classConfidence() ) > ( 90 ) ) ))
  {
    rasgat();
    Serial.println((String)"Распознан жест");
    Serial.println((String)NNTrack::className());
  }
}
