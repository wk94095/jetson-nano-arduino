void setup(){ 
  Serial.begin(9600);
}

void loop()
{  
  int sensorValue;

  sensorValue=analogRead(A0);
  Serial.print("The voltage value:");
  Serial.print(sensorValue*(5/1023.0)*1000);
  Serial.println("mV");
  delay(1000);
  Serial.print("\n");
}
