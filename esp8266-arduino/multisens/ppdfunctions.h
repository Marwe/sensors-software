#ifndef PPDFUNCTIONS_H
#define PPDFUNCTIONS_H

#ifdef PPD_ACTIVE

// P1 for PM10 & P2 for PM25
boolean valP1 = HIGH;
boolean valP2 = HIGH;
boolean trigP1 = false;
boolean trigP2 = false;

unsigned long durationP1;
unsigned long durationP2;
unsigned long trigOnP1;
unsigned long trigOnP2;
unsigned long lowpulseoccupancyP1 = 0;
unsigned long lowpulseoccupancyP2 = 0;

float ratio_p1 = 0;
float concentration_p1 = 0;
float ratio_p2 = 0;
float concentration_p2 = 0;

// String versions (convert only once)
String ratio_p1s;
String ratio_p2s;
String concentration_p1s;
String concentration_p2s;
String lowpulseoccupancyP1s;
String lowpulseoccupancyP2s;


// #ifndef PPD42_P1_PIN
// #define  PPD42_P1_PIN 12
// #endif
// 
// #ifndef PPD42_P2_PIN
// #define  PPD42_P2_PIN 14
// #endif

void ppd_init(unsigned int pin_p1, unsigned int pin_p2){
  pinMode(pin_p1,INPUT); // Listen at the designated PIN
  pinMode(pin_p2,INPUT); //Listen at the designated PIN
}


boolean ppd_measure(){
  // measure
  // create string
  // push
  
}

#endif //PPD_ACTIVE
#endif //PPDFUNCTIONS_H