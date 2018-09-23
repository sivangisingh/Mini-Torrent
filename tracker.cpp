#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <iostream> 
#include <thread>
#include<pthread.h>
#include <sstream> 
#include <vector>
#include<arpa/inet.h>
#include <map>
#include<fstream>
using namespace std;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
std::vector<string> seederlist(char *sha){
	ifstream fin;
	fin.open("seederlist.txt");     
    string S;                           
    string a;
    std::vector<string> v;
    // printf("sha %s\n", sha);
    while (getline(fin, S)){
      stringstream str(S);
      str >> a;
      // printf(" a %s\nsha %s", a, sha);
      if(a==sha){
      	string cli_names;
      	str>>cli_names;
      	// printf("cli_names%s\n", cli_names);
      	v.push_back(cli_names);
      }
    }
    // close("seederlist,txt");
    return v;
}
void share(int argc, char *argv[]){
	int sockfd, newsockfd, portno;
     socklen_t clilen;
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
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
         int num=0;
         
         unsigned char buff[20];
     while(num<2){
     	num++;
     listen(sockfd,5);
	 clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	 char buffer[20];
	 bzero(buffer,20);
	 printf("%d\n",num);
	 read(newsockfd,buffer,20);
	 string command;
	 command=string(buffer);
	 if(command=="share"){
	 	// cout<<"command recieved\n";
	 // 	listen(sockfd,5);
	 // clilen = sizeof(cli_addr);
  //    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	 	char mdString[41];
     bzero(buff,20);
     read(newsockfd,buff,20);
     // cout<<"buff recieved\n";
    //  ofstream myfile;
  	 // myfile.open ("seederlist.txt");
  	 	FILE * myfile;
  	// // ifstream file(path, ifstream::binary);
  		myfile = fopen ("seederlist.txt" , "a" );
        for (int i = 0; i < 20; i++)
            sprintf(&mdString[i * 2], "%02x",buff[i]);
 		char *ipstr = new char[16];
		strcpy(ipstr,inet_ntoa(cli_addr.sin_addr));
		// cout<<"printing in file:";
		fprintf(myfile, "%s ", mdString);
  		// myfile << mdString;
  		// myfile << "  ";
  		char buffer2[40];
  		bzero(buffer2,40);
     	read(newsockfd,buffer2,40);
     	printf("%s\n",buffer2);
  		fprintf(myfile, "%s\n",buffer2);
  		// myfile << ipstr;
  		// fprintf( myfile, ":%s\n", cli_addr.sin_port);
  		// myfile << ":" <<cli_addr.sin_port;
  		// myfile << "\n";
  		printf("%s\n",mdString);
  		fclose(myfile);
  		// cout << mdString;
  		// cout << "  ";
  		// cout << ipstr;
  		// cout << ":" <<cli_addr.sin_port;
  		// cout << "\n";
	 }else if(command=="get"){
	 // 	listen(sockfd,5);
	 	// printf("getting data\n");
	 // clilen = sizeof(cli_addr);
  //    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	 	char mString[41];
     bzero(buff,20);
     read(newsockfd,buff,20);
        for (int i = 0; i < 20; i++)
            sprintf(&mString[i * 2], "%02x",buff[i]);

		std::vector<string> v;
	v=seederlist(mString);
	// printf("clients \n");

	for(auto it=v.begin();it!=v.end();it++)
		cout<<*it<<"\n";
	 }else if(command=="remove"){
	 // 	ifstream fin;
		// fin.open("seederlist.txt");
	 // 	while (getline(fin,line))
		// {
  //   	line.replace(line.find(deleteline),deleteline.length(),"");
  //   	temp << line << endl;
		// }
	 }
      }
     close(newsockfd);
     close(sockfd);
}

int main(int argc, char *argv[])
{
     share(argc,argv);
	// std::vector<string> v;
	// v=seederlist("bc4234393c9450b7c9ce92defc0e836f159c45bd");
	// for(auto it=v.begin();it!=v.end();it++)
	// 	cout<<*it<<"\n";
     return 0; 
}
