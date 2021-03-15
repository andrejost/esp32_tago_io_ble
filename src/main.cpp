#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <WiFi.h>
#include <Credentials.h>

// WiFi connection configuration
const char* ssid = WIFI_SSID; //rede wifi
const char* password = WIFI_PW; //senha wifi

// Tagio configuration
String oauth_token = OAUTH_TOKEN; 
const char* host = "api.tago.io";

//Use port 80 only for testing purposes. If this is a production Scenario, prefer HTTPS using 443
const int httpsPort = 80; 

// This is to ensure that the bluetooh is enabled on startup
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Token to control if device is connected to the BLE Service
bool deviceConnected = false; 

//Creation of a Client for the Wifi and sending HTTP data
WiFiClient client;

//Function Responsible for sending the information to the Tago.io Platform
void httpSend(String topico, String valor) { 
  
  // JSON Formatter to send the payload
  String post_payload = "{\"variable\":\"" + topico + "\",\"value\":" + valor + "}";

   Serial.print("connecting to ");
   Serial.println(host);

   //Sending data to the Tago.io API
   if (!client.connect(host, httpsPort)) {
     Serial.println("connection failed");
     return;
   }
  
   //Reult of the data sent and the response received.
   client.println("POST /data? HTTP/1.1");
   client.print("Host: ");
   client.println(host);
   // For production purposes, this line should be true instead of false
   client.println("_ssl: false");
   client.print("Device-Token: ");
   client.println(oauth_token);
   client.println("Content-Type: application/json");
   client.print("Content-Length: ");
   client.println(post_payload.length());
   client.println();
   client.println(post_payload);              
   Serial.println("request sent");
   Serial.println("reply was:");
   Serial.println("=========="); 

  //Here it is shown the result of the data sent to the Tago.io API
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line); // mostra o resultado da carga no tago.io

    //This is to ensure that it leaves the loop of the while once all the response was received.
    if (line.startsWith("{")){
      break;
    }
    delay(50);
  }

      client.stop();
      Serial.println("==========");
      Serial.println("closing connection"); 

 }

//Callback for the BLE Carachteristic when it receives the data from the BLE Application in the connected device.
class CharacteristicCallbacks: public BLECharacteristicCallbacks {
     void onWrite(BLECharacteristic *pCharacteristic) {
          
          //Returns the pointer to the register containing the current of the carachteristic and passes to the rxValue variable
          std::string rxValue = pCharacteristic->getValue(); 
          
          //Check if the data is present (value > 0)
          if (rxValue.length() > 0) {
 
               for (int i = 0; i < rxValue.length(); i++) {
                  Serial.print(rxValue[i]);
               }

               Serial.println(" ");

            // Send a value 1 (Meaning Yellow) to the Tago.io API
                if (rxValue.find("1") != -1) {
                  String Sinal = "1"; 
                    httpSend("semaforo" , Sinal );
                    
                }
            // Send a value 2 (Meaning Green) to the Tago.io API
                if (rxValue.find("2") != -1) { 
                  String Sinal = "2"; 
                    httpSend("semaforo" , Sinal );
                    
                }
            // Send a value 3 (Meaning Red) to the Tago.io API
                if (rxValue.find("3") != -1) { 
                  String Sinal = "3";
                    httpSend("semaforo" , Sinal );
                    
                }
                
          }
     }//onWrite
};

//callback to identify when a device is connected or not.
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {

  Serial.begin(115200);

  //Conexão Wifi
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  
  //WI-FI Startup
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //BLE Startup
  Serial.println("Bluetooth Will now start so you cann connect to your cell phone");
  BLEDevice::init("ESP32-Tagio");
  BLEServer *pServer = BLEDevice::createServer();

  //Creates the Callbacks for the Server
  pServer->setCallbacks(new ServerCallbacks()); 

  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Creates a carachteristic for the bluetooth to receive data.
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  //Creates a callback for the created BLE Carachteristic
  pCharacteristic->setCallbacks(new CharacteristicCallbacks());             
    
  // Start the service
  pService->start();
 
  // Start advertising (ESP32 no can be discovered)
  pServer->getAdvertising()->start();
  
  Serial.println("Characteristic defined! Now you can read it in your phone!");

}

void loop() {

//For this example no code in the Main Loop is necessary, since all the data is being handeled in the callbacks of the BLE function BLECharacteristicCallbacks

}

