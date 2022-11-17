//Khai bao thu vien
#include <DHT.h>
#include <DHT_U.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

#define led2 14 //D5
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht (DHTPIN,DHTTYPE);
//Khi bao bien làm việc
BlynkTimer timer;
WidgetLED led1(V0);

// ket noi wifi and blynk
char auth[] = "i8NSNAfCkkBNk2buGG3UkxnKP6qFkJlH";
char ssid[] = "home";
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
  
  pinMode(led2, OUTPUT);
  
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
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.println(t);
  Serial.println(h);
  Blynk.virtualWrite(V1,t);
  Blynk.virtualWrite(V2,h);
}
BLYNK_WRITE(V5){
  int button = param.asInt();
  if(button == 1){
    digitalWrite(led2, HIGH); 
  }else{
    digitalWrite(led2, LOW);
  }
  
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
    
    if( now_time < 30000)
    {
      if (giatri == 1)
      {
        count ++;
        if(count > 10)
        {
          Serial.println("em be khoc"); 
          Blynk.virtualWrite(V3,"em be khoc");   
        }
      }
    }else{
      now_time = 0;
      count = 0;
      Blynk.virtualWrite(V3,"em be chua khoc");
    }
  }

  if(giatri_vt == 1){
    Blynk.virtualWrite(V4,"em be da roi vi tri");
    
  }else{
    Blynk.virtualWrite(V4,"Em be con o do");
  }
  Serial.print("Gia tri cam bien am thanh la:");
  Serial.println(giatri);
  Serial.print("Gia tri cam bien vi tri la:");
  Serial.println(giatri_vt);
  Serial.print("Count = :");
  Serial.println(count);
  Serial.println(now_time);
  delay(500);
}
