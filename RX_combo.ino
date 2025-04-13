#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(34, 10);

struct packet {
  uint32_t index;
  uint32_t timestamp;
  uint8_t dataRate;
  uint8_t powerLevel;
  uint8_t counter;
};

packet data;

void setDataRate() {
  int s4 = digitalRead(17);
  int s3 = digitalRead(16);
  
  if (s3 == LOW && s4 == LOW) {
    radio.setDataRate(RF24_250KBPS);
  } else if (s3 == LOW && s4 == HIGH) {
    radio.setDataRate(RF24_1MBPS);
  } else {
    radio.setDataRate(RF24_2MBPS);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("Timestamp,Index,DataRate,PowerLevel,Counter");

  pinMode(16, INPUT);
  pinMode(17, INPUT);

  radio.begin();
  radio.setAutoAck(false);

  setDataRate(); 

  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
}

void loop() {
  while (radio.available()) {
    radio.read(&data, sizeof(packet));

    Serial.print(data.timestamp);
    Serial.print(",");
    Serial.print(data.index);
    Serial.print(",");
    Serial.print(data.dataRate);
    Serial.print(",");
    Serial.print(data.powerLevel);
    Serial.print(",");
    Serial.println(data.counter);
  }
}
