#include <ADXL345.h>
#include <Ethernet.h>
#include <FreqMeasure.h>
#include <SPI.h>
#include <Wire.h>

// Ethernet Config
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte gateway[] = {10,21,69,4};
byte subnet[] = {255,255,255,0};
IPAddress ip(10,21,69,7);
EthernetServer server(1180);

// ADXL345 Variables
const float alpha = 0.5;
double fXg = 0;
double fYg = 0;
double fZg = 0;
double pitch, roll;
ADXL345 Acc;

// Shooter
int RPM = 0;
double sum = 0;
int count = 0;
unsigned long time = 0;

// Communications
char dataStream[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setup() {
  // Configure default Serial COM to communicate at 57600 baud
  Serial.begin(57600);

  // Initialize Ethernet Server (TCP)
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  server.begin();
  Serial.print("Webserver is at ");
  Serial.println(Ethernet.localIP());

  // Join I2C bus
  Wire.begin();
  Serial.println("Initializing I2C");

  // Initialize the accelerometer
  Acc.begin();
  
  // Initialize the Hall Effect sensor
  hallInit();
  
}
void loop() {
  // Read from the ADXL345 and convert force vectors into pitch and roll
  anglesCalc();
  
  // Measure frequency from Hall Effect Sensor
  measureFreq();
  
  // We still need to confirm that we will be using pitch and not roll
  sprintf(dataStream, "%+.2i.%.2u:%.4i\r\n", int(pitch),int(abs(pitch - int(pitch))*100),RPM);

  /* Remove prior to competition. For debugging ONLY */
  Serial.println(dataStream);
  Serial.print(millis()-time);
  Serial.print(" ms");
  // Listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    String clientMsg ="";
    if (client.connected()) {
      /* Remove prior to competition. For debugging ONLY
         Serial.println("Someone is connected"); */
      if (client.available()) {
        client.print(dataStream);
        Serial.println(dataStream);
        
        /* Remove prior to competition. For debugging ONLY
           Serial.println("Printing"); */    
      }
    }
    // Give the Client time to receive the data
    delay(50);
    // Close the connection:
    client.stop();
  }
  delay(100);
}

void anglesCalc() {
	double Xg, Yg, Zg;
	Acc.read(&Xg, &Yg, &Zg);

	//Low Pass Filter
	fXg = Xg * alpha + (fXg * (1.0 - alpha));
	fYg = Yg * alpha + (fYg * (1.0 - alpha));
	fZg = Zg * alpha + (fZg * (1.0 - alpha));

	//Roll & Pitch Equations
	roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
	pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;
}
void measureFreq() {
  /* Remove prior to competition. For debugging ONLY
     Serial.println(digitalRead(49)); */
  if (FreqMeasure.available()) {
    
    // Average several reading together
    sum = sum + FreqMeasure.read();
    count = count + 1;
    if (count > 15) {
      double frequency = F_CPU / (sum / count);
      RPM = frequency * 60;
      time = millis();
      /* Remove prior to competition. For debugging ONLY
         Serial.println(RPM); */
      sum = 0;
      count = 0;
    } 
  }
  unsigned long timeElapsed = millis() - time;
  if (timeElapsed > 1000){
      RPM = 0;
  }
}

void hallInit() {
  // Turn on internal PULL-UP resistor on pin 49
  digitalWrite(49,HIGH);
  
  // Start Frequency Monitor
  FreqMeasure.begin();
}

