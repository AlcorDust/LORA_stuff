#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

#define MY_ADDRESS 1
#define TARGET_ADDRESS 2
#define MAX_LEN 40

RH_RF95 driver;
RHReliableDatagram manager(driver, MY_ADDRESS);

bool check = 0;
uint8_t data[MAX_LEN];
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

char outstr[MAX_LEN];
byte i;
byte SNR;
byte cont = 0;

void setup() {

  Serial.begin(9600);

  if (!manager.init()) {
    Serial.println("init failed");
  }

  driver.setTxPower(23, false);

  Serial.println("---- LORA chat modulo A ----");
  Serial.println("Digita il messaggio e premi INVIO per spedire.");
}

void loop() {

  //**********************************SPEDIZIONE

  if (Serial.available()) {

    delay(100); // Non togliere questo delay!!
    i = 0;
    
    while(Serial.available()){
      
      outstr[i] = Serial.read();
      //Serial.println(outstr[i]);
      i++;

      if(i == MAX_LEN){
        break;
      }
      
    }

    String(outstr).getBytes(data, i);         // Inserisco il dato nell'array da
                                                //   spedire.
    Serial.print("Messaggio da spedire: \t\t\t\t\t\t");
    Serial.println((char*)data);
    Serial.println("Provo a spedire...");

    // Spedisco la stringa all'indirizzo e vedo se l'invio è andato a buon fine
    check = manager.sendtoWait(data, sizeof(data), TARGET_ADDRESS);

    if (check) {

      Serial.println("Invio riuscito!");
      Serial.println("Digita il messaggio e premi INVIO per spedire di nuovo");

    } else {

      Serial.println("Invio fallito!");
      Serial.println("Digita il messaggio e premi INVIO per spedire di nuovo");

    }
  }

  //**********************************RICEZIONE

  if (manager.available()) {

    Serial.println("Manager available");

    uint8_t len = sizeof(buf);
    uint8_t from;

    check = manager.recvfromAckTimeout(buf, &len, 2000, &from);

    if (check) {

      cont++;
      Serial.print("Ricevuto messaggio da : 0x");
      Serial.print(from, HEX);
      Serial.print(": \t\t\t\t\t");
      Serial.print(cont);
      Serial.print("\t");
      Serial.println((char*)buf);

      SNR = driver.lastSNR();
      Serial.print("SNR : ");
      Serial.println(SNR);

    } else {

      Serial.print("Ricezione fallita");

    }

  } // Manager available

}
