#include <WiFi.h> 
#include <WebServer.h>
/* SSID & Password */ 
const char* ssid = "ESP32_Eben";  
const char* password = "pietrols";  
/* IP Address*/ IPAddress 
local_ip(192,168,1,1); 
IPAddress gateway(192,168,1,1); 
IPAddress subnet(255,255,255,0);
WebServer server(80);
uint8_t LED1pin = 2; 
bool LED1stat = LOW;
uint8_t LED2pin = 1; 
bool LED2stat = LOW;
void setup() 
{ 
  Serial.begin(115200); 
  pinMode(LED1pin, OUTPUT); 
  pinMode(LED2pin, OUTPUT);
  WiFi.softAP(ssid, password); 
  WiFi.softAPConfig(local_ip, gateway, subnet); 
  delay(100);
  
  server.on("/", handle_OnConnect); 
  server.on("/led1on", handle_led1on); 
  server.on("/led1off", handle_led1off); 
  server.on("/led2on", handle_led2on); 
  server.on("/led2off", handle_led2off); 
  server.onNotFound(handle_NotFound);
  server.begin(); 
  Serial.println("HTTP server started");
}

void loop() 
{ 
  server.handleClient(); 
  if(LED1stat) { digitalWrite(LED1pin, HIGH); } else { digitalWrite(LED1pin, LOW); }
  if(LED2stat) { digitalWrite(LED2pin, HIGH); } else  { digitalWrite(LED2pin, LOW); }
}

void handle_OnConnect() { server.send(200, "text/html", SendHTML(LED1stat,LED2stat)); }
void handle_led1on() { server.send(200, "text/html", SendHTML(true,LED2stat)); }
void handle_led1off() { server.send(200, "text/html", SendHTML(false,LED2stat)); }
void handle_led2on() { server.send(200, "text/html", SendHTML(LED1stat,true)); }
void handle_led2off() { server.send(200, "text/html", SendHTML(LED1stat,false)); }
void handle_NotFound(){ server.send(404, "text/plain", "Not found"); }

String SendHTML(uint8_t led1stat,uint8_t led2stat)
{ 
  String ptr = "<!DOCTYPE html> <html>\n"; 
  ptr +="<head>\n"; 
  ptr +="<title>LED Control</title>\n"; 
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n"; 
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;marginbottom: 50px;}\n"; 
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n"; 
  ptr +=".button-on {background-color: #3498db;}\n"; 
  ptr +=".button-on:active {background-color: #2980b9;}\n"; 
  ptr +=".button-off {background-color: #34495e;}\n"; 
  ptr +=".button-off:active {background-color: #2c3e50;}\n"; 
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n</style>\n</head>\n<body>\n"; 
  ptr +="<h1>ESP32 Web Server</h1>\n"; 
  
  if(led1stat) 
  {
    ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n"; 
  } 
  else  
  { 
    ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n"; 
  } 
  if(led2stat) 
  { 
    ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n"; 
  } 
  else  
  { 
    ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n"; 
  } 
  ptr +="</body>\n</html>\n"; 
  return ptr;
}
