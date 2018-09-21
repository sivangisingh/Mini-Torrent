#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>
#include <openssl/sha.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string>
using namespace std;
const string tracker1="127.0.0.0:12344";
const string tracker2="127.0.0.1:12345";
string filename(string name){
	int n=name.find(".");
	return name.substr(0,n);
}
int create_mtorrent(int argc,char* argv[]){
// tracker1,tracker2;
	string filen=argv[1];
	string file_name=filename(argv[1]);
  ofstream myfile;
  myfile.open (file_name+".mtorrent");
  myfile << tracker1+"\n";
  myfile << tracker2+"\n";
  myfile << filen+"\n";

 char * buffer;
 size_t result;

 FILE * pFile;
  if (argc < 2)
    {
      cerr << "usage: input_file " << endl;
      return 1;
    }

  ifstream file(argv[1], ifstream::binary);
  pFile = fopen ( argv[1] , "rb" );
  if (not file)
    {
      cerr << "file: " << argv[1] << " failed to open" << endl;
      return 1;
    }
  struct stat filestatus;
  stat(argv[1], &filestatus);
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
        if(chunk == total_chunks - 1){
          this_chunk_size=last_chunk_size;
        }else{
          this_chunk_size=chunk_size;
        }
      buffer = (char*) malloc (sizeof(char)*this_chunk_size);
      result = fread (buffer,1,this_chunk_size,pFile);
      // vector<char> chunk_data(this_chunk_size);
      unsigned char obuf[20];
      // cout << "chunk #" << chunk <<" "<< this_chunk_size<<"  ";
      SHA1((const unsigned char*)buffer,this_chunk_size, obuf);
      int i;
      for (i = 0; i < 20; i++) {
        // printf("%x", obuf[i]);
        myfile << obuf[i];
      }
      free(buffer);
    }
  myfile.close();
  return 1;
}
int main(int argc, char* argv[]){
	// string s="sivangi";
	create_mtorrent(argc,argv);
	return 0;
}