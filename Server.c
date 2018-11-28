#include <fstream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 10000

int main() {
  printf("%s\n", "FTP Server Running:");
  int Sockfd;
  if ((Sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("cannot create server socket");
    return 0;
  } else
    printf("%s\n", "Socket created.");

  struct sockaddr_in serverAddr;

  memset((char *)&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(7000);

  if (bind(Sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
    perror("bind failed");
    return 0;
  } else
    printf("%s\n", "Bind done.");
  listen(Sockfd, 5);
  int new_socket;
  int addrlen = sizeof(serverAddr);
  if ((new_socket = accept(Sockfd, (struct sockaddr *)&serverAddr,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept error");
    exit(EXIT_FAILURE);
  } else
    printf("%s\n", "Connection accepted.");

  // int new_socket =
  //    accept(Sockfd, (struct sockaddr *)&serverAddr, (socklen_t *)&addrlen);

  // void *recvBuf; // = "ABCDOOO\0";
  unsigned char recvBuf[1024] = {0};
  // int bufLen;
  int val_read = read(new_socket, recvBuf, 1024);
  if (val_read > 0)
    printf("%s\n", "Read.");
  /*if ( < 0) {
    perror("Read failed");
    return 0;
  }*/
  // printf("%d\n", new_socket);
  printf("Client wants %s\n", recvBuf);

  std::ifstream is((const char *)recvBuf, std::ifstream::binary);
  if (is) {
    // get length of file:
    is.seekg(0, is.end);
    unsigned int length = is.tellg();
    is.seekg(0, is.beg);
    // cout << length << endl;

    // allocate memory:
    char *buffer = new char[length];

    // read data as a block:
    is.read(buffer, length);

    is.close();
    // std::cout << buffer << std::endl;
    // std::ofstream os("server_1.jpeg", std::ofstream::binary);

    // os.write((const char *)buffer, length);
    // os.close();
    printf("Data to be sent: %s\n", buffer);
    if (send(new_socket, buffer, sizeof(buffer), 0) < 0) {
      perror("Send failed");
      return 0;
    }
  }

  // else
  // cout << "could'nt open file" << endl;

  close(new_socket);
  close(Sockfd);
  return 0;
}
