#include <SimpleDHT.h>

int pinDHT11 = 7;
int sensor_pin = A0;
int output_value,output;
int ldr=0;
int light=3;
int fan=9;
int motor=2;
SimpleDHT11 dht11(pinDHT11); 

void setup() {
  pinMode(A1,INPUT);
  Serial.begin(9600);
  pinMode(motor,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(light,OUTPUT);
  pinMode(fan,OUTPUT);
  //digitalWrite(light,HIGH);
}

void loop() {
  
  byte temperature = 0;
  byte humidity = 0;
  int temp=0,hum=0;
  int sum=0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  for(int i=0;i<=10;i++){
    output_value= analogRead(sensor_pin);
    sum+=output_value;
    temp+=int(temperature);
    hum+=int(humidity);
    delay(1000);
  }
  temp=temp/10;
  hum=hum/10;
  output=sum/10;
  ldr=analogRead(A1);
int output1 = map(output,1123,30,0,100);
  //Serial.println("output= " + String(output));
  Serial.println("Moisture: "+String(output1)+"%");
  Serial.print("Temparture: " + String(temp)); Serial.print(" *C, "); 
  Serial.print("Humidity: " + String(hum)); Serial.println(" H");
  Serial.println("LDR: "+ String(ldr));
  if(ldr<876){
   digitalWrite(light,HIGH);
    Serial.println("Lights On \nIdeal ldr reading is more than 876");
  }
  else{
    digitalWrite(light,LOW);
    Serial.println("Lights Off \nIdeal ldr reading is more than 876");
  }
  if(temp>=27){
    Serial.println("Fan On \nIdeal temperature is 20-27 C");
    digitalWrite(fan,HIGH);
  }
  else if(temp<20){
    Serial.println("Fan off \nIdeal temperature is 20-27 C");
    digitalWrite(fan,LOW);
  }
  if(output1<=20){
    digitalWrite(motor,HIGH);
    Serial.println("Motor On \nIdeal moisture is 20-25 %");
  }
  else if(output1>25){
    digitalWrite(motor,LOW);
    Serial.println("Motor Off \nIdeal moisture is 20-25 %");
  }
  else{
    digitalWrite(motor,LOW);
    Serial.println("Motor Off \nIdeal moisture is 20-25 %");
  }
}
