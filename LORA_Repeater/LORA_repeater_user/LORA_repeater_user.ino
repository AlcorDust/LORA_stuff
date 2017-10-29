#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

#define MY_ADDRESS 2
#define TARGET_ADDRESS 1
#define MAX_LEN 40

RH_RF95 driver;
RHReliableDatagram manager(driver, MY_ADDRESS);

bool check = 0;
uint8_t data[MAX_LEN];
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

String outstr = "";
byte i;
int SNR = 0;
char ricevuto[MAX_LEN];
int count = 0;

void setup() {

  Serial.begin(9600);

  manager.init();

  driver.setTxPower(23, false);

  Serial.println("# ---- Utente Ripetitore LORA ----\n");

}

void loop() {

  count ++;

  //**********************************SPEDIZIONE

  outstr = "";
  outstr.concat(count);
  outstr.concat(",");
  outstr.concat(SNR);
  outstr.getBytes(data, outstr.length()+10);

  //Serial.print("# Messaggio da spedire: ");
  //Serial.println((char*)data);
  //Serial.println("# Provo a spedire...");

  // Spedisco la stringa all'indirizzo e vedo se l'invio è andato a buon fine
  check = manager.sendtoWait(data, sizeof(data), TARGET_ADDRESS);

  if (check) {
    //Serial.println("# Invio riuscito!");
  } else {
    //Serial.println("# Invio fallito!");
  }

  delay(1000);

  //**********************************RICEZIONE

  if (manager.available()) {

    //Serial.println("# Manager available");

    uint8_t len = sizeof(buf);
    uint8_t from;

    check = manager.recvfromAckTimeout(buf, &len, 2000, &from);

    if (check) {

      //Serial.print("# Ricevuto messaggio da : 0x");
      //Serial.println(from, HEX);
      Serial.println((char*)buf);

      SNR = driver.lastSNR();

    } else {

      //Serial.print("# Ricezione fallita");

    }

  delay(1000);

  } // Manager available

}
