#define motora 2 // Motor a+ pin
#define motorb 3 // Motor a- pin
#define motorc 4 // Motor b+ pin
#define motord 5 // Motor b- pin
#define motorenable1 6 // Speed control for Motor
#define trigPin 7   // Trigger pin of the sensor
#define echoPin 8  // Echo pin of the sensor
#define solenoid 9 // Soleniod Control pin
#define motorenable2 10 // Speed control for Motor

const int sensorPin = A0; // Sensitivity in mV per Ampere (adjust based on your sensor's rating)
const float sensitivity = 185.0; // for ACS712-05B (185 mV/A)
const int baselineOffset = 2500; // Sensor baseline offset in millivolts (typically 2500 for ACS712 sensors with a 5V supply)
const int numSamples = 100; // Number of samples for averaging

#include <Sipeed_ST7789.h>
#include "Maix_Speech_Recognition.h"
#include "voice_model_repo.h"
#include "WiFiEsp.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "Angu";            // your network SSID (name)
char pass[] = "naannallavan";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status


const char* apiKey = "X903CMCOLAM1X61S";  // Replace with your ThingSpeak Write API Key
const char* server = "api.thingspeak.com";  // ThingSpeak server
WiFiEspClient client;


long duration;
      int activate = 1;
int distance;
SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(320, 240, spi_, SIPEED_ST7789_DCX_PIN, SIPEED_ST7789_RST_PIN, DMAC_CHANNEL2);

SpeechRecognizer rec;
    void  Clean_activate(); 
void setup()
{
                           
  Serial.begin(115200); // initialize serial for debugging
  Serial1.begin(115200);  // initialize serial for ESP module
   WiFi.init(&Serial1); // initialize ESP module
   // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid); // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

    
    pinMode(motora, OUTPUT);
    pinMode(motorb, OUTPUT);
    pinMode(motorc, OUTPUT);
    pinMode(motord, OUTPUT);
     pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(solenoid, OUTPUT);
    digitalWrite(motora, LOW);
  digitalWrite(motorb, LOW);  
  digitalWrite(motorc, LOW);
  digitalWrite(motord, LOW);
  digitalWrite(solenoid, LOW);
   digitalWrite(motorenable1, LOW);
  digitalWrite(motorenable2, LOW);  

  
    lcd.begin(15000000, COLOR_RED);   
    
    rec.begin();
    Serial.println("init model...");
    rec.addVoiceModel(0, 0, beanbot_0, hello_beanbot_0); 
    rec.addVoiceModel(0, 1, beanbot_1, hello_beanbot_1);
    rec.addVoiceModel(0, 2, beanbot_2, hello_beanbot_2);
    rec.addVoiceModel(0, 3, beanbot_3, hello_beanbot_3);   
    rec.addVoiceModel(1, 0, on_0, Engines_on_0); 
    rec.addVoiceModel(1, 1, on_1, Engines_on_1);
    rec.addVoiceModel(1, 2, on_2, Engines_on_2); 
    rec.addVoiceModel(1, 3, on_3, Engines_on_3); 
    rec.addVoiceModel(2, 0, start_0, engine_start_0);
    rec.addVoiceModel(2, 1, start_1, engine_start_1);
    rec.addVoiceModel(2, 2, start_2, engine_start_2);
    rec.addVoiceModel(2, 3, start_3, engine_start_3);
    rec.addVoiceModel(3, 0, stop_0, engine_stop_0);
    rec.addVoiceModel(3, 1, stop_1, engine_stop_1);
    rec.addVoiceModel(3, 2, stop_2, engine_stop_2);
    rec.addVoiceModel(3, 3, stop_3, engine_stop_3);
    Serial.println("init model ok!");
    lcd.fillScreen(COLOR_RED);
    lcd.setTextSize(2);
    lcd.setTextColor(COLOR_WHITE);
    lcd.setCursor(100,100);
    lcd.setRotation(3);
    lcd.fillScreen(COLOR_RED);
      lcd.setCursor(100,100);

}
void loop()
{
    int res;
    while(1)
    {
      delayMicroseconds(200);
      lcd.fillScreen(COLOR_RED);
      lcd.setCursor(50,100);
      lcd.print("Welcome");
      res = rec.recognize();
      Serial.printf("res : %d ", res);
    if (res == 1)
    {
       delayMicroseconds(200);
       lcd.fillScreen(COLOR_RED);
      lcd.setCursor(100,100);
      lcd.print("Hey Angu");
     break;
    }
    else
    {
       delayMicroseconds(50);
      lcd.fillScreen(COLOR_RED);
      lcd.setCursor(50,100);
     lcd.println("Speak again");
      delay(500);
    }
    }
       while(1)
       {
         delayMicroseconds(200);
       res = rec.recognize();
       Serial.printf("res : %d ", res);
            if(res == 2)
            {
          lcd.fillScreen(COLOR_RED);
          lcd.setCursor(5,100);
          lcd.print("Engines activated");
       
           delay(2000);
             break;
            }
            else
            {
              lcd.fillScreen(COLOR_RED);
             lcd.setCursor(50,100);
              lcd.print("Speak again");
              delay(500);
            }
          }
         while(1)
     {
          delayMicroseconds(50);
          res = rec.recognize();
          Serial.printf("res : %d ", res);
         if(res == 3)
          {
             delayMicroseconds(200);
             lcd.fillScreen(COLOR_RED);
            lcd.setCursor(5,100);
            lcd.print("Beach Cleaning Started");
                       break;
           }
          else
          {
            lcd.fillScreen(COLOR_RED);
            lcd.setCursor(100,100);
            lcd.print("Speak again");
              delay(500);
          }
     }
     do
     {
     
       int  engines = 1;
      Clean_activate();

      printCurrentNet();
  printWifiData();
     sendDataToThingSpeak(activate, engines);
          }while(1);
      }
