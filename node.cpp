#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream> 
#include <thread>
#include <pthread.h>
#include <vector>
#include <openssl/bio.h>
#include <openssl/sha.h>
#include <openssl/buffer.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string.h>
using namespace std;

const string tracker1="127.0.0.2:12344";
const string tracker2="127.0.0.1:12345";
string getip(string name){
  int n=name.find(":");
  return name.substr(0,n);
}
string getport(string name){
  int n=name.find(":");
  return name.substr(n+1);
}
string filename(string name){
  int n=name.find(".");
  return name.substr(0,n);
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
class client{
public:
	size_t total_chunks;
	std::vector<char> check;
int sendmtorrent(int argc, char *argv1,char *argv2,string path,unsigned char *sha,string com){
    // cout<<"fun3\n";
    int sockfd, portno, n;
    // string com="share";
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    portno = atoi(getport(argv2).c_str());
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(getip(argv1).c_str());
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
    am=path;
    // cout<<"sending command"<<com<<"\n";
    write(sockfd,com.c_str(),20);
    for(int i=0;i<20;i++){
      printf("%x",sha[i]);
    }
    // cout<<"sending sha\n";
    n = write(sockfd, sha, 20);
    if (n < 0) 
         error("ERROR reading from socket");
       printf("arg1 %s\n", argv1);
       // string cl;
       // cl=argv1;
       // printf("%s\n", cl);
       write(sockfd,argv1,40);
    // printf("%s\n", buffer);
    close(sockfd);
    return 0;
}
void filetaken(int argc, char *argv1,char *argv2,string path,string mt_name,string com){
  // cout<<"fun1\n";
	std::vector<unsigned char> hashval;
	hashval=create_mtorrent(argc,path,mt_name);
	int num=total_chunks;
	unsigned char buff[hashval.size()],output[20];
	for(int i=0;i<hashval.size();i++){
		buff[i]=hashval[i];
	}
	SHA1((const unsigned char*)buff,hashval.size(), output);
  // cout<<"fun1 end";
  sendmtorrent(argc,argv1,argv2,path,output,com);
}

vector<unsigned char> create_mtorrent(int argc,string path,string mt_name){
  // cout<<"fun2\n";
	std::vector<unsigned char> hashval;
  ofstream myfile;
  myfile.open (mt_name);
  myfile << tracker1+"\n";
  myfile << tracker2+"\n";
  myfile << path+"\n";

  char * buffer;
  size_t result;

  FILE * pFile;
  ifstream file(path, ifstream::binary);
  pFile = fopen ( path.c_str() , "rb" );
  struct stat status;
  stat(path.c_str(), &status);
  size_t total_size = status.st_size;
  size_t chunk_size = 512 * 1024;
  total_chunks = total_size / chunk_size;
  size_t last_size = total_size % chunk_size;

  if (last_size != 0){
      ++total_chunks;
    }else{
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
  fclose (pFile);
  return hashval;
}
void file_transfer(){
  return;
}
};
class server{
public:
  void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int transfer(int argc, char *argv1,char *argv2,char *path, string file){
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[512 * 1024];
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
     portno = atoi(argv1);
     serv_addr.sin_family = AF_INET;  
     serv_addr.sin_addr.s_addr = INADDR_ANY;  
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
     bzero(buffer,512 * 1024);
     file_to_recv = fopen (file.c_str(),"w");
    while((n=read(newsockfd,buffer,512 * 1024))>0){
        fwrite(buffer,sizeof(char),n,file_to_recv);
        bzero(buffer,512 * 1024);
    }
     close(newsockfd);
     close(sockfd);
     return 0; 
}
};
int main(int argc, char *argv[])
{
  cout<<"enter command:\n";
  string s;
  cin>>s;
  if(s=="share"){
    string path;
    string mt_name;
    cin>>path>>mt_name;
    client c1;
    // pthread_t thread;

    // c1.filetaken(argc,argv[1],argv[2],path,mt_name,s);
    std::thread t1(&client::filetaken, client(),argc,argv[1],argv[2],path,mt_name,s);
    t1.join();
  }
  if(s=="get"){
    string mt_path,dest_path;
    cin>>mt_path>>dest_path;
    client c1;
    c1.filetaken(argc,argv[1],argv[2],mt_path,dest_path,s);
    // std::thread t2(&client::)
  }
	return 0;
}