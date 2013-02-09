#include <Wire.h>
#include <ADXL345.h>
#include <SPI.h>
#include <Ethernet.h>

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
ADXL345 acc;

// Shooter
int rpm = 0;

// Communications
char dataStream[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setup(){
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
  acc.begin();
}
void loop(){
	anglesCalc();
  
  // We still need to confirm that we will be using pitch and not roll.
  sprintf(dataStream, "%+.2i.%.2u:%.4i\r\n", int(pitch),int(abs(pitch - int(pitch))*100),rpm);

  // Remove prior to competition. For debugging ONLY
	Serial.println(dataStream);
	
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    String clientMsg ="";

    if (client.connected()) {
      //Serial.println("Someone is connected");

      if (client.available()) {
        client.print(dataStream);
        
        // Remove prior to competition. For debugging ONLY
        Serial.println("Printing");
      }      
    }
    // Give the Client time to receive the data
    delay(50);
    // Close the connection:
    client.stop();
  }
}

void anglesCalc(){
	double Xg, Yg, Zg;
	acc.read(&Xg, &Yg, &Zg);

	//Low Pass Filter
	fXg = Xg * alpha + (fXg * (1.0 - alpha));
	fYg = Yg * alpha + (fYg * (1.0 - alpha));
	fZg = Zg * alpha + (fZg * (1.0 - alpha));

	//Roll & Pitch Equations
	roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
	pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;
}

void sPrintAngles(){
	Serial.print(dataStream);
}


