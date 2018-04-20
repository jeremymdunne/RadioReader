#include <Arduino.h>

int channelInputPins[5] = {PC13,PC14,PC15,PA0,PA1}; //hardware pins. these should be sequential
int channelValues[5] = {0,0,0,0,0}; //actual values from 0 - 2000 us
long channelRisingTimes[5] = {0,0,0,0,0}; //time at which a rising was recorded

int lastChannelWithRise = -1; //indexed to the channelInputPins array
bool middleOfRise = false;
long tempTime = 0;


void change(){
    Serial.println("Change!");
    tempTime = micros();
    for(int i = 0; i < 5; i ++){
      if(digitalRead(channelInputPins[i])){
        channelRisingTimes[i] = tempTime;
        middleOfRise = true;
        lastChannelWithRise = i;
      }
    }
    if(!middleOfRise && lastChannelWithRise != -1){
        channelValues[lastChannelWithRise] = tempTime - channelRisingTimes[lastChannelWithRise];
        middleOfRise = false;
        lastChannelWithRise = -1;
    }
}



void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    for(int i = 0; i < 5; i ++){
      pinMode(channelInputPins[i],INPUT);
      attachInterrupt(channelInputPins[i], change, CHANGE);
    }

}

String stringifyChannelsInputs(){
  String values = "Channel Values: ";
  for(int i = 0; i < 5; i ++){
    values += "; " + String(i) + ": " +  String(channelValues[i]);
  }
  return values;
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println(stringifyChannelsInputs());
    delay(20);
}
