#include <SPI.h>
#include <FlexCAN_T4.h>
#include "RF24.h"

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> TxCAN;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> RxCAN;

uint8_t address[][6] = { "1Node", "2Node" };
struct message {
  uint32_t timestamp;
  uint32_t id;
  uint64_t data= 0;
};
message payload[4];

void setup() {
  Serial.begin(115200);

  Serial.println("Transmitter starting...");
  
  pinMode(13, OUTPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(20, INPUT);

  // Initialize CAN
  TxCAN.begin();
  TxCAN.setBaudRate(500000);
  
  RxCAN.begin();
  RxCAN.setBaudRate(500000);
  RxCAN.setMaxMB(4);
  for (int i = 0; i < 4; i++) {
    RxCAN.setMB((FLEXCAN_MAILBOX)i, RX, EXT);
  }
  RxCAN.setMBFilter(REJECT_ALL);
  RxCAN.enableMBInterrupts();
  RxCAN.onReceive(MB1, canSniff1);
  RxCAN.onReceive(MB2, canSniff2);
  RxCAN.onReceive(MB3, canSniff3);
  RxCAN.onReceive(MB4, canSniff4);
  RxCAN.setMBUserFilter(MB1, 0x2014, 0xFF);
  RxCAN.setMBUserFilter(MB2, 0x2114, 0xFF);
  RxCAN.setMBUserFilter(MB3, 0x2214, 0xFF);
  RxCAN.setMBUserFilter(MB4, 0x2314, 0xFF);
  // Initialize RF24
  radio.begin();

  Serial.println("NRF24L01+ initialized");
  
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(message));
  radio.openWritingPipe(address[0]);
  radio.openReadingPipe(1, address[1]);
  radio.stopListening();  
  
  Serial.println("Setup complete, beginning transmission...");
}

void canSniff1(const CAN_message_t &msg) {canSniff(msg);}
void canSniff2(const CAN_message_t &msg) {canSniff(msg);}
void canSniff3(const CAN_message_t &msg) {canSniff(msg);}
void canSniff4(const CAN_message_t &msg) {canSniff(msg);}

void canSniff(const CAN_message_t &msg) {
  Serial.print("0x");
  Serial.print(msg.id, HEX);
  
  Serial.print(" time");
  Serial.print(msg.timestamp);
  for (uint8_t i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }
  int index = -1;
  if (msg.id == 0x2014) index = 0;
  else if (msg.id == 0x2114) index = 1;
  else if (msg.id == 0x2214) index = 2;
  else if (msg.id == 0x2314) index = 3;

  if (index != -1) {
 //   memcpy(&payload[index].timestamp, &msg.timestamp, sizeof(payload[index].timestamp));
    payload[index].timestamp = millis();
    memcpy(&payload[index].id, &msg.id, sizeof(payload[index].id));
    memcpy(&payload[index].data, &msg.buf, sizeof(payload[index].data));

      
  }
}


int counter =1;
void loop() {
  RxCAN.events();
  
/*


  if (digitalRead(20) == HIGH) {
    delay(50);  //debounce delay
    if (digitalRead(20) == HIGH) { 
      counter++;  
      if (counter > 4) counter = 1;  // 
      while (digitalRead(20) == HIGH);  // 
      delay(50);  //
    }
  }
    // inject sample msg
  CAN_message_t msg;
  msg.id = 0x2014;
  switch (counter) {
    case 1: msg.id = 0x2014; break;
    case 2: msg.id = 0x2114; break;
    case 3: msg.id = 0x2214; break;
    case 4: msg.id = 0x2314; break;
  }
  msg.flags.extended = 1;
  msg.len = 4;
  msg.buf[0] = digitalRead(14);
  msg.buf[1] = digitalRead(15);
  msg.buf[2] = digitalRead(16);
  msg.buf[3] = digitalRead(17);
  
  if (TxCAN.write(msg)) {
    Serial.println("CAN Message Sent");
  }
*/
  Serial.print("Sending payload: ");

  Serial.print(" ");
  Serial.println();
  for (int i = 0; i < 4; i++) {
    Serial.print("index: ");
    Serial.print(i);
    Serial.print(" ID: 0x");
    Serial.print(payload[i].id, HEX);
    Serial.print(" Timestamp: ");
    Serial.print(payload[i].timestamp);
    Serial.print(" Data: ");
    Serial.println(payload[i].data, HEX);
    if (radio.write(&payload[i], sizeof(payload))) {
      Serial.println("RF24 Transmission successful");
      digitalWrite(13, HIGH);
      delay(1);
      digitalWrite(13, LOW);
    } else {
      Serial.println("RF24 Transmission failed");
    }
    Serial.print(payload[i].data, HEX);
    }
  delay(1);
}