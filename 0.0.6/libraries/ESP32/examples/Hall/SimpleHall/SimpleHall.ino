/*
  SimpleHall
  The Hall value been read is changing with the movement of the magnet to esp32.

  modified 2017-6-15
  by ouki-wang
 */


void setup() {
  Serial.begin(115200);
}


void loop() {
  Serial.println(hallRead());
  delay(50);              // wait for 50ms
}