#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(9, 10);

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
    data.dataRate = 250;
  } else if (s3 == LOW && s4 == HIGH) {
    radio.setDataRate(RF24_1MBPS);
    data.dataRate = 1;
  } else {
    radio.setDataRate(RF24_2MBPS);
    data.dataRate = 2;
  }
}



void setup() { 
  Serial.begin(115200);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(20, INPUT);

  radio.begin();
  radio.setAutoAck(false);
  
  setDataRate(); 
  radio.setPALevel(RF24_PA_MAX);

  radio.openWritingPipe(pipe);
  data.index = 0;
}

void increment() {
  data.index++;
  data.timestamp = millis();
}
void count(){
  if (digitalRead(20)== HIGH){
    delay(200);
    data.counter++;
    LED();
  }
}
void LED(){
  digitalWrite(3, (data.counter >> 0) & 1);
  digitalWrite(4, (data.counter >> 1) & 1);
  digitalWrite(5, (data.counter >> 2) & 1);
}
void loop() {   

  increment();
  radio.write(&data, sizeof(packet));

  Serial.print("TX: ");
  Serial.print(data.timestamp);
  Serial.print(",");
  Serial.print(data.index);
  Serial.print(",");
  Serial.print(data.dataRate);
  Serial.print(",");
  Serial.println(data.powerLevel);
  count();
  LED();
  delay(1);
}
 