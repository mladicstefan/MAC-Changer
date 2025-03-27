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
  else {ifr.ifr_flags |= ~IFF_UP;}

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


}


