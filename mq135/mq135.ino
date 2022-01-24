#include <MQ135.h>
#include "DHT.h"
#define ANALOGPIN A1
#define dhtPin 8
#define dhtType DHT11 //選用DHT11   

DHT dht(dhtPin, dhtType);

int PPMStatus = 0;
int PPMStatusOld = 0;

float  airTemperature, airHumidity, ppm, ppmbalanced, rzero;
int initStep=1;                 // 1 = Connection in progress / 2 = Connection Done 

MQ135 gasSensor = MQ135(ANALOGPIN);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {

float h = dht.readHumidity();//讀取濕度
  float t = dht.readTemperature();//讀取攝氏溫度
  float f = dht.readTemperature(true);//讀取華氏溫度

  // 當時室內大約的溫度跟濕度，讓 getCorrectPPM進行校正
  airTemperature = dht.readTemperature();
  airHumidity = dht.readHumidity();

 if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("無法從DHT傳感器讀取！");
    return;
  }
  
  Serial.print("濕度: ");
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("攝氏溫度: ");
  Serial.print(t);
  Serial.print("*C\t");
  Serial.print("華氏溫度: ");
  Serial.print(f);
  Serial.print("*F\n");

  rzero = gasSensor.getRZero(); //取得rzero 校準值
  Serial.print("RZero=");
  Serial.println(rzero);    // 持續顯示 rzero 值
   
  ppm = gasSensor.getPPM(); // 取得 ppm 值
  Serial.print("PPM=");
  Serial.println(ppm); 
  
  ppmbalanced = gasSensor.getCorrectedPPM(airTemperature, airHumidity);  // 取得修正的 ppm 值
  Serial.print("PPM Corrected=");
  Serial.println(ppmbalanced); 
  Serial.println();

  delay(5000);
  }
