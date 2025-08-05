#define CS_USE_NIMBLE
#include "Button2.h"

#include <Control_Surface.h>
#include <MIDI_Interfaces/BluetoothMIDI_Interface.hpp>

#define Led        2
#define button_bl 27
#define button_gr 26
#define button_rd 25

BluetoothMIDI_Interface midi_ble;

bool activation = false , isTuner = false;
long val;
Button2 button_blue, button_red, button_green;



void setup() {
  midi_ble.setName("Kiosk Pedal Board");
  val = 1;
  Serial.begin(115200);
  delay(50);
  pinMode(Led, OUTPUT);
  button_blue.begin(button_bl);
  button_red.begin(button_rd);
  button_green.begin(button_gr);

  button_blue.setClickHandler(click);
  button_red.setClickHandler(click);
  button_green.setClickHandler(click);
  button_blue.setLongClickDetectedHandler(longClickDetected);
  button_red.setLongClickDetectedHandler(longClickDetected);
  button_green.setLongClickDetectedHandler(longClickDetected);
  midi_ble.begin();

}

void loop(){
if(!midi_ble.isConnected()){
 Serial.println("Not Connected!");
 digitalWrite(Led, HIGH);
 delay(100);
 digitalWrite(Led,LOW);
 delay(100);
} else{
  digitalWrite(Led,HIGH);
  button_blue.loop();
  button_red.loop();
  button_green.loop();


}
 //  

  
 
  delay(10);
}

void longClickDetected(Button2& btn){


    if(btn == button_blue){
          Serial.println("Preset Previous");
       midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 24, 127);


    } else if (btn == button_red) {
               Serial.println("Preset Next");
      midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 25, 127);  
    }else if (btn == button_green) {
               Serial.println("Tuner");
      midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 58, 127);  
      isTuner = true;
    }

}

void click(Button2& btn){
    val++;
  if(btn == button_red){
  //DRIVE CONTROLLER on and off
  if(val % 2 == 0){
  midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 45, 127);

  } else {
  midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 45, 0);
  }
  Serial.println(val % 2 == 0? "DRIVE On!": "DRIVE off!");
  } else if (btn == button_blue){
     //FX1 CONTROLLER on and off
   if(val % 2 == 0){
    midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 44, 127);

  } else {
    midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 44, 0);
  } 
  Serial.println(val % 2 == 0? "FX1 On!": "FX1 off!");
  } else if (btn == button_green){
      //RVB CONTROLLER on and off
   if(val % 2 == 0){
  midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 51, 127);

  } else {
    midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 51, 0);
  } 
  Serial.println(val % 2 == 0? "RVB On!": "RVB off!");
  }
  if(isTuner == true){
        midi_ble.sendChannelMessage(MIDIMessageType::ControlChange, Channel(1), 58, 0);
      }
}

