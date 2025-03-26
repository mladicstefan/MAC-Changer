#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


void generate_random_mac(uint8_t mac[6]){
  mac[0] = ((uint8_t)(rand() % 256 ) & 0xFE) | 0x02;
  for (int i = 1; i<6; i++){
    mac[i] = (uint8_t)(rand() % 256);
  }
}

int main(void){
  uint8_t mac[6];
  srand((unsigned)time(NULL));
  generate_random_mac(mac);
  printf("Random MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return 0;
}

