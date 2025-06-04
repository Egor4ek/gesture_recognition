#include "Arduino.h"
#include "TrackduinoRemote.h"
#include "../../libraries/ArduinoJson/ArduinoJson.h"

//volatile int _uart;
//HardwareSerial *_port = &Serial;
//_port = &Serial;
// volatile int cursorY, cursorX;
// String linesholder[9];
// int linepointer = 0;





int requestNeuroData(int requestedNeuroData){

	Serial.begin(115200);
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["Nreq"] = requestedNeuroData;

	Serial.print("<");
	root.printTo(Serial);
	Serial.print(">");
	Serial.flush();

	return parseNeuroData(requestedNeuroData);
}



int parseNeuroData(int requestedNeuroData){
	//unsigned long startTime = millis();
	const int bSize = 200; 
	char Buffer[bSize];
	Buffer[0] = (char)0;//null the first byte of char array, thus terminating it
	int attention = 0, meditation = 0, blink = 0;
	
	Serial.setTimeout(200);//time to wait for reponse
	Serial.readStringUntil('<');//wait for reponse or clean old data in buffer
	//Serial.read();//remove ">" symbol
	Serial.setTimeout(10);//
	int end = Serial.readBytesUntil('>',Buffer,bSize); 
	Buffer[end] = (char)0;//clean rest of buffer

	//char Buffer2[] = "{\"n\":{\"m\":321}}";
	//Serial1.print("Buffer: ");
	//Serial1.println(Buffer);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(Buffer);
	if (!root.success()) 
	{
		//Serial1.println("parseObject() failed");
		return 0;
	}
	//Serial1.print("Op time ");
	//Serial1.println((millis())-startTime);
	if (root.containsKey("n"))//neuro data
	{
		//Serial1.println("neuro detected");
		if (requestedNeuroData == 1)
		{
			attention = root["n"]["a"];
			return attention;
		}
		else if (requestedNeuroData == 2)
		{
			meditation = root["n"]["m"];
			return meditation;
		}
		else if (requestedNeuroData == 3)
		{
			blink = root["n"]["b"];
			return blink;
		}

	}

	return 0;

}

//analogs


remoteSensor::remoteSensor(int sensorType, int port)
{
	
	this->sensorType = sensorType;
	this->port = port;
	Serial.begin(115200);

}

remoteSensor::remoteSensor(int sensorType)
{
	
	this->sensorType = sensorType;
	this->port = 0;
	Serial.begin(115200);

}




void remoteSensor::send(void)
{
	//do nothing

}

void remoteSensor::send(int data)
{
	Serial.begin(115200);
	this->data = data;
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["st"] = this->sensorType;//sensorType
	root["p"] = this->port;//port
	root["d"] = this->data;//data
	Serial.print("<");
	root.printTo(Serial);
	Serial.println(">");
	delay(20);
	

}

void remoteSensor::send(bool data)
{
	Serial.begin(115200);
	this->data = data;
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["st"] = this->sensorType;//sensorType
	root["p"] = this->port;//port
	root["d"] = this->data;//data
	Serial.print("<");
	root.printTo(Serial);
	Serial.println(">");
	delay(20);
	
}

void remoteSensor::send(float data)
{
	Serial.begin(115200);
	this->data = data;
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["st"] = this->sensorType;//sensorType
	root["p"] = this->port;//port
	root["d"] = this->data;//data
	Serial.print("<");
	root.printTo(Serial);
	Serial.println(">");
	delay(20);
}

void remoteSensor::send(long data)
{
	Serial.begin(115200);
	this->data = data;
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["st"] = this->sensorType;//sensorType
	root["p"] = this->port;//port
	root["d"] = this->data;//data
	Serial.print("<");
	root.printTo(Serial);
	Serial.println(">");
	delay(20);
}
	

int requestRCData(int sensorType, int port){
Serial.begin(115200);
StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["RCreq"] = 1;
	root["st"] = sensorType;
	root["p"] = port;

	Serial.print("<");
	root.printTo(Serial);
	Serial.print(">");
	Serial.flush();

	return receiveRCData();
}

int requestRCData(int sensorType){
	
	requestRCData(sensorType, 0);
	
}


int receiveRCData(void){
const int bSize = 200; 
	char Buffer[bSize];
	Buffer[0] = (char)0;//null the first byte of char array, thus terminating it
	int data = 0;
	
	Serial.setTimeout(200);//time to wait for reponse
	Serial.readStringUntil('<');//wait for reponse or clean old data in buffer
	//Serial.read();//remove ">" symbol
	Serial.setTimeout(10);//
	int end = Serial.readBytesUntil('>',Buffer,bSize); 
	Buffer[end] = (char)0;//clean rest of buffer

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(Buffer);
	if (!root.success()) 
	{
		//Serial1.println("parseObject() failed");
		return 0;
	}
	//Serial1.print("Op time ");
	//Serial1.println((millis())-startTime);
	if (root.containsKey("RC"))//neuro data
	{
		//Serial1.println("neuro detected");
		
			data = root["RC"]["d"];
			return data;
		
	}

	return 0;

}