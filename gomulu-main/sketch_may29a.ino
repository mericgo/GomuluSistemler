#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Pin where the DHT11 is connected
#define DHTPIN 26 
#define DHTTYPE DHT11 
DHT dht11(DHTPIN, DHTTYPE); 

// URL to send data to
String URL = "http://192.168.1.120/gomulu_proje/test_document.php";

// WiFi credentials
const char* ssid = "malik";
const char* password = "Rayanca2.";

// Data to send
float temperature = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(115200); 
  dht11.begin(); 
  connectWiFi();  // Connect to WiFi
}

void loop() {
  // Reconnect to WiFi if disconnected
  if (WiFi.status() != WL_CONNECTED) { 
    connectWiFi();
  }
  
  // Read DHT11 data
  loadDHT11Data();

  // Prepare data for POST request
  String postData = "temperature=" + String(temperature, 1) + "&humidity=" + String(humidity, 1); 

  HTTPClient http;
  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpCode = http.POST(postData); 
  String payload = http.getString(); 
  
  // Print debug information
  Serial.print("URL: "); Serial.println(URL); 
  Serial.print("Data: "); Serial.println(postData); 
  Serial.print("HTTP Code: "); Serial.println(httpCode); 
  Serial.print("Payload: "); Serial.println(payload); 
  Serial.println("--------------------------------------------------");
  
  http.end(); // End the HTTP connection
  
  delay(2000); // Delay between requests (2 seconds)
}

void loadDHT11Data() {
  // Read temperature and humidity from DHT11
  temperature = dht11.readTemperature(); // Celsius
  humidity = dht11.readHumidity();
  
  // Check if any reads failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0.0;
    humidity = 0.0;
  } else {
    // Print the values
    Serial.printf("Temperature: %.1f °C\n", temperature);
    Serial.printf("Humidity: %.1f %%\n", humidity);
  }
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);  // Set WiFi to station mode
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Connection successful
  Serial.print("Connected to: "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}
