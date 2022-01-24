#include <MQ135.h>
#include <MQ2.h>
#include "DHT.h"
#define ANALOGPIN A1
#define dhtPin 8
#define dhtType DHT11 //選用DHT11   

DHT dht(dhtPin, dhtType);

int PPMStatus = 0;
int PPMStatusOld = 0;
int pin = A2;
int lpg, co, smoke;

float  airTemperature, airHumidity, ppm, ppmbalanced, rzero;
int initStep=1;                 // 1 = Connection in progress / 2 = Connection Done 

MQ135 gasSensor = MQ135(ANALOGPIN);
MQ2 mq2(pin);

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  mq2.begin();
}

void loop() 
{
  //UVM-30A紫外線感測器
  int sensorValue;
  sensorValue=analogRead(A0);
  Serial.print("The voltage value:");
  Serial.print(sensorValue*(5/1023.0)*1000);
  Serial.println("mV");
  //delay(1000);
  //Serial.print("\n");
  
  //mq2感測器
  float* values= mq2.read(true); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
  //DHT11感測器
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
  //MQ135感測器
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
