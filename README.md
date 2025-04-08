## Introduction
Arduino sketches to transmit CAN packets wirelessly using between two teensy 4.0s.
This is done using a custom breakout board with CAN transivers, nRF24l01P modules and various
switches and LED for IO purposes. The program uses extended CAN IDs and filters using mailboxes, 
these can be changed in RFplusCANext1. 

## Files
1. Rx_Combo - uploaded to the reciever and used for range testing
2. Tx_Combo - uploaded to the transmitter and used for range testing
3. RFplusCANExt1 - uploaded to the transmitter, filters for DTI extented CAN ID on CAN2 and transmits wirelessly
4. trace_data.h - sample data based off motor testing, uncomment section on RFplusCANExt1 to transmit over CAN1
5. RFplusCANExt2 - uploaded to the reciever, listens for nRF24 messages and prints to serial

## Range testing
This was done to range test NRF24l01P modules and does not rely on CAN communication. For range testing upload Rx_Combo and Tx_Combo to respective teensys. Digital switches on pins 16 and 17 are used to set data rate for tests

00 --> 250kbps

01 --> 1Mbps

10 or 11 --> 2Mbps

Push button on pin 20 was used to increment a counter. This was used duing testing to track the distance intervals that had been reached. The value of this counter is displayed using LEDs on pins 3, 4 and 5 in binary. 
The packet that is sent includes a timestamp, index, datarate, powerlevel and counter value. The reciever prints this to serial, and a python CSV logging script records the data along with PC timestamp - this allows for easy analysis of packet success rate/packets per second. Which is available at ____

## Read/transmit CAN packets
Files 3 and 5 facilitate wireless CAN communication between the two teensys. Mailbox interupts trigger an update to packets that are sent, these are periodically polled and sent using nRF24l01p. Polling mailboxes directly didn't work for some reason. Currently traffic on CAN bus from DTI controller = 200pps, however when traffic increases due to other CAN nodes the on both CAN buses this maybe much higher, which is why polling works nicely.


## CAN Demo
To simulate transmitting and reading CAN messages, CAN 1 is used to inject messages onto the CAN bus, with CAN 2 reading the messages and transmitting these wirelessly. The messages that are transmitted are read from the trace_data.h. This part is currently commented out and is only used for demo purposes. 

