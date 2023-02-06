#include "star_kee.h"
Preferences prefs;
Preferences prefsmem;
bool disponto=false;
bool disponto2=false;
int SMALL = 700;
int LONG =1400;  
int ing = 0;
int starlineCounter = 0;
int starlinePreambula = 0;
static long StarLineCode1 = 0; // first part
static long StarLineCode2 = 0; // last part
static long invertStarLineCode1 = 0; // first part
static long invertStarLineCode2 = 0; // last part
int k=0;//schet keeloq  
int kk  =0; //shet star line
String code = "";
String codan ="Scanning begin";
String priem ="0";
String sending ="nothing no send";
String camo_codan="";
String camo_codanI="";
 String getun="";
int starkk = 0;
///bool bryt_g1 = false;
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

volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, state;
volatile int cameCounter = 0;    // count of bits stored
volatile static long cameCode = 0;       // code itself
volatile static long lastCode = 0;


volatile byte level = 255;
volatile unsigned long last;
volatile unsigned long len;
byte p_level;
unsigned long p_len;
unsigned long p_len_prev;
struct
{
    uint8_t state;
    uint8_t data[3], dat_bit;
} came;

void init_kepsta(){
pinMode(TX,OUTPUT);
pinMode(rxPin,INPUT);
lastRxValue = digitalRead(rxPin);
lastRxTime = micros();
//kee
keelastRxValue = digitalRead(rxPin);
keelastRxTime = micros();

}

void SPI_SendBit(byte Bit){
  digitalWrite(spiMosi, Bit);
  delayMicroseconds(1);
  digitalWrite(spiSclk, HIGH);
  delayMicroseconds(1);
  digitalWrite(spiSclk, LOW);
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
///Serial.println(String(difTime));
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
if(kk<2){
prefs.begin("sta0");
prefs.putBytes("sta0",starline_code,9);
prefs.end();
}
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
disponto=true;
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
Serial.println(codan);
 if(k<2){
prefs.begin("kee0");
prefs.putBytes("kee0",keelog_code,9);
prefs.end();
//k++;
 }



disponto=true;
}

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
digitalWrite(TX,!digitalRead(TX)); 
ganstaDelaymic(tim);}
}

