/*
name this file sensorconfigvar.h
and change your config here
*/
// #define SOFTWARE_VERSION_INITIALS "MARWE"
// additionally set SOFTWARE_VERSION_NUMBER in softwareversion.h
#include "softwareversion.h"
software_version = String("MULTISENS")+software_version;

// GPIO PINSs <-> nodemcu PINs (Dx) mapping
//          0  1  2  3  4   5   6   7   8  9  10  11  12
int D[13]={16, 5, 4, 0, 2, 14, 12, 13, 15, 3,  1,  9, 10};

/**************************/
/* sensor activation      */
/**************************/
bool ppd_read = false;
bool dht_read = false;
bool sds_read = false; // TODO: implement sds
bool ds_read = false;

/* sampletime */
unsigned long sampletime_ms = 30000;

/**************************/
/*     PIN definitions    */
/**************************/

/* PPD PINs */
unsigned int pin_ppd_p1=D[6];
unsigned int pin_ppd_p2=D[5];

/* DHT PIN */
unsigned int pin_dht=D[2];
#define DHTTYPE DHT22

/* ONEWIRE BUS PIN FOR DS sensors */
unsigned int pin_ds=D[4];

/* LED PIN */
unsigned int pin_led_status=D[8]; // the GPIO of the LED pin

bool wireless_active=true; // use wireless
bool wificonfig=true; // enable configuration via wifi (AP mode)
bool send2dusti = true; // send data to api.luftdaten.info API
bool send2madavi = false; // TODO: implement madavi
bool send2mqtt = false; // send data to MQTT
bool send2csv = false; // output as csv over serial
bool debug = false; // debug outputs over serial

/**************************/
/*     API definitions    */
/**************************/

/* WIFI definitions */
char* ssid = "Freifunk";
char* password = "";

/* django api */
char* djangoapihost = "api.luftdaten.info";
int djangoapihttpPort = 80;

/* MQTT settings */
char mqttserver[] = "mqtt.opendataset.click";
int mqttport = 1883;
char mqttusername = "";
char mqttpassword = "";
char mqtttopicbase[]="dusti"; // no "/"
char mqttintopic[]="configchange";

/* madavi api */
// TODO: implement madavi
char* madaviapihost = "www.madavi.de";
char* madaviapiurl = "/sensor/data.php";
int madaviapihttpPort = 80;

