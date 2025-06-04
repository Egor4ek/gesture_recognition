#include "Arduino.h"
#include "NNTrack.h"
#include <String.h> 

namespace NNTrack{
void begin() {Serial.begin(115200);}

String _recievedData = "";
String _parcedData[3];

void update(){
    _recievedData = Serial.readStringUntil('\n');
    if (_recievedData == ""){
        return;
    };
    int _first_index  = _recievedData.indexOf(':');
    int _second_index = _recievedData.lastIndexOf(':');
    _parcedData[0] = _recievedData.substring(0, _first_index);
    _parcedData[1] = _recievedData.substring(_first_index + 1, _second_index);
    _parcedData[2] = _recievedData.substring(_second_index + 1);
}

int classNumber() {
    if (_parcedData != ""){   
    return _parcedData[0].toInt();
    }
    else{
        return -1;
    }   
}
String className() {return _parcedData[1]; }
int classConfidence() {return _parcedData[2].toInt();}
} //Namespace NNTrack