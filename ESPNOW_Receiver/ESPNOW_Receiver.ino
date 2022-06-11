#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define SIGNAL_TIMEOUT 1000  // This is signal timeout in milli seconds. We will reset the data if no signal

unsigned long lastRecvTime = 0;

struct PacketData
{
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
 
  byte switch1Value;
  byte switch2Value;
  byte switch3Value;
  byte switch4Value;  
  byte switch5Value;
  byte switch6Value;
  byte switch7Value;
  byte switch8Value;  
  
};
PacketData receiverData;

Servo servo1;     
Servo servo2;     
Servo servo3;     
Servo servo4;     

int   led1 = 15;
int   led2 = 16;
int   led3 = 17;
int   led4 = 18;
int   led5 = 19;
int   led6 = 21;
int   led7 = 22;
int   led8 = 23;

//Assign default input received values
void setInputDefaultValues()
{
  // The middle position for joystick. (254/2=127)
  receiverData.lxAxisValue = 127;
  receiverData.lyAxisValue = 127;
  receiverData.rxAxisValue = 127;
  receiverData.ryAxisValue = 127;
  
  receiverData.switch1Value = LOW;
  receiverData.switch2Value = LOW;
  receiverData.switch3Value = LOW;
  receiverData.switch4Value = LOW; 

  receiverData.switch5Value = LOW;
  receiverData.switch6Value = LOW;
  receiverData.switch7Value = LOW;
  receiverData.switch8Value = LOW;   
}

void mapAndWriteValues()
{
  servo1.write(map(receiverData.lxAxisValue, 0, 254, 0, 180));
  servo2.write(map(receiverData.lyAxisValue, 0, 254, 0, 180));
  servo3.write(map(receiverData.rxAxisValue, 0, 254, 0, 180));
  servo4.write(map(receiverData.ryAxisValue, 0, 254, 0, 180));
  
  digitalWrite(led1, receiverData.switch1Value);
  digitalWrite(led2, receiverData.switch2Value);
  digitalWrite(led3, receiverData.switch3Value);
  digitalWrite(led4, receiverData.switch4Value); 

  digitalWrite(led5, receiverData.switch5Value);
  digitalWrite(led6, receiverData.switch6Value);
  digitalWrite(led7, receiverData.switch7Value);
  digitalWrite(led8, receiverData.switch8Value);   
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  if (len == 0)
  {
    return;
  }
  memcpy(&receiverData, incomingData, sizeof(receiverData));
  mapAndWriteValues();  
  lastRecvTime = millis(); 
}

void setUpPinModes()
{
  servo1.attach(27);
  servo2.attach(26);
  servo3.attach(25);
  servo4.attach(33);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);      

  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT); 
  
  setInputDefaultValues();
  mapAndWriteValues();
}

void setup() 
{
  setUpPinModes();
 
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
 


void loop()
{
  //Check Signal lost.
  unsigned long now = millis();
  if ( now - lastRecvTime > SIGNAL_TIMEOUT ) 
  {
    setInputDefaultValues();
    mapAndWriteValues();  
  }
}
