 /*******************************************************/
/* OK LAB Particulate Matter Sensor                    */
/*      - nodemcu-LoLin board                          */
/*      - Shinyei PPD42NS                              */
/*      http://www.sca-shinyei.com/pdf/PPD42NS.pdf     */
/*                                                     */       
/*******************************************************/

/**********************************************/
/* configure your sensors,                    */
/* PINs and WIFI in sensorconfig.h            */
/**********************************************/
#include "sensorconfigvar.h" // config with variables
#include "sensorconfigdefs.h" // config with defines, will be superseeded by sensorconfigvar.h

#include "serialfunctions.h"

#ifdef WIRELESS_ACTIVE
#include <ESP8266WiFi.h>
#endif
unsigned long starttime;
int value = 0;
// String software_version_s=String(SOFTWARE_VERSION_INITIALS)+"_"+String(SOFTWARE_VERSION_NUMBER);
#include "apifunctions.h"

/**********************************************/
/* MQTT declarations: see sensorconfig.h      */
/**********************************************/
#ifdef PUSHTO_MQTT
// in PubSubClient.h, MQTT_MAX_PACKET_SIZE restricts 
//the maximum length of a message to 128, this is not enough here: redefine
#define MQTT_MAX_PACKET_SIZE 4096
#undef MAX_TRANSFER_SIZE
#include <PubSubClient.h>
#include "mqttfunctions.h"
#endif

/**********************************************/
/* DHT declaration 
/**********************************************/
#ifdef DHT_ACTIVE
// The IDE requires all libraries to be #include’d in the main (.ino) file.
// http://jamesreubenknowles.com/multiple-source-files-with-the-arduino-ide-1618
#include <DHT.h>
#include "dhtpush.h"
#endif

/**********************************************/
/* onewire ds18x20 temperature sensor declaration 
/**********************************************/
// derived from https://github.com/esp8266/Arduino/blob/esp8266/libraries/OneWire/examples/DS18x20_Temperature/DS18x20_Temperature.pde
#ifdef DS_ACTIVE
#include <OneWire.h>
#include "ds18x20push.h"
// on pin ONEWIRE_PIN = GPIO (a 4.7K pullup resistor is necessary)
# endif

/**********************************************/
/* WiFi declarations: see sensorconfig.h      */
/**********************************************/


/**********************************************************/
/* Variable Definitions for PPD24NS moved to ppdfunctions */
/**********************************************************/
#ifdef PPD_ACTIVE
#include "ppdfunctions.h"
#endif

#ifdef PIN_LED_STATUS
int ledsstate=LOW;
#endif

/**********************************************/
/* The Setup
/**********************************************/

void setup() {

#ifdef PIN_LED_STATUS
pinMode(PIN_LED_STATUS, OUTPUT);
digitalWrite(PIN_LED_STATUS, ledsstate);
#endif

#ifdef PIN_LED_STATUS
ledsstate=HIGH;
digitalWrite(PIN_LED_STATUS, ledsstate);
#endif

  init_serial(9600);
  if(send2csv){debug=false;}
  debugout_serial(system_info());
  debugout_serial(config_info());


#ifdef PPD_ACTIVE
#ifdef PIN_LED_STATUS
ledsstate=LOW;
digitalWrite(PIN_LED_STATUS, ledsstate);
#endif
ppd_init(pin_ppd_p1, pin_ppd_p2);
#ifdef PIN_LED_STATUS
ledsstate=HIGH;
digitalWrite(PIN_LED_STATUS, ledsstate);
#endif
#endif

#ifdef DHT_ACTIVE
#ifdef PIN_LED_STATUS
ledsstate=LOW;
digitalWrite(PIN_LED_STATUS, ledsstate);
#endif
  Serial.println("Sensortype DHT active");  
  Serial.print("DHTPIN: ");
  Serial.println(DHTPIN);
  Serial.println("init DHT");
#ifdef PIN_LED_STATUS
ledsstate=HIGH;
digitalWrite(PIN_LED_STATUS, ledsstate);
#endif
#endif


#ifdef DS_ACTIVE
#ifdef PIN_LED_STATUS
ledsstate=LOW;
digitalWrite(PIN_LED_STATUS, ledsstate);
#endif
  Serial.println("Sensortype DS active");  
  Serial.print("ONEWIRE_PIN: ");
  Serial.println(ONEWIRE_PIN);
  Serial.print("init DS");
  Serial.println();
  DSinit();
  delay(10);
  // try to read a temperature
  float t;
  t=DSgetTemperature();
  Serial.print("DSgetTemperature(): t=");
  Serial.println(t);
#endif
  connectWifi(); // Start ConnecWifi
  Serial.print("\n"); 
#ifdef PIN_LED_STATUS
ledsstate=HIGH;
digitalWrite(PIN_LED_STATUS, ledsstate);
#endif
#ifdef PUSHTO_MQTT
mqtt_setup();
#endif

  starttime = millis(); // store the start time

}

