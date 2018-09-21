#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 2)
    {
      cerr << "usage: input_file [chunk_size]" << endl;
      return 1;
    }

  ifstream file(argv[1], ifstream::binary);
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
      if(chunk==total_chunks-1){
        this_chunk_size=last_chunk_size;
      }else{
        this_chunk_size=chunk_size;
      }
      vector<char> chunk_data(this_chunk_size);
      file.read(&chunk_data[0],this_chunk_size);
      cout << "chunk #" << chunk <<" "<< this_chunk_size<< endl;
      // for (const auto c : chunk_data)  I like my C++11 extensions 
      //   {
      //     cout << c;
      //   }
      // cout << endl;
    }
}