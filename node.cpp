#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>
#include <openssl/sha.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <thread> 
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string.h>
using namespace std;

const string tracker1="127.0.0.0:12344";
const string tracker2="127.0.0.1:12345";
class client{
public:
	size_t total_chunks;
	std::vector<char> check;
string filename(string name){
	int n=name.find(".");
	return name.substr(0,n);
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int senddata(int argc, char *argv[],unsigned char *sha)
{
  
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    bzero(buffer,256);
    string am;
    am=argv[3];
    for (int i = 0; i < 20; i++)  
 {  
   printf("%02x", sha[i]);  
 } 
    n = write(sockfd, sha, 20);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n", buffer);
    close(sockfd);
    return 0;
}
void filetaken(int argc,char* argv[]){
	std::vector<unsigned char> hashval;
	hashval=create_mtorrent(argc,argv[3]);
	int num=total_chunks;
	unsigned char buff[hashval.size()],output[20];
	for(int i=0;i<hashval.size();i++){
		buff[i]=hashval[i];
	}
	SHA1((const unsigned char*)buff,hashval.size(), output);
  senddata(argc,argv,output);
}

vector<unsigned char> create_mtorrent(int argc,char *arg){
	std::vector<unsigned char> hashval;
	string filen=arg;
	string file_name=filename(arg);
  ofstream myfile;
  myfile.open (file_name+".mtorrent");
  myfile << tracker1+"\n";
  myfile << tracker2+"\n";
  myfile << filen+"\n";

  char * buffer;
  size_t result;

  FILE * pFile;
  ifstream file(arg, ifstream::binary);
  pFile = fopen ( arg , "rb" );
  struct stat status;
  stat(arg, &status);
  size_t total_size = status.st_size;
  size_t chunk_size = 512 * 1024;
  total_chunks = total_size / chunk_size;
  size_t last_size = total_size % chunk_size;

  if (last_size != 0) 
    {
      ++total_chunks;
    }
  else
    {
      last_size = chunk_size;
    }
    int j=0;
  for (size_t chunk = 0; chunk < total_chunks; ++chunk)
    {
      size_t current_size;
        if(chunk == total_chunks - 1){
          current_size=last_size;
        }else{
          current_size=chunk_size;
        }
      buffer = (char*) malloc (sizeof(char)*current_size);
      result = fread (buffer,1,current_size,pFile);
      unsigned char obuf[20];
      SHA1((const unsigned char*)buffer,current_size, obuf);
      int i;
      for (i = 0; i < 20; i++) {
        hashval.push_back(obuf[i]);
        j++;
        myfile << obuf[i];
      }
      free(buffer);
    }
  myfile.close();
 
  return hashval;
}
};
class server{
public:
  void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int transfer(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     FILE * file_to_recv;
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
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR on binding");

     listen(sockfd,5);

     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     bzero(buffer,256);
     file_to_recv = fopen ("result.pdf","w");
    
    while((n=read(newsockfd,buffer,256))>0){
        fwrite(buffer,sizeof(char),n,file_to_recv);
        bzero(buffer,256);
    }

     close(newsockfd);
     close(sockfd);
     return 0; 
}
};
int main(int argc, char *argv[])
{
	// thread th2(client);
	client c1;
	c1.filetaken(argc,argv);
	return 0;
}# OSassignment
