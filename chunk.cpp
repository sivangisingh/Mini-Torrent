#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream> 
#include <vector>

using namespace std;
vector<vector<int> > getFileNumbers(string FileName) {
    vector<vector<int> > Numbers;            //Stores all the file's numbers
    ifstream infile(FileName.c_str());       
    string S;                           
    int a;
    while (getline(infile, S)) {        
        vector<int> Line;                    //Stores each line's numbers
        stringstream Str(S);         
        while (Str >> a){                  //Extracts numbers from 'Stream'
            Line.push_back(a);  
          std::cout<< a;
            }             //Each number is added to Line
        Numbers.push_back(Line);             //Each Line is added to Numbers
    }
    return Numbers;
}
int main(int argc, char* argv[])
{
   // std::stringstream ss;
ifstream infile("seederlist.txt");       
    string S;                           
    string a;
    while (getline(infile, S)){
      stringstream str(S);
      str >> a;
        cout<<a<<" \n";
    }

  // ifstream infile("seederlist.txt"); 
  std::vector<std::vector<int> > Numbers = getFileNumbers("chunk.cpp");
// for (unsigned int i = 0; i < Numbers[0].size(); ++i)
//     std::cout << Numbers[0][i] << ' '; 
  // cout<<"hi";
  // if (argc < 2)
  //   {
  //     cerr << "usage: input_file [chunk_size]" << endl;
  //     return 1;
  //   }

  // ifstream file(argv[1], ifstream::binary);
  // if (not file)
  //   {
  //     cerr << "file: " << argv[1] << " failed to open" << endl;
  //     return 1;
  //   }
  // struct stat filestatus;
  // stat(argv[1], &filestatus);
  // size_t total_size = filestatus.st_size;
  // size_t chunk_size = 512 * 1024;
  // size_t total_chunks = total_size / chunk_size;
  // size_t last_chunk_size = total_size % chunk_size;

  // if (last_chunk_size != 0)
  //   {
  //     ++total_chunks;
  //   }
  // else 
  //   {
  //     last_chunk_size = chunk_size;
  //   }
  // for (size_t chunk = 0; chunk < total_chunks; ++chunk)
  //   {
  //     size_t this_chunk_size;
  //     if(chunk==total_chunks-1){
  //       this_chunk_size=last_chunk_size;
  //     }else{
  //       this_chunk_size=chunk_size;
  //     }
  //     vector<char> chunk_data(this_chunk_size);
  //     file.read(&chunk_data[0],this_chunk_size);
  //     cout << "chunk #" << chunk <<" "<< this_chunk_size<< endl;
      // for (const auto c : chunk_data)  I like my C++11 extensions 
      //   {
      //     cout << c;
      //   }
      // cout << endl;
    // }
}