#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "winnone";
const char* password = "winnone12";

const char* urlroom1 = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=11";
//const char* urlroom2 = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=11";
//const char* urlroom3 = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=11";

const int sizee = 2*JSON_OBJECT_SIZE(4);

int sw=22;
int ldr1=34,ldr2=33,ldr3=32;
int led1=5,led2=18,led3=21;
int used1=0,used2=0,used3=0;
char str[50];

StaticJsonDocument<sizee> JSONPost;

void wificonnect(){
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to the WiFi network");
}

void _post (const char* url,int led1){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type","application/json");
    JSONPost["ldr"]=0;
    JSONPost["led1"]=led1;
    JSONPost["led2"]=0;
    serializeJson(JSONPost,str);
    int httpCode = http.POST(str);
    
    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.print("POST ");
      Serial.println(httpCode);
      Serial.println(payload);
    }
   else
      wificonnect();
 }
  delay(100);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  wificonnect();

  pinMode(sw,INPUT_PULLUP);
  
  pinMode(led1,OUTPUT);
  digitalWrite(led1,HIGH);
  
  pinMode(led2,OUTPUT);
  digitalWrite(led2,LOW);
  
  pinMode(led3,OUTPUT);
  digitalWrite(led3,LOW);

  pinMode(ldr1,INPUT);
  pinMode(ldr2,INPUT);
  pinMode(ldr3,INPUT);

  xTaskCreatePinnedToCore(Task2,"Task 2",1024*32,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(Task3,"Task 3",1024*32,NULL,1,NULL,0);
}

void loop() {
  if(analogRead(ldr1)>1000){
    if(used1==0){
      digitalWrite(led1,LOW);
      _post(urlroom1,1);
      used1=1;
    }
  }
  else{
    if(used1==1){
        digitalWrite(led1,HIGH);
        _post(urlroom1,0);
        used1=0;
    }
  }  
  vTaskDelay(30/portTICK_PERIOD_MS);
}

void Task2 (void *parameter){
  while(1){
   /*if(analogRead(ldr2)>1000){
    _post(urlroom1,1);
    while(1){
      digitalWrite(led2,HIGH);
      if(analogRead(ldr2)<1000){
        digitalWrite(led2,LOW);
        _post(urlroom1,0);
        break;
      }
    }
    }
    vTaskDelay(30/portTICK_PERIOD_MS);
  }*/

   if(analogRead(ldr2)>1000){
    if(used2==0){
      digitalWrite(led2,HIGH);
      //_post(urlroom2,1);
      used2=1;
    }
  }

  else{
    if(used2==1){
        digitalWrite(led2,LOW);
       // _post(urlroom2,0);
        used2=0;
    }
  }  
  vTaskDelay(30/portTICK_PERIOD_MS);
  }
}

void Task3 (void *parameter){
  while(1){
    if(analogRead(ldr3)>1000){
      if(used3==0){
        digitalWrite(led3,HIGH);
        //_post(urlroom3,1);
        used3=1;
      }
    }

    else{
      if(used3==1){
        digitalWrite(led3,LOW);
        // _post(urlroom2,0);
        used3=0;
      }
    }  
    vTaskDelay(30/portTICK_PERIOD_MS);
 }
}
