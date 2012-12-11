/*
  Software serial multple serial test
 
 Receives from the hardware serial, sends to software serial and ethernet.
 Receives from software serial, sends to hardware serial and ethernet.
 
 The circuit: 
 * RX is digital pin 5 (connect to TX of other device)
 * TX is digital pin 6 (connect to RX of other device)

 SHIFTER TX to PIN 5
 SHIFER RX to PIN 6

*/

#include <SoftwareSerial.h>
#include <SPI.h>
#include <Ethernet.h>

SoftwareSerial mySerial(5, 6); // RX, TX

byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x0D, 0x44, 0x85 };
IPAddress ip(192,168,1,177);
byte gateway[] = { 
  150,101,0,4 }; // internet access via router
byte subnet[] = { 
  255, 255, 0, 0 }; //subnet mask

EthernetServer server(1235);

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  //Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
}

void loop() // run over and over
{
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        mySerial.write(c);
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        
      }
      if (mySerial.available()){
        char c = mySerial.read();
        if (Serial.read() == '\n') {
          client.print("\n");
        }
        client.print(c);
        Serial.write(c);
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

