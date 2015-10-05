/*
 Created by Rui Santos
 Visit: http://randomnerdtutorials.com for more arduino projects
 Modified by Nicu Florica aka niq_ro
 visit http://nicuflorica.blogspot.ro/
 or http://arduinotehniq.blogspot.com
 or http://www.tehnic.go.ro

 Arduino with Ethernet Shield
 */

#include <SPI.h>
#include <Ethernet.h>
int led = 9;
int pos = 0; 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 0, 20 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 0, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(8081);                             //server port     
String readString;

int clic; // number of mouse clicks
int clic1; // number of recovered mouse clicks
int stare; // 0 is OFF, 1 is ON
int stare0; // before state
int a = 0;  // decinal value
int b = 0;  // unitar value
int a1 = 0;  // recovered decimal value
int b1 = 0;  // recovered unitar value

float t,h;
float teset, dete;
int teset0, teset1, teset2, teset3;
//int teset = 225;  // temperaturee set x 10;
//int dete = 5;   // hysteresys temperature x 10;

byte led0; 

// http://tronixstuff.com/2011/03/16/tutorial-your-arduinos-inbuilt-eeprom/
#include <EEPROM.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#define DHTPIN A2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
   
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

a1 = EEPROM.read(100);
b1 = EEPROM.read(101);
// recover number of mouse click
clic = a1*256 + b1;

stare0 = EEPROM.read(102);  
stare = EEPROM.read(102);

/*
// initial value for thermostat
EEPROM.write(100,0);
EEPROM.write(101,0);
EEPROM.write(150,205);
EEPROM.write(149,0);
EEPROM.write(151,5);
*/

teset1 = EEPROM.read(150);
teset0 = EEPROM.read(149);
teset = 256*teset0+teset1;

dete = EEPROM.read(151);



  dht.begin();
  
}


void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
     
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           // add a meta refresh tag, so the browser pulls again every 10 seconds:
//          client.println("<meta http-equiv=\"refresh\" content=\"10\">");
           client.println("<meta http-equiv=\"refresh\" content=\"5\" URL='http://192.168.0.20:8081/'\" >");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
//           client.println("<link rel='stylesheet' type='text/css' href='https://github.com/tehniq3/webcontrol/blob/master/ethernetcss.css' />");
           client.println("<TITLE>Arduino thermostat with Ethernet Shield</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<body style=background:#F0FFFF>");
           client.println("<H1>Arduino Thermostat with Ethernet Shield");
    //       client.println("</H2>");
       
 // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  t = dht.readTemperature();

       client.println("<H2>");
client.print("<p>Actual: ");
client.print("temperature = ");
client.print(t,1);
client.print("<sup>o</sup>");
client.print("C & ");
client.print("humidity = ");
client.print(h,1);
client.print(" %RH<p>");

           client.println("<font color=white>");
           client.println("<a href=\"/tesetplus\"\">tset +</a>");
           client.println("<a href=\"/deteplus\"\">dt +</a><br />");
           client.println("<br />");     
           client.println("<a href=\"/tesetminus\"\">tset -</a>");   
           client.println("<a href=\"/deteminus\"\">dt -</a><br />");   
           client.println("</font>");
   //        client.println("<br />");     


 if (readString.indexOf("tesetplus") >0){
          teset = teset + 5;        
          if (teset > 500) teset = 500;
         
          // calculate the unitar and zecinal units
teset0 = teset/256;
teset1 = teset - teset0*256;
// write in internal eeprom
EEPROM.write(150,teset1);
EEPROM.write(149,teset0);
          client.println("<meta http-equiv=\"refresh\" content=\"0; URL='http://192.168.0.20:8081/'\" />"); 
          }
 if (readString.indexOf("tesetminus") >0){
          teset = teset - 5;        
          if (teset < 10) teset = 10;
// calculate the unitar and zecinal units
teset0 = teset/256;
teset1 = teset - teset0*256;
// write in internal eeprom
EEPROM.write(150,teset1);
EEPROM.write(149,teset0);
          client.println("<meta http-equiv=\"refresh\" content=\"0; URL='http://192.168.0.20:8081/'\" />"); 
          }
 if (readString.indexOf("deteplus") >0){
          dete = dete + 1;        
          if (dete > 10) dete = 10;
          EEPROM.write(151,dete);
         client.println("<meta http-equiv=\"refresh\" content=\"0; URL='http://192.168.0.20:8081/'\" />"); 
          }
 if (readString.indexOf("deteminus") >0){
          dete = dete - 1;        
          if (dete < 1) dete = 1;
          EEPROM.write(151,dete);
         client.println("<meta http-equiv=\"refresh\" content=\"0; URL='http://192.168.0.20:8081/'\" />"); 
          }
