#include <WiFiManager.h>

/*****************************************************************
/* WifiConfig                                                    *
/*****************************************************************/
void wifiConfig(debug=false) {
  String boolvar;
  String apname;
  String custom_wlanssid;
  String custom_wlanpwd;
  WiFiManager wifiManager;

  if (! debug) wifiManager.setDebugOutput(false);
  boolvar = (dht_read) ? '1' : '0';
  WiFiManagerParameter custom_dht_read("dht_read", "DHT Sensor (0/1) ?", "", 10);
  wifiManager.addParameter(&custom_dht_read);
  boolvar = (ppd_read) ? '1' : '0';
  WiFiManagerParameter custom_ppd_read("ppd_read", "PPD42NS Sensor (0/1) ?", "", 10);
  wifiManager.addParameter(&custom_ppd_read);
  boolvar = (sds_read) ? '1' : '0';
  WiFiManagerParameter custom_sds_read("sds_read", "SDS Sensor (0/1) ?", "", 10);
  wifiManager.addParameter(&custom_sds_read);
  boolvar = (send2dusti) ? '1' : '0';
  WiFiManagerParameter custom_send2dusti("send2dusti", "Senden an luftdaten.info (0/1) ?", "", 10);
  wifiManager.addParameter(&custom_send2dusti);
  boolvar = (send2madavi) ? '1' : '0';
  WiFiManagerParameter custom_send2madavi("send2madavi", "Senden an madavi.de (0/1) ?", "", 10);
  wifiManager.addParameter(&custom_send2madavi);
  boolvar = (send2csv) ? '1' : '0';
  WiFiManagerParameter custom_send2csv("send2csv", "Seriell als CSV (0/1) ?", "", 10);
  wifiManager.addParameter(&custom_send2csv);
  boolvar = (debug) ? '1' : '0';
  WiFiManagerParameter custom_debug("debug", "Debug output (0/1) ?", "", 10);
  wifiManager.addParameter(&custom_debug);
  apname  = "Feinstaubsensor-";
  apname += ESP.getChipId();
  wifiManager.setTimeout(300);
  wifiManager.setBreakAfterConfig(true);
  wifiManager.startConfigPortal(apname.c_str());
  custom_wlanssid = WiFi.SSID();
  custom_wlanpwd = WiFi.psk();
  strcpy(wlanssid,custom_wlanssid.c_str());
  strcpy(wlanpwd,custom_wlanpwd.c_str());
  if (strcmp(custom_dht_read.getValue(),"") != 0) dht_read = strtol(custom_dht_read.getValue(), NULL, 10);
  if (strcmp(custom_ppd_read.getValue(),"") != 0) ppd_read = strtol(custom_ppd_read.getValue(), NULL, 10);
  if (strcmp(custom_sds_read.getValue(),"") != 0) sds_read = strtol(custom_sds_read.getValue(), NULL, 10);
  if (strcmp(custom_send2dusti.getValue(),"") != 0) send2dusti = strtol(custom_send2dusti.getValue(), NULL, 10);
  if (strcmp(custom_send2madavi.getValue(),"") != 0) send2madavi = strtol(custom_send2madavi.getValue(), NULL, 10);
  if (strcmp(custom_send2csv.getValue(),"") != 0) send2csv = strtol(custom_send2csv.getValue(), NULL, 10);
  if (strcmp(custom_debug.getValue(),"") != 0) debug = strtol(custom_debug.getValue(), NULL, 10);
  if (debug) {
    Serial.println("------ Result from Webconfig ------");
    Serial.print("WLANSSID: ");Serial.println(wlanssid);
    Serial.print("DHT_read: ");Serial.print(custom_dht_read.getValue());Serial.print(" - ");Serial.println(dht_read);
    Serial.print("PPD_read: ");Serial.print(custom_ppd_read.getValue());Serial.print(" - ");Serial.println(ppd_read);
    Serial.print("SDS_read: ");Serial.print(custom_sds_read.getValue());Serial.print(" - ");Serial.println(sds_read);
    Serial.print("Dusti   : ");Serial.print(custom_send2dusti.getValue());Serial.print(" - ");Serial.println(send2dusti);
    Serial.print("Madavi  : ");Serial.print(custom_send2madavi.getValue());Serial.print(" - ");Serial.println(send2madavi);
    Serial.print("CSV     : ");Serial.print(custom_send2csv.getValue());Serial.print(" - ");Serial.println(send2csv);
    Serial.print("Debug   : ");Serial.print(custom_debug.getValue());Serial.print(" - ");Serial.println(debug);
    Serial.println("-----------------------------------");
  }
}
