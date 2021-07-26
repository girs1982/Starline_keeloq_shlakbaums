#include "star_kee.h"
int ing = 0;
int starlineCounter = 0;
int starlinePreambula = 0;
static long StarLineCode1 = 0; // first part
static long StarLineCode2 = 0; // last part
static long invertStarLineCode1 = 0; // first part
static long invertStarLineCode2 = 0; // last part
String code = "";
String codan="";
String priem ="0";
int starkk = 0;
bool bryt_g1 = false;
bool jam_g=false;
boolean bValidPacket=false;
int decodeMethod = 1;
int lastRxValue = 0;
int tempRxValue = 0;
unsigned long lastRxTime = 0;
unsigned long tempTime = 0; 
//keeloq
int keeloqHeader=0;
int keeloqCounter = 0;
int starlineZCounter = 0;
int starlineZpreambulaCounter = 0;
int keeloqPreambula = 0;
int stralineHeader=0;
static long keeloqCode1 = 0; // first part
static long keeloqCode2 = 0; // last part
static long invertkeeloqCode1 = 0; // first part
static long invertkeeloqCode2 = 0; // last part
String keecode = "";
boolean keebValidPacket=false;
int keelastRxValue = 0;
int keetempRxValue = 0;
unsigned long keelastRxTime = 0;
unsigned long keetempTime = 0;
//пробуем выброс
//keelog start
unsigned long difTime = 0;
unsigned long difTime2 = 0;
int keelog_state = 0;
volatile int keelogCounter = 0;
byte keelog_code[9];
byte keelog_codePAK2[9];
byte keelog_codePAK1[9];
byte hugaz[9];
byte hugazk[9];
byte hugazi[9];
byte starline_code[9];
byte starline_codePAK2[9];
byte starline_codePAK1[9];
byte webpak2[9];
byte webpak1[9];
int starline_state = 0;
int zoro=1;
/////////////////////////////////////////////////VVODNIE
void init_kepsta(){
pinMode(TX,OUTPUT);
pinMode(rxPin,INPUT);
lastRxValue = digitalRead(rxPin);
lastRxTime = micros();
//kee
keelastRxValue = digitalRead(rxPin);
keelastRxTime = micros();

}
void send_meander(int time)//pra meandr
{
  digitalWrite(TX, HIGH);
  delayMicroseconds(time);
  digitalWrite(TX, LOW);
  delayMicroseconds(time);
}


void startgrabber(){
tempRxValue = digitalRead(rxPin);
if(tempRxValue != lastRxValue){
tempTime = micros();
difTime = tempTime - lastRxTime;
difTime2 = tempTime - lastRxTime;
keelog_get(); 
starline_get(); 
//grab(tempTime - lastRxTime, lastRxValue);
lastRxTime = tempTime;
lastRxValue = tempRxValue;
}
}

void starline_get(){
    bValidPacket = false;
    if(starline_state==0){//ждем преамбулу и хедер
      if(difTime2 > 900 && difTime2 < 1100 && lastRxValue ==1 ){
        starlineZpreambulaCounter ++;
      } 
      if(starlineZpreambulaCounter==6){
            starline_state=1;         
        }
       //  else{       
     //  starlineZCounter = 0;
     // }
    }
    /////////////////////////////////////////////////////PREAMBULA STARA///////////////////////////////////////////////////////////////////////////
    else if(starline_state==1){// получаем биты
      if(difTime2 > 350 && difTime2 < 650 && lastRxValue == 1){// получили 1
        if(decodeMethod==0){
          starline_code[lround(starlineZCounter/8)] = (starline_code[lround(starlineZCounter/8)]>>1)|B10000000;
        }
        else{
          starline_code[lround(starlineZCounter/8)] = (starline_code[lround(starlineZCounter/8)]<<1)|B00000000;
        }
        bValidPacket = true;
      }
      else if(difTime2 > 150 && difTime2 < 350 && lastRxValue == 1){
        if(decodeMethod==0){
          starline_code[lround(starlineZCounter/8)] = (starline_code[lround(starlineZCounter/8)]>>1)|B00000000;
        }
        else{
          starline_code[lround(starlineZCounter/8)] = (starline_code[lround(starlineZCounter/8)]<<1)|B00000001;
        }
        bValidPacket = true;
      }
      else if(lastRxValue == 0){
      }
      else{
        starline_state=1;
        starlineZCounter = 0;
      }
      
      if(bValidPacket){
        starlineZCounter++;
        if(starlineZCounter==64){           //64ili66
          starline_vardump();
          starlineZCounter = 0;
          starline_state = 0;
          starlineZpreambulaCounter = 0; /////////novshetch
          starkk++;                   /////////shetchik
        }
      }
    }
}

