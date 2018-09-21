#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include<iostream>
#include<vector>
#include <map>
#include<fstream>
using namespace std;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     // std::map<unsigned char *,std::vector<string>> seederlist;
     socklen_t clilen;
     char buffer[256];
     unsigned char buff[20];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;  
     serv_addr.sin_addr.s_addr = INADDR_ANY;  
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     ofstream myfile;
  myfile.open ("seederlist.txt");
  int inum=0;
          while(inum<2){
          	inum++;
     listen(sockfd,5);
	 clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     bzero(buffer,256);
     bzero(buff,20);
     n = read(newsockfd,buff,20);
     unsigned char sha[20];
     // sha=buff;
     for (int i = 0; i < 20; i++)  
 		{  
 			sha[i]=buff[i];
   		printf("%02x",sha[i]);
 		}
  		myfile << sha;
  		myfile << ".";
  		myfile << cli_addr.sin_addr.s_addr;  		
 // printf(" %s\n",name.c_str());
      }
     close(newsockfd);
     close(sockfd);
     return 0; 
}
