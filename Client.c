#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
static void makeDestSA(struct sockaddr_in *sa, char *hostname, int port) {
  struct hostent *host;
  sa->sin_family = AF_INET;
  if ((host = gethostbyname(hostname)) == NULL) {
    // printf("Unknown host name\n");
    exit(-1);
  }
  sa->sin_addr = *(struct in_addr *)(host->h_addr);
  sa->sin_port = htons(port);
}

int main() {
  int Sockfd;
  if ((Sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("cannot create server socket");
    return 0;
  }

  struct sockaddr_in serverAddr;

  std::string dos_ip_string; // = "10.7.57.141";
  std::cout << "Enter Server IP Address:\n";
  std::cin >> dos_ip_string;
  char *dos_ip;
  dos_ip = new char[dos_ip_string.size() + 1];
  dos_ip_string.copy(dos_ip, dos_ip_string.size() + 1);
  dos_ip[dos_ip_string.size()] = '\0';
  memset((char *)&serverAddr, 0, sizeof(serverAddr));
  makeDestSA(&(serverAddr), dos_ip, 8000); // make dos socket

  /*
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl();
  serverAddr.sin_port = htons(7000);
*/
  if (connect(Sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("Connect failed");
    return 0;
  }

  // listen(Sockfd, 5);
  // int new_socket =
  //  accept(Sockfd, (struct sockaddr *)&serverAddr, (socklen_t *)&serverAddr);
  // char *msg = {0}; // = "ABCDOOO";
  unsigned char msg[1024] = {0};
  printf("%s\n", "Please, enter the name of the file to download:");
  scanf("%s", msg);
  // printf("%s\n", msg);

  if (send(Sockfd, msg, sizeof(msg), 0) < 0) {
    perror("Send failed");
    return 0;
  }
  printf("%s sent!\n", msg);
  unsigned char recvBuf[1024] = {0};
  int val_read = read(Sockfd, recvBuf, 1024);
  printf("Data received: %s\n", recvBuf);
  std::ofstream os((const char *)msg, std::ofstream::binary);

  os.write((const char *)recvBuf, val_read);
  os.close();

  close(Sockfd);
  return 0;
}
