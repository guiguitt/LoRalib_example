  #include "Lora_Module.h"
#include "Conversion.h"
#include <ArduinoLowPower.h>


Lora_Module lora;
Conversion conv;

#define tempo 10000 //en ms
int32_t latitude;
int32_t longitude;
uint8_t detec;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  if(debug>0){
    Serial.begin(115200);
    //while(!Serial){}
  }
  if(debug>2)Serial.println("-------------------------------LoRa------------------------------");
  if(debug>2)Serial.println("initialisation...");
  lora.Init();
  if(debug>1)lora.info_connect();
  latitude = conv.float_int32("43.7810", 5);
  longitude = conv.float_int32("4.56", 5);
  detec=true;

}
void loop()
{ 
  digitalWrite(LED_BUILTIN, HIGH);
  if(detec==true){
    detec=false;
  }
  else{
    detec=true;
  }
  Serial.println(detec);
  digitalWrite(LED_BUILTIN, LOW );
  uint8_t buffer[12];
  buffer[0] = (uint8_t)0x02;
  buffer[1] = (uint8_t)(longitude >> 24);
  buffer[2] = (uint8_t)(longitude >> 16);
  buffer[3] = (uint8_t)(longitude >> 8);
  buffer[4] = (uint8_t)longitude;
  
  buffer[5] = (uint8_t)0x03; 
  buffer[6] = (uint8_t)(latitude >> 24);
  buffer[7] = (uint8_t)(latitude >> 16);
  buffer[8] = (uint8_t)(latitude >> 8);
  buffer[9] = (uint8_t)latitude;

  buffer[10] = (uint8_t)0x07;
  buffer[11] = (uint8_t)detec;
  
  digitalWrite(LED_BUILTIN, HIGH );
  lora.send(buffer, 12);
  digitalWrite(LED_BUILTIN, LOW);
  if(debug>2){
    Serial.print("veille pendant "); Serial.print(tempo/1000); Serial.println(" s");
  }
  //LowPower.deepSleep(tempo-3000); // veille profonde seul la RTC reste allumer, temps de reveille long
  //LowPower.sleep(tempo);//veille mode
  //LowPower.idle(tempo);// stanby mode,temps de reveille rapide
  delay(tempo);// mise en pause classique, consommation important, pas de temps de reveille

}