void starline_vardump(){
  if(decodeMethod == 0){
    Serial.println(" - starline origin - ");
  }
  else{
    Serial.println(" - starline invert - ");
  }
  Serial.print(starline_code[0], HEX);
  Serial.print(starline_code[1], HEX);
  Serial.print(starline_code[2], HEX);
  Serial.print(starline_code[3], HEX);
  Serial.println("-hop");
  Serial.print(starline_code[4], HEX);
  Serial.print(starline_code[5], HEX);
  Serial.print(starline_code[6], HEX);
  Serial.println("-fix");
  Serial.print(starline_code[7], HEX);
  Serial.println("-btn");
  Serial.print(starline_code[8], HEX);
  Serial.println("-dop");
///proda vibrosaaaaaaaaaaaaaaaa
priem="333";
///proda vibrosaaaaaaaaaaaaaaaa
codan="code starline:"+String(starline_code[0],HEX);
codan=codan+String(starline_code[1],HEX);
codan=codan+String(starline_code[2],HEX);
codan=codan+String(starline_code[3],HEX);
codan=codan+"-hop-";
codan=codan+String(starline_code[4],HEX);
codan=codan+String(starline_code[5],HEX);
codan=codan+String(starline_code[6],HEX);
codan=codan+"-fix-";
codan=codan+String(starline_code[7],HEX);
codan=codan+"-btn-";
codan=codan+String(starline_code[8],HEX);
Serial.println(codan);
}
void keelog_get(){
    bValidPacket = false;
    if(keelog_state==0){//ждем преамбулу и хедер
      if(difTime > 280 && difTime < 620 && lastRxValue != tempRxValue){
        keelogCounter ++;
      }
      else{
        if(keelogCounter==23){
          if(difTime>2800 && difTime<6200 && lastRxValue == 0){
            keelog_state=1;
          }
        }
       keelogCounter = 0;
      }
    }
    else if(keelog_state==1){// получаем биты
      if(difTime > 560 && difTime < 1240 && lastRxValue == 1){// получили 1
        if(decodeMethod==0){
          keelog_code[lround(keelogCounter/8)] = (keelog_code[lround(keelogCounter/8)]>>1)|B10000000;
        }
        else{
          keelog_code[lround(keelogCounter/8)] = (keelog_code[lround(keelogCounter/8)]<<1)|B00000000;
        }
        bValidPacket = true;
      }
      else if(difTime > 280 && difTime < 620 && lastRxValue == 1){
        if(decodeMethod==0){
          keelog_code[lround(keelogCounter/8)] = (keelog_code[lround(keelogCounter/8)]>>1)|B00000000;
        }
        else{
          keelog_code[lround(keelogCounter/8)] = (keelog_code[lround(keelogCounter/8)]<<1)|B00000001;
        }
        bValidPacket = true;
      }
      else if(lastRxValue == 0){
      }
      else{
        keelog_state=1;
        keelogCounter = 0;
      }
      
      if(bValidPacket){
        keelogCounter++;
        if(keelogCounter==66){
          keelog_vardump();
          keelogCounter = 0;
          keelog_state = 0;
         ///////  glushilka();
        }
      }
    }
}


void keelog_vardump(){
  if(decodeMethod == 0){
    Serial.println(" - keelog origin - ");
  }
  else{
    Serial.println(" - keelog invert - ");
  }
  Serial.print(keelog_code[0], HEX);
  Serial.print(keelog_code[1], HEX);
  Serial.print(keelog_code[2], HEX);
  Serial.print(keelog_code[3], HEX);
  Serial.println("-hop");
  Serial.print(keelog_code[4], HEX);
  Serial.print(keelog_code[5], HEX);
  Serial.print(keelog_code[6], HEX);
  Serial.println("-fix");
  Serial.print(keelog_code[7], HEX);
  Serial.println("-btn");
  Serial.print(keelog_code[8], HEX);
  Serial.println("-dop");
priem="333";
codan="code keqloq:"+String(keelog_code[0],HEX);
codan=codan+String(keelog_code[1],HEX);
codan=codan+String(keelog_code[2],HEX);
codan=codan+String(keelog_code[3],HEX);
codan=codan+"-hop-";
codan=codan+String(keelog_code[4],HEX);
codan=codan+String(keelog_code[5],HEX);
codan=codan+String(keelog_code[6],HEX);
codan=codan+"-fix-";
codan=codan+String(keelog_code[7],HEX);
codan=codan+"-btn-";
codan=codan+String(keelog_code[8],HEX);
Serial.println(codan);}

