unsigned long ts = 0;



void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
}

void loop() {
  short int pos = 1000;
  
  unsigned char com[4] = {0x76,0x01,highByte(pos),lowByte(pos)};
  unsigned short int ck16 = return_CRC(com);
  byte hb = highByte(ck16), lb = lowByte(ck16);
  Serial.println("");


  unsigned char ck16c[6] = {com[0], com[1], com[2], com[3], hb, lb};

  Serial1.write(ck16c, 6);

  byte rsp[6];

  char i = 0;

  ts = millis();
  while(!Serial1.available()&&(millis()-ts < 1000)){}
  while(Serial1.available()){
    rsp[i] = Serial1.read();
    i++;
  }

  for (int i = 0; i < 6; i++){
    Serial.print(ck16c[i], HEX);
    Serial.print(" ");

  }
  Serial.println();
  
  for (int i = 0; i < 6; i++){
    Serial.print(rsp[i], HEX);
    Serial.print(" ");

  }
  Serial.println();
  
  delay(10);
  
}

short int return_CRC(unsigned char cmd[4]){
  unsigned short int crc = 0; // Result

  char x, y;
  crc=0xFFFF; //init value

  for (x=0; x<4; x++)
  {
    crc = ( ( cmd[x] <<8 ) ^ crc);
    for (y=0; y<8; y++ )
    {
      if ( crc & 0x8000 ){
        crc = (crc << 1) ^ 0x8005;
      }
      else
      {  
        crc = crc << 1;
      }
    }
  }
  return crc;
}


void sndrcv(){
  byte cmd[6] = {0xDD, 0x1F,  0x0D, 0x1A};
  Serial1.write(cmd,6);

  byte rsp[6];

  int i = 0;
  while(Serial1.available()){
    rsp[i] = Serial1.read();
    i++;
  }
  
  for (int i = 0; i < 6; i++){
    Serial.print(rsp[i]);
    Serial.print(" ");

  }
  Serial.println();
  delay(200);
}
