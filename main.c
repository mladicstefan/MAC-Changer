#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>





static int set_interface_state(const char *ifname, int state){
  int fd;
  struct ifreq ifr;

  fd = socket(AF_INET,SOCK_DGRAM,0);
  if (fd < 0){
    perror("socket");
    return -1;
  }

  memset(&ifr,0,sizeof(ifr));
  strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);

  if (ioctl(fd,SIOCGIFFLAGS, &ifr)<0){
    perror("ioctl(SIOCGIFFLAGS)");
    close(fd);
    return -1;
  }
  
  if (state){ ifr.ifr_flags |= IFF_UP;}
  else {ifr.ifr_flags &= ~IFF_UP;}

  if (ioctl(fd,SIOCSIFFLAGS,&ifr) < 0){
    perror("ioctl(SIOCSIFFLAGS)");
    close(fd);
    return -1;
  }

  close(fd);
  return 0;


}


static int set_mac_address(const char *ifname, const unsigned char *mac_addr){
  
  int fd;
  struct ifreq ifr;

  fd = socket(AF_INET,SOCK_DGRAM,0);
  if (fd < 0){
    perror("socket");
    return -1;
  }

  memset(&ifr,0,sizeof(ifr));
  strncpy(ifr.ifr_name,ifname,IFNAMSIZ - 1);
  
  ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
  memcpy(ifr.ifr_hwaddr.sa_data, mac_addr, 6);

  if(ioctl(fd, SIOCSIFHWADDR, &ifr) < 0){
    perror("ioctl(SIOCSIFHWADDR)");
    close(fd);
    return -1;
  }

  close(fd);
  return 0;

}


int main(int argc, char *argv[]){
  if (argc != 3){
    fprintf(stderr,"Usage: %s <interface> <new_mac>\n",argv[0]);
    fprintf(stderr, "Example MAC format: 00:11:22:33:44:55\n");
    return EXIT_FAILURE;
  
  }
  
  const char *ifname = argv[1];
  const char *mac_str = argv[2];

  unsigned char mac_bytes[6];
  int values[6];
  if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x",
               &values[0], &values[1], &values[2],
               &values[3], &values[4], &values[5]) != 6) {
        fprintf(stderr, "Invalid MAC address format.\n");
        return EXIT_FAILURE;
    }

  for (int i = 0; i < 6; i++){mac_bytes[i] = (unsigned char) values[i];}

  //workflow
  // 1) Bring interface down
  if (set_interface_state(ifname, 0) < 0) {
     fprintf(stderr, "Failed to bring interface %s down.\n", ifname);
     return EXIT_FAILURE;
  }

    // 2) Set new MAC address
  if (set_mac_address(ifname, mac_bytes) < 0) {
    fprintf(stderr, "Failed to set MAC address on interface %s.\n", ifname);
      
    set_interface_state(ifname, 1);
    return EXIT_FAILURE;
  }

    // 3) Bring interface up again
  if (set_interface_state(ifname, 1) < 0) {
    fprintf(stderr, "MAC changed, but failed to bring interface %s up.\n", ifname);
    return EXIT_FAILURE;
  }

  printf("Successfully changed MAC of %s to %s\n", ifname, mac_str);
  return EXIT_SUCCESS;
}






