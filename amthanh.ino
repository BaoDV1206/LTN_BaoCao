//Khai bao thu vien
#include <BlynkSimpleEsp8266.h>
#include <DHTesp.h>
#include <ESP8266WiFi.h>

//Khi bao bien làm việc
BlynkTimer timer;
WidgetLED led1(V0);
DHTesp dht;

// ket noi wifi and blynk
char auth[] = "4sP8zB4-fxCi9Q9eScs-m1ynNimgkPlw";
char ssid[] = "Home";
char pass[] = "12345678";


// bien linh kien
int cambien = 5;
int cambienv = 4;
int giatri;
int giatri_vt;
//bien xu ly thoi gian
int count = 0;
int now_time;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conected");
  Serial.print("IP host: ");
  Serial.println(WiFi.localIP());
  
  pinMode(cambien, INPUT);
  pinMode(cambienv, INPUT);
  now_time = 0;
  
  if(WiFi.isConnected()){
    Serial.println("Wifi is connected!");
    Blynk.config(auth,"blynk.cloud", 80);
    Blynk.connect();
  }  
  timer.setInterval(1000L, blinkLedWidget);
}

void loop() {
  if(WiFi.isConnected()){
    Blynk.run();
  }
  timer.run();
  
}
void blinkLedWidget()
{
  if(Blynk.connected()){
    if (led1.getValue()) {
      led1.off();
    } else {
      led1.on();
    }
    //code
    giatri = digitalRead(cambien);
    giatri_vt = digitalRead(cambienv);
    now_time += 500;
    
    if( now_time < 40000)
    {
      if (giatri == 1)
      {
        count ++;
        if(count > 10)
        {
          Serial.println("em be khoc!"); 
          Blynk.virtualWrite(V3,"em be khoc!");   
        }
      }
    }else{
      now_time = 0;
      count = 0;
      Blynk.virtualWrite(V3,"em be chua khoc!");
    }
  }

  if(giatri_vt == 1){
    Blynk.virtualWrite(V4,"404 not found!");
  }else{
    Blynk.virtualWrite(V4,"Em be con o do!");
  }
  Blynk.virtualWrite(V1,30);
  Blynk.virtualWrite(V2,80);
  
  Serial.print("Gia tri cam bien am thanh la:");
  Serial.println(giatri);
  Serial.print("Gia tri cam bien vi tri la:");
  Serial.println(giatri_vt);
  Serial.print("Count = :");
  Serial.println(count);
  Serial.println(now_time);
  delay(500);
}
