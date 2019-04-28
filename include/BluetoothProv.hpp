#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>

BluetoothSerial SerialBT;

void onDataReceive(esp_spp_cb_param_t *param) {
  Serial.print("BLT recvd Data: ");          
  int i;
  char jsonData[param->data_ind.len]; 
  for(i = 0; i < param->data_ind.len; i++) {
    Serial.write(param->data_ind.data[i]);
    jsonData[i] = param->data_ind.data[i];
  }
  Serial.print("\n");
  DynamicJsonDocument doc(1024);
  DeserializationError er = deserializeJson(doc, jsonData);
  if(er == STATUS::OK) {
      const char *SSID = doc["SSID"];
      const char *PWD = doc["PWD"];
      WiFi.begin(SSID,PWD);
      int counter = 20;
      Serial.print("Connecting.");
      while(WiFi.status() != WL_CONNECTED && counter) {
        Serial.print(".");
        counter--;
        delay(1000);
      } 
      if(WiFi.status() != WL_CONNECTED) {
        Serial.println("Failed to connect");
        SerialBT.println("Failed to Connect!!");
      }
       else {
         Serial.println("Connected");
         SerialBT.println("Connected!!");
       }
   } else Serial.println("Not a json object");
}
void bluethoothCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  switch(event) {
    case ESP_SPP_INIT_EVT: Serial.println("BLT initiated");
                            break;
    case ESP_SPP_START_EVT: Serial.println("BLT is now ready for being paired & connected");
                          break;
    case ESP_SPP_SRV_OPEN_EVT: Serial.println("some BLT device Connected");
                          break;
    case ESP_SPP_CLOSE_EVT: Serial.println("BLT connection closed");
                             break;
    case ESP_SPP_DATA_IND_EVT: onDataReceive(param); 
                              break;
    case ESP_SPP_WRITE_EVT: if(param->write.status == ESP_SPP_SUCCESS)
                                Serial.println("BLT write success!");
                              else Serial.println("BLT write failed!");
                              break;
    default: Serial.printf("Unhandled Bluetooth event: %d", event);
  }
}
esp_spp_cb_t *(callback) = & bluethoothCallback;


static void initBLProvision() {
  SerialBT.begin("ESP32test"); //Bluetooth device name
  SerialBT.register_callback(callback);
}