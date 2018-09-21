#include <openssl/sha.h>
#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string>
using namespace std;
   void ReadHashFile(string name)
{
 FILE * pFile;
 long lSize;
 char * buffer;
 size_t result;

 pFile = fopen ( name.c_str() , "rb" );
 if (pFile==NULL) {fputs ("File error",stderr); return;}

 // obtain file size:
 fseek (pFile , 0 , SEEK_END);
 lSize = ftell (pFile);
 rewind (pFile);

 if(lSize == -1){fputs ("Read Error",stderr);return;}
 buffer = (char*) malloc (sizeof(char)*lSize);
 result = fread (buffer,1,lSize,pFile);
 fclose (pFile);
 unsigned char ibuf[] = "compute sha1";
 unsigned char obuf[20];

 SHA1((const unsigned char*)buffer, lSize, obuf);
 printf("file %s\n", name.c_str());
 int i;
 for (i = 0; i < 20; i++) {
  printf("%x", obuf[i]);
 }
 printf("\n");

 free(buffer);
}
int main()
{  
 //  const unsigned char str[] = "Original String";
 //  unsigned char hash[SHA_DIGEST_LENGTH]; // == 20

 //  SHA1(str, sizeof(str) - 1, hash);

 //  for (int i = 0; i < 20; i++)  
 // {  
 //   printf("%02x", hash[i]);  
 // } 

ReadHashFile("OS_Assignment_1.pdf");

  return 0;
}
//  g++ -o tt trial.cpp -lssl -lcrypto