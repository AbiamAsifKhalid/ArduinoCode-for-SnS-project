#include <WiFi.h>

const char* ssid = "Abiam Asif Khalid ";  // WiFi network name
const char* password = "dyfgdyjhlota";    // WiFi network password
const int port = 90;                      // Port number for the server

const int dataPin = 34;           // Analog pin connected to the potentiometer
const int sampleInterval = 10;  // Sampling interval in milliseconds
const int recordDuration = 10;   // Duration to record data in seconds
const int sampleCount = recordDuration * (1000 / sampleInterval);

WiFiServer server(port);  // Create a server on port 90

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the ESP32 static IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.println("Device is Ready to send Data, Use MATLAB Record Button to record and receive data!");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {

      for (int i = 0; i < sampleCount; i++) {
        int dataValue = analogRead(dataPin);  // Read the sensor value
        client.print(dataValue*(3.3)/4095);                    // Send the Sensor value to the client (MATLAB)
        client.println();                   // Print a new line
        //Serial.println(dataValue*(3.3)/4095);
        delay(sampleInterval);  // Delay for smoother plotting (adjust as needed)
      }

      Serial.println("Do you want to record the signal again? (y/n)");

      char inp = ' ';
      while (inp != 'y' && inp != 'n') {
        if (Serial.available() > 0) {
          inp = Serial.read();
        }
        delay(100);
      }

      if (inp == 'n') {
        Serial.println("To reuse the code, restart the device");
        while (1) {}

      } else if (inp == 'y') {
        Serial.println("Device is Ready to send Data, Use MATLAB Record Button to record and receive data!");
      }
    }

    client.stop();
  }
}