/**********************************************/
/* And action
/**********************************************/
void loop() {
  String data;
  // Read pins connected to ppd42ns
#ifdef PPD_ACTIVE
  valP1 = digitalRead(PPD42_P1_PIN);
  valP2 = digitalRead(PPD42_P2_PIN);

  if(valP1 == LOW && trigP1 == false){
    trigP1 = true;
    trigOnP1 = micros();
  }
  
  if (valP1 == HIGH && trigP1 == true){
    durationP1 = micros() - trigOnP1;
    lowpulseoccupancyP1 = lowpulseoccupancyP1 + durationP1;
    trigP1 = false;
  }
  
  if(valP2 == LOW && trigP2 == false){
    trigP2 = true;
    trigOnP2 = micros();
  }
  
  if (valP2 == HIGH && trigP2 == true){
    durationP2 = micros() - trigOnP2;
    lowpulseoccupancyP2 = lowpulseoccupancyP2 + durationP2;
    trigP2 = false;
  }
#endif
  // Checking if it is time to sample
  if ((millis()-starttime) > sampletime_ms)
  {
#ifdef PPD_ACTIVE
    ratio_p1 = lowpulseoccupancyP1/(sampletime_ms*10.0);                 // int percentage 0 to 100
    concentration_p1 = 1.1*pow(ratio_p1,3)-3.8*pow(ratio_p1,2)+520*ratio_p1+0.62; // spec sheet curve
    ratio_p1s=Float2String(ratio_p1);
    concentration_p1s=Float2String(concentration_p1);
    lowpulseoccupancyP1s=String(lowpulseoccupancyP1);
    // Begin printing
    Serial.print("LPO P10     : ");
    Serial.println(lowpulseoccupancyP1);
    Serial.print("Ratio PM10  : ");
    Serial.print(ratio_p1s);
    Serial.println(" %");
    Serial.print("PM10 Count  : ");
    Serial.println(concentration_p1s);

    // json for push to api / P1
    data = "{\"software_version\": \"";
    data += software_version_s;
    data += "\",";
    data += "\"sensordatavalues\":[{";
    data += "\"value_type\":\"durP1\",\"value\":\"";
    data += lowpulseoccupancyP1s;
    data += "\"},{";
    data += "\"value_type\":\"ratioP1\",\"value\":\"";
    data += ratio_p1s;
    data += "\"},{";
    data += "\"value_type\":\"P1\",\"value\":\"";
    data += concentration_p1s;
    data += "\"},{";

    ratio_p2 = lowpulseoccupancyP2/(sampletime_ms*10.0);
    concentration_p2 = 1.1*pow(ratio_p2,3)-3.8*pow(ratio_p2,2)+520*ratio_p2+0.62;
    ratio_p2s=Float2String(ratio_p2);
    concentration_p2s=Float2String(concentration_p2);
    lowpulseoccupancyP2s=String(lowpulseoccupancyP2);
    // Begin printing
    Serial.print("LPO PM25    : ");
    Serial.println(lowpulseoccupancyP2s);
    Serial.print("Ratio PM25  : ");
    Serial.print(ratio_p2s);
    Serial.println(" %");
    Serial.print("PM25 Count  : ");
    Serial.println(concentration_p2s);

    // json for push to api / P2
    data += "\"value_type\":\"durP2\",\"value\":\"";
    data += lowpulseoccupancyP2s;
    data += "\"},{";
    data += "\"value_type\":\"ratioP2\",\"value\":\"";
    data += ratio_p2s;
    data += "\"},{";
    data += "\"value_type\":\"P2\",\"value\":\"";
    data += concentration_p2s;
    data += "\"}]}";

    //sending to dustix api
    Serial.println("#### Sending to dusti.xyz: ");
    // -1 -> '-' is default for ppd
    sendData(data,-1);
    
    #ifdef PUSHTO_MQTT//process incoming data/ping
       Serial.print("PubSubClient::loop");
       if (psclient.loop()){
           Serial.println("OK");
        }else{
           Serial.println("failed");
        }
        mqtt_publish_subtopic("json/PPD42NS",data);
        mqtt_publish_subtopic("PPD42NS/durP1",lowpulseoccupancyP1s);
        mqtt_publish_subtopic("PPD42NS/durP2",lowpulseoccupancyP2s);
        mqtt_publish_subtopic("PPD42NS/ratioP1",ratio_p1s);
        mqtt_publish_subtopic("PPD42NS/ratioP2",ratio_p2s);
        mqtt_publish_subtopic("PPD42NS/P1",concentration_p1s);
        mqtt_publish_subtopic("PPD42NS/P2",concentration_p2s);
    #endif

#endif
    // FIXME: option to send PIN
#ifdef DHT_ACTIVE
    sensorDHT();  // getting temperature and humidity (optional)
    Serial.println("------------------------------");
#endif
#ifdef DS_ACTIVE
    DSpush();
#endif

  // Resetting for next sampling
  lowpulseoccupancyP1 = 0;
  lowpulseoccupancyP2 = 0;
  yield(); // Do (almost) nothing -- yield to allow ESP8266 background functions
  // reset start time at the very end
  starttime = millis(); // store the start time
  }

}