void  Clean_activate()
{
  analogWrite(motorenable1,200);
  analogWrite(motorenable2,200);
  Serial.println("motorenable");
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trigger the ultrasonic pulse by setting the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin to measure the duration of the pulse
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2; // Speed of sound is approximately 0.034 cm/µs

  // Display the distance in the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Wait for a short moment before taking another measurement
  delay(200);
  if (distance > 15)
  {
    lcd.fillScreen(COLOR_RED);
     lcd.setCursor(10,100);
     lcd.print("Motor forward");
     motorforward();
     delay(500);
     motorstop();
     delay(1000);
    lcd.fillScreen(COLOR_RED);
     lcd.setCursor(10,100);
     lcd.print("Sand Seiving");
     solenoidcontrol();
     
  }
 else
 {
  lcd.fillScreen(COLOR_RED);
     lcd.setCursor(10,100);
    lcd.print("Obstacle detected");
      motorstop();
  delay(2000);
  motorreverse();
  delay(2000);
  motorright();
  delay(2000);
 }
}
void solenoidcontrol()
{
  for(int i=0; i<20;i++)
  {
digitalWrite(solenoid,HIGH);
 delay(200);
 digitalWrite(solenoid,LOW);
 delay(200);
  }
 
    }
  void motorforward(){
  digitalWrite(motora, HIGH);
  digitalWrite(motorb, LOW);  
  digitalWrite(motorc, HIGH);
  digitalWrite(motord, LOW);
  Serial.println("Motorforward");
  }

   void motorreverse(){
  digitalWrite(motora, LOW);
  digitalWrite(motorb, HIGH);  
  digitalWrite(motorc, LOW);
  digitalWrite(motord, HIGH);
    Serial.println("Motorreverse");
  }

  void motorright(){
  digitalWrite(motora, LOW);
  digitalWrite(motorb, HIGH);  
  digitalWrite(motorc, HIGH);
  digitalWrite(motord, LOW);
    Serial.println("Motorright");
  }
  void motorleft()
  {
  digitalWrite(motora, HIGH);
  digitalWrite(motorb, LOW);  
  digitalWrite(motorc, LOW);
  digitalWrite(motord, HIGH);
     Serial.println("Motorleft");
  }
  void motorstop()
  {
     digitalWrite(motora, LOW);
  digitalWrite(motorb, LOW);  
  digitalWrite(motorc, LOW);
  digitalWrite(motord, LOW);
    Serial.println("Motorstop");
  }
   void printWifiData()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

 }

void printCurrentNet()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  }

   void sendDataToThingSpeak(int activate, int engines) {
  if (client.connect(server, 80)) {  // Connect to ThingSpeak on port 80
    Serial.println("Connected to ThingSpeak");
    // Create the URL for the HTTP request
    String url = "/update?api_key=" + String(apiKey) + "&field1=" + String(activate)+ "&field2=" + String(engines);
    // Send HTTP GET request to ThingSpeak
    client.println("GET " + url + " HTTP/1.1");
     client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println();  // End of headers
    
    // Wait for the response from the server
    long timeout = millis() + 5000;  // 5 seconds timeout
    while (client.connected() && millis() < timeout) {
      if (client.available()) {
        String response = client.readString();
        Serial.println("Server Response: ");
        Serial.println(response);  // Print the server response
        break;
      }
    }

    if (millis() >= timeout) {
      Serial.println("Server response timed out.");
    }

    client.stop();  // Close the connection
    Serial.println("Data sent to ThingSpeak.");
  } else {
    Serial.println("Connection to ThingSpeak failed.");
  }
}
  
