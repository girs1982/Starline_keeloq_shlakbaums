#ifndef STAR_KEE_H 
#define STAR_KEE_H 
#define MAX_DELTA_1 200 ///shlack
// #define rxPin 17   ////pin reciver
// #define TX 16     //// pin transiver
#define Tranciver 1 ///1 cc1101 0 simle
#include <Arduino.h>
#include <SPI.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Preferences.h>
////#define TX 5 //d6
#define rxPin 13   ////pin reciver
#define TX 12     //// pin transiver
#define spiMosi 23
#define spiSclk 18
#define CM_MAX_TE 450
#define CM_MIN_TE 250
#define CM_BITS12 12
#define CM_BITS24 24
#define Te 320


void send_meander(int time);
void keelog_vardump();
void keelog_get();
void starline_vardump();
void starline_get();
void startgrabber();
void init_kepsta();
void starline_send(byte* starline_code);
void keelog_send(byte* keelog_code);
static void ganstaDelaymic(unsigned long ms);
void glushilko_gansta(int tim);
void SendCame4(long Code);
void starline_sendSPI(byte* starline_code);
boolean CheckValue(unsigned int base, unsigned int value);
void grabshlak();
void grabshlak2023();
void sendshlackfrom_mem();
String return_codan();
String return_sending();
bool return_disponto();
void falldisponto();
bool return_disponto2();
void falldisponto2();
void posilkeeloq1();
void posilkeeloq2();
void posilstarline1();
void posilstarline2();
void SendNice(long Code);
void brutshlak2();
void brutshlak();
void jamok(bool jam);
void keelog_sendPAK2(byte* keelog_codePAK2);
void starline_sendPAK2(byte* starline_codePAK2);
void RfReceive();
void pinRX_int();
void process_came();
void set_bit(uint8_t *data, uint8_t n);
String stringWithPrefix(String line, int len, char prefix);
void grabshlack_mymod();
#endif




