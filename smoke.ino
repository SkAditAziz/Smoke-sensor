#include <MQ2.h>
#include <ESP8266WiFi.h>

float resolution = 3.3 / 480;

String apiKey = "BBME13V18A2IPKML";

//const char* ssid = "SWH_207";
//const char* password = "029T079A081S";

const char* ssid = "AZ79";
const char* password = "1505079";

const char* host = "api.thingspeak.com";


//================================================================================ setup
void setup() {

  Serial.begin(115200);
  WiFi.disconnect();
  delay(10);

  Serial.println("Connecting to... ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}


// ====================================================================== loop
void loop() {

  float smoke = analogRead(A0);
  Serial.println(String(smoke));
  smoke = (smoke * resolution) * 100;
  Serial.println(smoke);


  String url = "https://api.thingspeak.com/update?api_key=BBME13V18A2IPKML&field1=" + String(smoke);

  Serial.println(url);


  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  //GET https://api.thingspeak.com/update?api_key=BBME13V18A2IPKML&field1=


  delay(10);
  while (!client.available()) {
    delay(100);
    Serial.print(".");
  }
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  delay(16000);

}
