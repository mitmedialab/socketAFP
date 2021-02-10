#include "msgpck.h"

/* 
 * This is an example of message pack library use for Arduino mega.
 *
 * It writes some message pack serialized data on Serial1 and receives them on Serial2.
 * Serial is used, plugged with your computer, in order to display the json corresponding 
 * to data exchanged on your serial console.
 * 
 * Expected output:
 * Start
 * {"nil": nil, "bool": false, "uint": 12, "int": -256, "float": 0.50, "array": [5, 2500, -1503]}
 * Done
 * 
 * Exchanged Message Pack data:
 * 
 * 0x86 0xa3 0x6e 0x69 0x6c 0xc0 0xa4 0x62
 * 0x6f 0x6f 0x6c 0xc2 0xa4 0x75 0x69 0x6e
 * 0x74 0x0c 0xa3 0x69 0x6e 0x74 0xd1 0xff
 * 0x00 0xa5 0x66 0x6c 0x6f 0x61 0x74 0xca
 * 0x3f 0xe0 0x00 0x00 0xa5 0x61 0x72 0x72
 * 0x61 0x79 0x93 0x05 0xcd 0x09 0xc4 0xd1
 * 0xfa 0x21
 * 
 * Wiring:
 * Connect Rx1 with Tx2
 * Connect Rx2 with Tx1
*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
//
//   Serial.println("Start");
  
  msgpck_write_map_header(&Serial, 3);
  //1
  msgpck_write_string(&Serial, "nil", 3);
  msgpck_write_nil(&Serial);
 //2 
  msgpck_write_string(&Serial, "bool", 4);
  msgpck_write_bool(&Serial, false);
 //3 
  msgpck_write_string(&Serial, "uint", 4);
  msgpck_write_integer(&Serial, 12);

  delay(100); 
  msgpck_write_map_header(&Serial, 3);
  msgpck_write_string(&Serial, "int", 3);
  msgpck_write_integer(&Serial, -256);

  msgpck_write_string(&Serial, "float", 5);
  float f = 0.5;
  msgpck_write_float(&Serial, f);
 
  msgpck_write_string(&Serial, "array", 5);
  msgpck_write_array_header(&Serial, 3);
  uint8_t u0 = 5;
  uint16_t u1 = 2500;
  int32_t i0 = -1503;
  msgpck_write_integer(&Serial, u0);
  msgpck_write_integer(&Serial, u1);
  msgpck_write_integer(&Serial, i0);

  delay(100); 
//  Serial.println("hello world"); 
  
//  delay(1000);
//  msgpck_to_json(&Serial, &Serial);
  
//  Serial.println();
//  Serial.println("Done");
//  delay(10); 
//  Serial.println("hello"); 
//  delay(20);
//  Serial.print("hola"); 
//  delay(20);
//  Serial.print("adios");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