client.println("<H2>");      
client.println("<font color=red>");    
client.print("<p>Thermostat : ");
client.print("temperature set = ");
client.print(teset/10,1);
client.print("<sup>o</sup>");
client.print("C & ");
client.print("hystesis = ");
client.print(dete/10,1);
client.print("<sup>o</sup>");
client.print("C<p>");
client.println("</font>");    


          client.println("<font color=white>");
           client.println("<a href=\"/syson\"\">Turn On LED</a>");
           client.println("<a href=\"/sysauto\"\">System in AUTO mode</a>");   
           client.println("<a href=\"/sysoff\"\">Turn Off LED</a><br />");   
           client.println("</font>");
  
  
 
 if (readString.indexOf("syson") >0){
           stare = 1;     
           client.println("<meta http-equiv=\"refresh\" content=\"0; URL='http://192.168.0.20:8081/'\" />"); 
           }
 if (readString.indexOf("sysoff") >0){
           stare = 0;
           client.println("<meta http-equiv=\"refresh\" content=\"0; URL='http://192.168.0.20:8081/'\" />"); 
           }
 if (readString.indexOf("sysauto") >0){
 //          if (10*t > teset) stare = 2;    // temprature is over set value
 //          if (10*t < teset - dete) stare = 3;   // temperature is less if we need
              client.println("<meta http-equiv=\"refresh\" content=\"0; URL='http://192.168.0.20:8081/'\" />"); 
stare = 2;
           }          
          
//           
// read old state:
//stare0 == EEPROM.read(102);

if (stare == 1) {
           client.print("<p>LED is ON (MANUAL)</p>");
           led0 = 1;
           digitalWrite(led, HIGH);
           if ((stare0 == 0) || (stare0 == 3)) {clic = clic + 1; }
// write & read data in/from internal EEPROM
// calculate the unitar and zecinal units
a = clic/256;
b = clic - a*256;
// write in internal eeprom
EEPROM.write(100,a);
EEPROM.write(101,b);
// read from internal eeprom
a1 = EEPROM.read(100);
b1 = EEPROM.read(101);
// recover number of mouse click
clic1 = a1*256 + b1;
// write state in internal eeprom 
EEPROM.write(102,1); 
stare0 = stare;
}
if (stare == 0) {
           client.print("<p>LED is OFF (MANUAL)</p>");
           led0 = 0;
           digitalWrite(led, LOW);
// write state in internal eeprom 
EEPROM.write(102,0); 
stare0 = stare;
}
if ((stare == 2) || (stare0 == 2))  // auto mode
{
          if (10*t <= teset-dete) 
          {  
           client.print("<p>LED is ON (AUTO)</p>");
           led0 = 1;
           digitalWrite(led, HIGH);
           if ((stare0 == 0) || (stare0 == 3)) {clic = clic + 1; }
// write & read data in/from internal EEPROM
// calculate the unitar and zecinal units
a = clic/256;
b = clic - a*256;
// write in internal eeprom
EEPROM.write(100,a);
EEPROM.write(101,b);
// read from internal eeprom
a1 = EEPROM.read(100);
b1 = EEPROM.read(101);
// recover number of mouse click
clic1 = a1*256 + b1;
          }
          if (10*t > teset) 
          {
           client.print("<p>LED is OFF (AUTO)</p>");   
           led0 = 0;
           digitalWrite(led, LOW);           
          }
         
 if ((10*t > teset - dete) and (10*t <= teset))
          {
 //          client.print("<p>System is on AUTO mode !</p>");
          if (led0 == 0) client.print("<p>LED is OFF (AUTO)!</p>");   
          if (led0 == 1) client.print("<p>LED is ON (AUTO)!</p>");   
          }         
       
// write state in internal eeprom 
EEPROM.write(102,2); 
stare0 = stare;
}

stare0 = stare;
clic1 = clic;
// write state in internal eeprom 
//EEPROM.write(102,stare0); 

           client.print("<p>LED was on ");
           client.print(clic1);
           client.print(" times from last reboot !<p>");
  
//           client.println("<br />");     
//           client.println("<br />"); 
           client.println("<H4>");
//           client.println("<p>Original ketch was created by Rui Santos. Visit <a href=http://randomnerdtutorials.com target=blank>http://randomnerdtutorials.com</a> for more projects!</p>");  
//           client.println("<br />"); 
           client.println("Sketch by Nicu Florica aka niq_ro. Visit <a href=http://www.tehnic.go.ro target=blank>http://www.tehnic.go.ro</a> & <a href=http://nicuflorica.blogspot.ro target=blank>http://nicuflorica.blogspot.ro/</a> for more projects!</p>");  
           client.println("<br />"); 
           client.println("<H5>");
           client.println("version. 5.0 / 02.10.2015<p>");
           client.println("</BODY>");
           client.println("</HTML>");
     
     //  readString="";  
           delay(1);
           //stopping client
           client.stop();
             //clearing string for next read
           readString="";  
     
         }
       }
    }
}   // end clinet loop

 
Serial.println("---------fara client-----");
t = dht.readTemperature();
Serial.print("t = ");
Serial.println(t,1);
Serial.print("tset = ");
Serial.println(teset/10,1);
Serial.print("dt = ");
Serial.println(dete/10,1);
Serial.print("LED = ");
Serial.println(led0);


if (stare == 0) {
           led0 = 0;
           digitalWrite(led, LOW);
}
if (stare == 1) {
           led0 = 1;
           digitalWrite(led, HIGH);
}

if ((stare == 2) || (stare0 == 2))
{
          if (10*t > teset) 
          {  
           led0 = 0;
           digitalWrite(led, LOW);
          }
          if (10*t <= teset - dete) 
          {
           led0 = 1;
           digitalWrite(led, HIGH);           
          }
}
stare0 = stare;



}   // end main loop

