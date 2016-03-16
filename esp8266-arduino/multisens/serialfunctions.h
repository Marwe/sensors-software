// serial functions, debug output, csv output

void init_serial(const unsigned int baudrate = 9600){ 
  Serial.begin(baudrate); //Output to Serial at x baud
  delay(10);
}

void debugout_serial(String msg){
  if (!debug){return;}
  Serial.println(msg);
}

String system_imfo(){
  String info="Software version: "+software_version;
  info+="\n";
  info+="ESP chipid: ";
  info+=String(ESP.getChipId());
  info+="\n";
  info+="Heap free: ";
  info+=String(ESP.getFreeHeap());
  return (info);
}

String config_info(){
  String info="sampletime [s]: ";
  info+=Float2String(sampletime_ms/1000.0);
  info+="\n";
  info+="GPIOx Dx PIN matching:\n";
  int i;
  for (i = 0; i < 13; i = i + 1) {
      info+=String(i)+"\t"+String(D[i])+"\n";
  }
  info+="Sensors:\n"
  info+="ppd_read = "+String(ppd_read)+"\n";
  info+="dht_read = "+String(dht_read)+"\n";
  info+="sds_read = "+String(sds_read)+"\n";
  info+="ds_read = "+String(ds_read)+"\n";

  info+="Pins (GPIO):\n";
  info+="Pin PPD P1 = "+String(pin_ppd_p1)+"\n";
  info+="Pin PPD P2 = "+String(pin_ppd_p2)+"\n";
  info+="Pin DHT    ="+String(pin_dht)+"\n";
  info+="Pin DS     ="+String(pin_ds)+"\n";
  info+="Pin LED    ="+String(pin_led_status)+"\n";
  
  info+="Activations:\n";
  
  info+="wireless_active = "+String(wireless_active)+"\n"; // use wireless
  info+="wificonfig      = "+String(wificonfig)+"\n"; // enable configuration via wifi (AP mode)
  info+="send2dusti      = "+String(send2dusti)+"\n"; // send data to api.luftdaten.info API
  info+="send2madavi     = "+String(send2madavi)+"\n"; // TODO: implement madavi
  info+="send2mqtt       = "+String(send2mqtt)+"\n"; // send data to MQTT
  info+="send2csv        = "+String(send2csv)+"\n"; // output as csv over serial
  info+="debug           = "+String(debug)+"\n"; // debug outputs over serial

  info+="SSID: "+ssid+"\n"; // do not output password
  info+="Django API: "+djangoapihost+":"+String(djangoapihttpPort)+"\n";
  info+="MQTT API: "mqttusername+"@"++mqttserver+":"+mqttport;
  info+="\n\ttopicbase:"+mqtttopicbase;
  info+="\n\tconfigtopic:"+mqttintopic;
  
  info+="\nMADAVI: "+madaviapihost+":"+madaviapihttpPort+madaviapiurl;

  return(info);
}
