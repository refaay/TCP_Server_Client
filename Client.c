#include <fstream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int Sockfd;
  if ((Sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("cannot create server socket");
    return 0;
  }

  struct sockaddr_in serverAddr;

  memset((char *)&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(7000);

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
  printf("Data received %s\n", recvBuf);
  std::ofstream os((const char *)msg, std::ofstream::binary);

  os.write((const char *)recvBuf, val_read);
  os.close();

  close(Sockfd);
  return 0;
}