void SendCame4(long Code) {
   Serial.println("snding shlako:"+String(Code));
   sending=String("snding shlako:")+String(Code);
    disponto2=true;
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



void posilkeeloq1(){

 

prefs.begin("kee0", false);
//EEPROM.begin(3512);
// keelog_codePAK1[0]=EEPROM.read(0);
//keelog_codePAK1[0]=prefs.getUChar("kee0", 0);
prefs.getBytes("kee0",keelog_codePAK1,9);
prefs.end();
keelog_send(keelog_codePAK1);
 
  digitalWrite(TX, HIGH);
  delay(100);
  digitalWrite(TX, LOW);

sending="code keqloq:"+String( keelog_codePAK1[0],HEX);
sending=sending+String( keelog_codePAK1[1],HEX);
sending=sending+String( keelog_codePAK1[2],HEX);
sending=sending+String( keelog_codePAK1[3],HEX);
sending=sending+String( keelog_codePAK1[4],HEX);
sending=sending+String( keelog_codePAK1[5],HEX);
sending=sending+String( keelog_codePAK1[6],HEX);
sending=sending+String( keelog_codePAK1[7],HEX);
sending=sending+String( keelog_codePAK1[8],HEX);


 disponto2=true;
   
  keelog_state = 0;
/// for(int i = 0; i<9; i++){
///    keelog_code[i]=0;}

//Serial.println("srabotalo-keeloq");
/// for(int i = 0; i<9; i++){
///    keelog_code[i]=0;}
//Serial.println("srabotalo-keeloq");
}
void posilkeeloq2(){
prefs.begin("kee0", false);
//EEPROM.begin(3512);
// keelog_codePAK1[0]=EEPROM.read(0);
//keelog_codePAK1[0]=prefs.getUChar("kee0", 0);
prefs.getBytes("kee0",keelog_codePAK2,9);
prefs.end();
sending="code keqloq:"+String( keelog_codePAK2[0],HEX);
sending=sending+String(keelog_codePAK2[1],HEX);
sending=sending+String(keelog_codePAK2[2],HEX);
sending=sending+String(keelog_codePAK2[3],HEX);
sending=sending+String(keelog_codePAK2[4],HEX);
sending=sending+String(keelog_codePAK2[5],HEX);
sending=sending+String(keelog_codePAK2[6],HEX);
sending=sending+String(keelog_codePAK2[7],HEX);
sending=sending+String(keelog_codePAK2[8],HEX);
   keelog_sendPAK2(keelog_codePAK2);
 digitalWrite(TX, HIGH);
  delay(100);
  digitalWrite(TX, LOW);
//vibros na displey   
  keelog_state = 0;
  disponto2=true;
/// for(int i = 0; i<9; i++){
///    keelog_codePAK2[i]=0;}
/// for(int i = 0; i<9; i++){
///    keelog_codePAK2[i]=0;}
}
void posilstarline1(){

 
prefs.begin("sta0", false);
prefs.getBytes("sta0",starline_codePAK1,9);
prefs.end();
   sending="code star_line:"+String(   starline_codePAK1[0],HEX);
sending=sending+String(   starline_codePAK1[1],HEX);
sending=sending+String(   starline_codePAK1[2],HEX);
sending=sending+String(   starline_codePAK1[3],HEX);
sending=sending+String(  starline_codePAK1[4],HEX);
sending=sending+String(   starline_codePAK1[5],HEX);
sending=sending+String(  starline_codePAK1[6],HEX);
sending=sending+String(   starline_codePAK1[7],HEX);
sending=sending+String(   starline_codePAK1[8],HEX);

 starline_send(starline_codePAK1);
 disponto2=true;
  digitalWrite(TX, HIGH);
  delay(100);
  digitalWrite(TX, LOW); 
 
  starline_state = 0;
// for(int i = 0; i<9; i++){
 //   starline_code[i]=0;
//  }
 
///  Serial.println("srabotalo");
}
void posilstarline2(){ 
prefs.begin("sta0", false);
prefs.getBytes("sta0",starline_codePAK2,9);
prefs.end();
sending="code star_line:"+String(   starline_codePAK2[0],HEX);
sending=sending+String(   starline_codePAK2[1],HEX);
sending=sending+String(   starline_codePAK2[2],HEX);
sending=sending+String(   starline_codePAK2[3],HEX);
sending=sending+String(  starline_codePAK2[4],HEX);
sending=sending+String(   starline_codePAK2[5],HEX);
sending=sending+String(  starline_codePAK2[6],HEX);
sending=sending+String(   starline_codePAK2[7],HEX);
sending=sending+String(   starline_codePAK2[8],HEX);
 starline_sendPAK2(starline_codePAK2); 
 disponto2=true;
  digitalWrite(TX, HIGH);
  delay(100);
  digitalWrite(TX, LOW);
   
  starline_state = 0;
// for(int i = 0; i<9; i++){
//    starline_code[i]=0;
 // }
 
 }

//pak2
void keelog_sendPAK2(byte* keelog_codePAK2){
  Serial.println("- sending keelog -ss-");
  for(int i = 0; i<9; i++){
    Serial.print(keelog_codePAK2[i], HEX);
    Serial.print(" -zz ");
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
        if(bitRead(keelog_codePAK2[i], i2)){
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
          if(!bitRead(keelog_codePAK2[i], i2)){
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
void starline_sendPAK2(byte* starline_codePAK2){
  Serial.println("- sending staraylayn -ss-");
  for(int i = 0; i<9; i++){
    Serial.print(starline_codePAK2[i], HEX);
    Serial.print(" -zz ");
  }
  for(int i = 0; i<13; i++){//посылаем преамблу
    send_meander(1000);
  }
//  digitalWrite(TX, HIGH);
 // delayMicroseconds(400);
//  digitalWrite(TX, LOW);
//  delayMicroseconds(4000);//посылаем хедер
  
  for( int i = 0; i<9; i++){
    if(decodeMethod==1){
      for(int i2 = 7;i2>=0;i2--){
        if(bitRead(starline_codePAK2[i], i2)){
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
          if(!bitRead(starline_codePAK2[i], i2)){
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

void SendNice(long Code) {
  for (int j = 0; j < 7; j++) { // посылку посылаем 4 раза подряд.
    // время стартового импульса 
    digitalWrite(TX, HIGH);
    delayMicroseconds(SMALL);
    digitalWrite(TX, LOW);
     ////delayMicroseconds(PREAMBUL);
    for (int i = 24; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
        digitalWrite(TX, LOW); // 1
        delayMicroseconds(LONG);
        digitalWrite(TX, HIGH);
        delayMicroseconds(SMALL);
      }
      else {
        digitalWrite(TX, LOW); // 0
        delayMicroseconds(SMALL);
        digitalWrite(TX, HIGH);
        delayMicroseconds(LONG);
      }
    }
    digitalWrite(TX, LOW);
    delayMicroseconds(18500);
  }
}

String return_codan(){
codan=codan;
return codan;
}
String return_sending(){
sending=sending;
return sending;
}
bool return_disponto(){disponto=disponto;return disponto; }
void falldisponto(){disponto=false;}
bool return_disponto2(){disponto2=disponto2;return disponto2; }
void falldisponto2(){disponto2=false;}
void jamok(bool jam){
jam_g=jam;
}
boolean CheckValue(unsigned int base, unsigned int value)
{
  return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA_1)) || ((value < base) && ((base - value) < MAX_DELTA_1)));
}




void grabshlak() {
   state = digitalRead(rxPin);
  if (state == HIGH)
    lolen = micros() - prevtime;
  else
    hilen = micros() - prevtime;
  prevtime = micros(); 
  if (state == LOW)
  {
    // последовательность закончилась
    if (CheckValue(320, hilen) && CheckValue(640, lolen)) // valid 1
    {
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    }
    else if (CheckValue(640, hilen) && CheckValue(320, lolen)) // valid 0
    {
      cameCode = (cameCode << 1) | 0;
      cameCounter++;
    }
    else cameCounter = 0;
  } else 
      if (lolen > 1000 &&
           (cameCounter == 12 || cameCounter == 13) &&
           ((cameCode & 0xfff) != 0xfff))
  {
    lastCode = cameCode & 0xfff;
    codan="shlak-"+String(lastCode);
    Serial.println("Gaboslako"+String(lastCode));
    disponto=true;
   prefs.begin("esp32king", false);
   prefs.putULong("shlak0",lastCode);  
    prefs.end();
    cameCounter = 0;
    cameCode = 0;     
  }
}
void sendshlackfrom_mem(){ 

    prefs.begin("esp32king", false);
   prefs.getULong("shlak0",lastCode);  
    prefs.end(); 
      getun=String(lastCode);
   Serial.println(getun);
  ///// lastCode= atol(getun.c_str());   
    SendCame4(lastCode);
}


void grabshlak2023() {  
tempRxValue = digitalRead(rxPin);
if(tempRxValue != lastRxValue){
tempTime = micros();
if (tempRxValue == HIGH){lolen = tempTime - lastRxTime;}
if (tempRxValue == LOW){hilen = tempTime - lastRxTime;}
lastRxTime = tempTime;
lastRxValue = tempRxValue;
}  
//if(tempRxValue==LOW){
  int sibur640_1=640+MAX_DELTA_1;
  int sibur640_2=640-MAX_DELTA_1;
  int sibur320_1=320+MAX_DELTA_1;
  int sibur320_2=320-MAX_DELTA_1;
    // последовательность закончилась
    if (lolen>sibur640_2&&lolen<sibur640_1&&hilen>sibur320_2&&hilen<sibur320_1 ) // valid 1   ///lolen 640 hilen 320
    {
     /// cameCodeByte[cameCounter]=1;
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    ///Serial.println("1");
    }
    else if (hilen>sibur640_2&&hilen<sibur640_1&&lolen>sibur320_2&&lolen<sibur320_1 )// valid 0
    {
     /// Serial.println("0");
      cameCode = (cameCode << 1) | 0;
    ///  cameCodeByte[cameCounter]=0;
      cameCounter++;
      Serial.println(cameCounter);
    }
    else {cameCounter = 0;}
///}

//else 
      if (cameCounter>=13 )
  {
    lastCode = cameCode & 0xfff;
    Serial.println("Codemo"+String(lastCode));
 
    cameCounter = 0;
    cameCode = 0;
    }    
     
  //   lastCode = cameCode & 0xfff;
  //   codan="shlak-"+String(lastCode);
  //   Serial.println("Gaboslako"+String(lastCode));
  //   disponto=true;
  //  prefs.begin("esp32king", false);
  //  prefs.putULong("shlak0",lastCode);  
  //   prefs.end();
  //   cameCounter = 0;
  //   cameCode = 0;     
  
}

void grabshlack_mymod(){

  pinRX_int();
    RfReceive();

}



void RfReceive()
{
    if (level != 255)
    {    
      
        p_level = level;
        p_len = len;
        len = 0;
        level = 255;     
        process_came();
        p_len_prev = p_len;
    }
    if (came.state == 100)
    {
        for (int i = 0; i < sizeof(came.data) - (came.dat_bit == CM_BITS12 ? 1 : 0); i++) {
            if (i > 0) {
               /// Serial.print("0");
                camo_codan+="0";
                camo_codanI+="0";
            }
            /// Serial.print(String(came.data[i],HEX));
             camo_codan+=String(came.data[i],HEX);
             camo_codanI+=String(came.data[i]);
           // Serial.print(stringWithPrefix(String(came.data[i], HEX), came.dat_bit == CM_BITS12 ? 6 : 8, '0'));
        }

        codan=camo_codan;
       Serial.println(codan);
       Serial.println(camo_codanI);
       lastCode=camo_codanI.toInt();
   prefs.begin("esp32king", false);
   prefs.putULong("shlak0",lastCode);  
    prefs.end();   
        disponto=true;
        camo_codan="";
        camo_codanI="";
        came.state = 0;
        Serial.println();
    }
}


void pinRX_int()
{
  tempRxValue = digitalRead(rxPin);
  if(tempRxValue != lastRxValue){
    if (level != 255) return;
    len = micros() - last;
    last = micros();
    if (digitalRead(rxPin) == HIGH) level = 0;
    else level = 1;
    lastRxValue = tempRxValue;}
}

void process_came()
{
    unsigned char b;

    switch (came.state)
    {
    case 0:
        if (p_level) break;
        came.state = 1;
        break;
    case 1: //start
        if (!p_level) break;

        else if (p_len >= CM_MIN_TE && p_len <= CM_MAX_TE)
        {
            came.state = 2;
            came.dat_bit = 0;

            for (int i = 0; i < sizeof(came.data); i++) {
                came.data[i] = 0x00;
            }

        }
        else came.state = 0;
    case 2: //dat
        if (p_level)
        {
            if (came.dat_bit == CM_BITS24)
            {
                came.state = 0;
                break;
            }

            if (p_len_prev <= CM_MAX_TE && p_len_prev >= CM_MIN_TE &&
                    p_len <= CM_MAX_TE * 2 && p_len >= CM_MIN_TE * 2) b = 0;
            else if (p_len_prev <= CM_MAX_TE * 2 && p_len_prev >= CM_MIN_TE * 2 &&
                     p_len <= CM_MAX_TE && p_len >= CM_MIN_TE) b = 1;
            else
            {
                came.state = 0;
                break;
            }

            if (b) set_bit(came.data, came.dat_bit);
            came.dat_bit++;
            break;
        }
        else
        {
            if ((p_len > 5000) && (came.dat_bit == CM_BITS12 || came.dat_bit == CM_BITS24)) came.state = 100;
        }
        break;
    }
}


void set_bit(uint8_t *data, uint8_t n)
{
    data[n / 8] |= 1 << (n % 8);
}

String stringWithPrefix(String line, int len, char prefix)
{
    String addon = "";
    int n = len - line.length();
    for (int i = 0; i < n; i++) {
        addon += prefix;
    }
    return addon + line;
}