void keelog_send(byte* keelog_code){
  Serial.println("- sending keelog -");
  for(int i = 0; i<9; i++){
    Serial.print(keelog_code[i], HEX);
    Serial.print(" - ");
  }
  for(int i = 0; i<11; i++){//посылаем преамблу
    send_meander(400);
  }
  digitalWrite(TX, HIGH);
  delayMicroseconds(400);
  digitalWrite(TX, LOW);
  delayMicroseconds(4000);//посылаем хедер
  for( int i = 0; i<9; i++){
    if(decodeMethod==1){
      for(int i2 = 7;i2>=0;i2--){
        if(bitRead(keelog_code[i], i2)){
          digitalWrite(TX, HIGH);
          delayMicroseconds(400);
          digitalWrite(TX, LOW);
          delayMicroseconds(2*400);
        }
        else{
          digitalWrite(TX, HIGH);
          delayMicroseconds(2*400);
          digitalWrite(TX, LOW);
          delayMicroseconds(400);
        }
      }
    }
    else{
        for(int i2 = 0;i2<8;i2++){
          if(!bitRead(keelog_code[i], i2)){
            digitalWrite(TX, HIGH);
            delayMicroseconds(400);
            digitalWrite(TX, LOW);
            delayMicroseconds(2*400);
          }
          else{
            digitalWrite(TX, HIGH);
            delayMicroseconds(2*400);
            digitalWrite(TX, LOW);
            delayMicroseconds(400);
          }
       } 
    }
    
  }
}

void starline_send(byte* starline_code){
  Serial.println("- sending staray -");
  for(int i = 0; i<9; i++){
    Serial.print(starline_code[i], HEX);
    Serial.print(" - ");
  }
  for(int i = 0; i<13; i++){//посылаем преамблу//13 srab
    send_meander(1000);
  }
 // digitalWrite(TX, HIGH);
//  delayMicroseconds(1000);
//  digitalWrite(TX, LOW);
 // delayMicroseconds(4000);//посылаем хедер
  for( int i = 0; i<9; i++){
    if(decodeMethod==1){
      for(int i2 = 7;i2>=0;i2--){
        if(bitRead(starline_code[i], i2)){
          digitalWrite(TX, HIGH);
          delayMicroseconds(250);
          digitalWrite(TX, LOW);
          delayMicroseconds(250);
        }
        else{
          digitalWrite(TX, HIGH);
          delayMicroseconds(500);
          digitalWrite(TX, LOW);
          delayMicroseconds(500);
        }
      }
    }
    else{
        for(int i2 = 0;i2<8;i2++){
          if(!bitRead(starline_code[i], i2)){
            digitalWrite(TX, HIGH);
            delayMicroseconds(250);
            digitalWrite(TX, LOW);
            delayMicroseconds(250);
          }
          else{
            digitalWrite(TX, HIGH);
            delayMicroseconds(500);
            digitalWrite(TX, LOW);
            delayMicroseconds(500);
          }
       } 
    }
    
  }
}
static void ganstaDelaymic(unsigned long ms)
{
  unsigned long start = micros();
  do
  {yield();
  } while (micros() - start < ms);
}
void glushilko_gansta(int tim){yield();
while (jam_g){if(!jam_g){break;}yield();
digitalWrite(TX,!digitalRead(TX)); ESP.wdtFeed();
ganstaDelaymic(tim);}
}

void SendCame4(long Code) {
  for (int j = 0; j < 7; j++) { // посылку посылаем 4 раза подряд.
    // время стартового импульса 
    digitalWrite(TX, HIGH);
    delayMicroseconds(320);
    digitalWrite(TX, LOW);
    for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
        digitalWrite(TX, LOW); // 1
        delayMicroseconds(640);
        digitalWrite(TX, HIGH);
        delayMicroseconds(320);
      }
      else {
        digitalWrite(TX, LOW); // 0
        delayMicroseconds(320);
        digitalWrite(TX, HIGH);
        delayMicroseconds(640);
      }
    }
    digitalWrite(TX, LOW);
    delayMicroseconds(11520);
  }
}

void brutshlakgansta(){
///ESP.wdtDisable();
ESP.wdtEnable(1000);
if(bryt_g1){
 for (int send_code = 0; send_code < 4096; send_code++) // этот цикл после того как код определен необходимо убрать
  {if(!bryt_g1){break;}
    for (int j = 0; j <7; j++) // достаточно 4-х, но из-за помех поставил 7
    {
      digitalWrite(TX, HIGH); // стартовый импульс
      delayMicroseconds(700);
      digitalWrite(TX, LOW);
      for (int i = 12; i > 0; i--)
      {
        boolean bit_code = bitRead(send_code, i - 1);
        if (bit_code)
        {
          digitalWrite(TX, LOW); // единица
          delayMicroseconds(1400);
          digitalWrite(TX, HIGH);
          delayMicroseconds(700);
        }
        else
        {
          digitalWrite(TX, LOW); // ноль
          delayMicroseconds(700);
          digitalWrite(TX, HIGH);
          delayMicroseconds(1400);
        }
      }
      digitalWrite(TX, LOW); // пилотный период
      delayMicroseconds(25200);
    }

  codan = String(send_code);
  Serial.println(send_code);

 ESP.wdtFeed();
  }
}
}


