#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<iostream>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;
void error(const char *msg)
{
    perror(msg);
    return;
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int ch;
    FILE * file;
    char toSEND[1];
    char remoteFILE[4096];
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return 0;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    file = fopen ("OS_Assignment_1.pdf","r");
    struct stat filestatus;
  stat("OS_Assignment_1.pdf", &filestatus);
  size_t total_size = filestatus.st_size;
  size_t chunk_size = 512 * 1024;
  size_t total_chunks = total_size / chunk_size;
  size_t last_chunk_size = total_size % chunk_size;

  if (last_chunk_size != 0)
    {
      ++total_chunks;
    }
  else 
    {
      last_chunk_size = chunk_size;
    }
  for (size_t chunk = 0; chunk < total_chunks; ++chunk)
    {
      size_t this_chunk_size;
      if(chunk==total_chunks-1){
        this_chunk_size=last_chunk_size;
      }else{
        this_chunk_size=chunk_size;
      }
      char buffer[this_chunk_size];
      fread(buffer,sizeof(char),this_chunk_size,file);
      write(sockfd,buffer,this_chunk_size);
      bzero(buffer,this_chunk_size);
    }
    close(sockfd);
    return 0;
}