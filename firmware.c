#include <WiFi.h>

const char* ssid     = "WIFI_SSID";
const char* password = "WIFI_SSID_PASSWORD";
const int SERVER_PORT = 80;

#define ANALOG_PIN 34
#define SAMPLE_RATE 8000 // 8 kHz is sufficient for 20-500 Hz heart/lung signals

WiFiServer server(SERVER_PORT);

void setup() {
  Serial.begin(115200);
  
  // Set ADC resolution to 12-bit (0 - 4095)
  analogReadResolution(12);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected! IP Address:");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected! Streaming Analog Audio...");

    const int CHUNK_SIZE = 256;
    int16_t buffer[CHUNK_SIZE];
    
    // Dynamic running average for DC offset tracking
    float dc_offset = 2048.0; 
    float alpha = 0.01; // Low-pass tracking factor for DC bias

    while (client.connected()) {
      unsigned long sample_delay = 1000000 / SAMPLE_RATE; // microseconds per sample

      for (int i = 0; i < CHUNK_SIZE; i++) {
        unsigned long t_start = micros();

        int raw_adc = analogRead(ANALOG_PIN);

        // Update adaptive DC offset estimate
        dc_offset = (alpha * raw_adc) + ((1.0 - alpha) * dc_offset);

        // Subtract DC offset to center waveform at 0
        int16_t centered_sample = (int16_t)(raw_adc - dc_offset);

        // Scale sample to fit 16-bit audio range comfortably
        buffer[i] = centered_sample * 16;

        // Maintain 8 kHz timing interval
        while (micros() - t_start < sample_delay) {
          // Wait for next sample tick
        }
      }

      // Stream 16-bit PCM chunk over TCP
      client.write((uint8_t*)buffer, CHUNK_SIZE * sizeof(int16_t));
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}
