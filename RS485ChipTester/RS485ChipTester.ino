void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte cmd[6] = {0xDD, 0x1F,  0x0D, 0x3A, 0x0A, 0x8B};
  byte rsp[6];
  Serial1.write(cmd,6);

  int i = 0;
  while(Serial1.available()){
    rsp[i] = Serial1.read();
    i++;
  }
  Serial.write(rsp, 6);
  Serial.println();
  delay(200);
}
