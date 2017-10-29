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

String outstr="";
int SNR;
char ricevuto[MAX_LEN];

void setup() {

  manager.init();
  driver.setTxPower(23, false);

  //Serial.begin(9600);

}

void loop() {

  if (manager.available()) {

    uint8_t len = sizeof(buf);
    uint8_t from;

    check = manager.recvfromAckTimeout(buf, &len, 2000, &from);

    //Serial.println((char*)buf);
        
    if (check) {

      SNR = driver.lastSNR();

      outstr = "A,";
      outstr.concat((char*)buf);
      outstr.concat(",");
      outstr.concat(SNR);
      outstr.concat(",Z");
      outstr.getBytes(data, outstr.length()+10);
            
      manager.sendtoWait(data, sizeof(data), TARGET_ADDRESS);

      check = 0;

    }

  } // Manager available

}
