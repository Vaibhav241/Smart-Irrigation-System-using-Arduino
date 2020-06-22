#include <SoftwareSerial.h>
#include <Wire.h>
#include <SimpleDHT.h>
SoftwareSerial SIM900(8, 7);//(Rx,Tx)
  
int pinDHT11 = 7;
int sensor_pin = A0;
int output_value,output;
int ldr=0;
SimpleDHT11 dht11(pinDHT11);

void setup() {
  Wire.begin();
  SIM900.begin(9600);
  pinMode(A1,INPUT);
  Serial.begin(9600);
  pinMode(3,OUTPUT);
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
int output1 = map(output,1023,30,0,100);
  Serial.println("Moisture: "+String(output1)+"%");
  Serial.print("Temparture: " + String(temp)); Serial.print(" *C, "); 
  Serial.print("Humidity: " + String(hum)); Serial.println(" H");
  Serial.println("LDR: "+ String(ldr));

  //Mail program
  Serial.println("HTTP post method :");
  Serial.print("AT\\r\\n");
  SIM900.println("AT"); /* Check Communication */
  delay(5000);
  ShowSerialData(); /* Print response on the serial monitor */
  delay(5000);
  /* Configure bearer profile 1 */
  Serial.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\\r\\n");
  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  /* Connection type GPRS */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"\\r\\n");
  SIM900.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");  /* APN of the provider */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=1,1\\r\\n");
  SIM900.println("AT+SAPBR=1,1"); /* Open GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=2,1\\r\\n");
  SIM900.println("AT+SAPBR=2,1"); /* Query the GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPINIT\\r\\n");
  SIM900.println("AT+HTTPINIT");  /* Initialize HTTP service */
  delay(5000); 
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPPARA=\"CID\",1\\r\\n");
  SIM900.println("AT+HTTPPARA=\"CID\",1");  /* Set parameters for HTTP session */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPPARA=\"URL\",\"ptsv2.com/t/6ial6-1560492120/post\"\\r\\n");
  SIM900.println("AT+HTTPPARA=\"URL\",\"ptsv2.com/t/6ial6-1560492120/post\"");  /* Set parameters for HTTP session */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPDATA=120,10000\\r\\n");
  SIM900.println("AT+HTTPDATA=120,10000"); /* POST data of size 33 Bytes with maximum latency time of 10seconds for inputting the data*/ 
  delay(2000);
  ShowSerialData();
  delay(2000);
  
  String data= "Temperature= " + String(temp) + "& " + "Humidity= " + String(hum) + "& " + "Soil Moisture= " + String(output1)+ " LDR= " +String(ldr);
  Serial.print(data);  /* Data to be sent */
  SIM900.println(data);
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPACTION=1\\r\\n");
  SIM900.println("AT+HTTPACTION=1");  /* Start POST session */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+HTTPTERM\\r\\n");  
  SIM900.println("AT+HTTPTERM");  /* Terminate HTTP service */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.print("AT+SAPBR=0,1\\r\\n");
  SIM900.println("AT+SAPBR=0,1"); /* Close GPRS context */
  delay(5000);
  ShowSerialData();
  delay(5000);
  Serial.println("Done");
}

void ShowSerialData()
{
  while(SIM900.available()!=0)  /* If data is available on serial port */
  Serial.write(char (SIM900.read())); /* Print character received on to the serial monitor */
}

void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char) Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}
