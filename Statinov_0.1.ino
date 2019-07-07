#include <ESP8266WiFi.h>
#include "DHT.h"
DHT dht;
const char* ssid = "SSID";
const char* password = "Password";
int bluePin = D1;
int  bluevalue;
WiFiServer server(80);
void setup() 
{
  Serial.begin(115200);
  delay(10);
  dht.setup(2); // data pin 2
//set up LEDs
  pinMode(bluePin, OUTPUT);
  digitalWrite(bluePin, LOW);
// Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
                                        }
  Serial.println("WiFi connected");
// Start the server
  server.begin();
  Serial.println("Server started");
// Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
void loop() {
hum:
if (dht.getHumidity()>60)//humidity will turn led&Relay on 
{
   digitalWrite(bluePin, HIGH);
   bluevalue = HIGH;
   goto hum;
}

{
delay(dht.getMinimumSamplingPeriod());
Serial.print("Humidity =");Serial.println(dht.getHumidity());
Serial.print("\t");
Serial.print("Temp =");Serial.println(dht.getTemperature());
}

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request

  
  int bluevalue = LOW;
  if (request.indexOf("/BLUELED=ON") != -1) {
    digitalWrite(bluePin, HIGH);
    bluevalue = HIGH;
  } 
  if (request.indexOf("/BLUELED=OFF") != -1){
    digitalWrite(bluePin, LOW);
    bluevalue = LOW;
  }
  // Return the response on browser
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
//Bootstrap CDN 3.3.7
 client.println("<head><title>Bootstrap Example</title> <meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'>");
 client.println("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>");
 client.println("<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js'>");
 client.println("</script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script></head>");
client.println("<body style='background-color:#4d7287;'>");
client.println("<br><br><br><br><br><br><center>");
client.println("<div class='btn btn-success btn-default' style='width:400px; color:#155743; background:#CCE5FF; padding:30px;'> ");
client.print("Blue led pin is now: ");  
  if(bluevalue == HIGH) {
    client.print(" On");  
  } else {
    client.print(" Off");
  }
  client.println("<br><br><br><br><center>");

  
  client.println("Humidity ="); client.println(dht.getHumidity());
  client.println("<br><br><center>");
  client.println("Temp="); client.println(dht.getTemperature());
  client.println("<br><br><center>");
  client.print("<a href=\"/BLUELED=ON\" ><input type=\"button\" value=\" LED ON\" class='btn btn-success btn-default'></a>");
  client.print("&nbsp;");client.print("&nbsp;");
  client.print("<a href=\"/BLUELED=OFF\"><input type=\"button\" value=\" LED OFF\"  class='btn btn-danger btn-default'></a>");
  client.println("</div> ");
//Refresh page per 5sec
client.println("<meta http-equiv='refresh' content='5' >");
client.print("</html>");
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
