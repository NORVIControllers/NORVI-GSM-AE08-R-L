/*
 NORVI GSM-AE08-R-L FINAL
 * 
 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "FS.h"
#include "SD.h"
#include "RTClib.h"
#include <WiFi.h>

#define ANALOG_PIN_0 36

#define INPUT1 13
#define INPUT2 34
#define INPUT3 35
#define INPUT4 14

#define RXD 25 // RS485
#define TXD 26
#define FC 22

#define OUTPUT1 12
#define OUTPUT2 2
#define OUTPUT3 27
#define OUTPUT4 4

#define GSM_RX 33
#define GSM_TX 32
#define GSM_RESET 21

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

RTC_DS3231 rtc; 
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
unsigned int localPort = 8888;      
const char timeServer[] = "time.nist.gov"; 
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[NTP_PACKET_SIZE]; 
EthernetUDP Udp;

#define OLED_RESET     -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int analog_value = 0;

  
int readSwitch(){
  analog_value = analogRead(ANALOG_PIN_0);

 
  return analog_value   ; //Read analog
}



unsigned long int timer1 = 0;


void setup() {
 
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD, TXD);
  Serial2.begin(115200, SERIAL_8N1, GSM_RX, GSM_TX); 

  pinMode(FC, OUTPUT);
  digitalWrite(FC, HIGH);

  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, HIGH);  
  
  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);
  pinMode(OUTPUT3, OUTPUT);
  pinMode(OUTPUT4, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(5,HIGH);
  digitalWrite(15,HIGH);

  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
 
  
  Wire.begin(16,17);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
   display.display();
   
  WiFi.mode(WIFI_MODE_APSTA);
  delay(300);
  wifi_test();
  delay(300);
  RTC_Check();
  delay(300);
  SD_CHECK();
  delay(300);
  ETHERNET_CHECK();
  delay(300);

  Serial.println("Testing Modem");
  

 timer1 = millis();
  Serial2.println("AT");
  while(millis()<timer1+1000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
    
  }

  timer1 = millis();
  Serial2.println("AT+CPIN?");
  while(millis()<timer1+1000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }

  timer1 = millis();
  Serial2.println("AT+GSN");
  while(millis()<timer1+1000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }
 
  timer1 = millis();
  Serial2.println("ATD+CSQ");
  while(millis()<timer1+1000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }

 
  
 digitalWrite(FC, HIGH);
}

void loop() {
  
  Serial.print(digitalRead(INPUT1));Serial.print(digitalRead(INPUT2));Serial.print(digitalRead(INPUT3));Serial.print(digitalRead(INPUT4));
  Serial.println(""); 
  
  Serial.print("Push button  ");Serial.println(readSwitch()); 

  digitalWrite(OUTPUT1, HIGH); delay(200);
  digitalWrite(OUTPUT1, LOW);  delay(200);
  digitalWrite(OUTPUT2, HIGH); delay(200);
  digitalWrite(OUTPUT2, LOW);  delay(200);
  digitalWrite(OUTPUT3, HIGH); delay(200);
  digitalWrite(OUTPUT3, LOW);  delay(200);
  digitalWrite(OUTPUT4, HIGH); delay(200);
  digitalWrite(OUTPUT4, LOW);  delay(300);
  Serial.println("");  
  
    while (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }


  while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }


  Serial.println(""); 
  digitalWrite(FC, HIGH);                   
  delay(100);
  Serial1.println(F("RS485 01 SUCCESS"));    
  delay(100);                               
  digitalWrite(FC, LOW) ;                   

                                           
  delay(100);     
  
  while (Serial1.available()) {  
    char c = Serial1.read();     
    Serial.write(c);             
  }

  Serial.println("------------------------------------"); 
}


void displayTime(void) {
  DateTime now = rtc.now();
     
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(500);

}

void RTC_Check(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
 else{
 if (rtc.lostPower()) {
  
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
  }

   
  int a=1;
  while(a<6)
  {
  displayTime();   // printing time function for oled
  a=a+1;
  }
 }
}


void SD_CHECK(){
  uint8_t cardType = SD.cardType();
  
    if(SD.begin(15))
 {
  Serial.println("Card Mount: success");
  Serial.print("Card Type: ");

    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("Unknown");
    }

  int cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("Card Size: %lluMB\n", cardSize);

  }

  if(!SD.begin(15))
  {
  Serial.println("NO SD card");            
  }

}

void ETHERNET_CHECK(){
  Ethernet.init(5);  // ESP32 with Adafruit Featherwing Ethernet

  // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");

    // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
  Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  }
  
  if (Ethernet.linkStatus() == LinkOFF) {
   Serial.println("Ethernet cable is not connected.");
  }
  }
  
 else{
  Udp.begin(localPort);
 
  sendNTPpacket(timeServer); // send an NTP packet to a time server

  // wait to see if a reply is available
  delay(1000);
  if (Udp.parsePacket()) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
   Serial.print("The UTC time is ");
   Serial.print((epoch  % 86400L) / 3600);
    // print the hour (86400 equals secs per day)
   Serial.print(':');
   if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
   }
    
   Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
   Serial.print(':');
   if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
   }
   
   Serial.println(epoch % 60); // print the second
   }
  // wait ten seconds before asking for the time again
  delay(1000);
  Ethernet.maintain();
 }
}


void sendNTPpacket(const char * address) 
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket(); 
}

void wifi_test(){
    Serial.println("");
String str_macAddress;
byte mac[6];

WiFi.macAddress(mac);

   str_macAddress = (String(mac[0] >> 4,  HEX) + String(mac[0] & 0x0F, HEX)) + (":") +
                   (String(mac[1] >> 4,  HEX) + String(mac[1] & 0x0F, HEX)) + (":") +
                   (String(mac[2] >> 4,  HEX) + String(mac[2] & 0x0F, HEX)) + (":") +
                   (String(mac[3] >> 4,  HEX) + String(mac[3] & 0x0F, HEX)) + (":") +
                   (String(mac[4] >> 4,  HEX) + String(mac[4] & 0x0F, HEX)) + (":") +
                   (String(mac[5] >> 4,  HEX) + String(mac[5] & 0x0F, HEX));
  str_macAddress.toUpperCase();
  
  Serial.println("MAC Address: " + str_macAddress);

  String ssid =  str_macAddress; 
  const char* password = "12345678";   

  Serial.println("Setting up the Wi-Fi Access Point...");
  WiFi.softAP(ssid.c_str(), password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  Serial.print("Access Point SSID: ");
  Serial.println(ssid);

  Serial.println("Wi-Fi Access Point is active!");
  Serial.println("");
}
