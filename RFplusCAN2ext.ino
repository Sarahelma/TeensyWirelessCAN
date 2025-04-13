#include <SPI.h>
#include "RF24.h"

#define CE_PIN 34
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
uint8_t address[][6] = { "1Node", "2Node" };
struct message {
  uint32_t timestamp;
  uint32_t id;
  uint8_t data[8];
};
message payload;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; 
  }
  Serial.println("Receiver starting...");
  
  pinMode(13, OUTPUT);  // Debug LED
  
  // Initialize RF24 with more detailed debugging
  radio.begin();
  Serial.println("NRF24L01+ initialized successfully");
  
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(payload));
  radio.openWritingPipe(address[1]);
  radio.openReadingPipe(1, address[0]);
  radio.startListening();
  
  // Print radio details
  Serial.print("Channel: ");
  Serial.println(radio.getChannel());
  Serial.print("Payload Size: ");
  Serial.println(radio.getPayloadSize());
  Serial.println("Listening for transmissions...");
}

void loop() {

  if (radio.available()) {
    radio.read(&payload, sizeof(payload));
    Serial.print(payload.timestamp);
    Serial.print(" 0x");
    Serial.print(payload.id, HEX);

    Serial.print(" ");
    for (int i = 0; i < 8; i++) {
        Serial.print(payload.data[i], HEX); 
        Serial.print(" ");
    }
    Serial.println();
    
  }
  

}