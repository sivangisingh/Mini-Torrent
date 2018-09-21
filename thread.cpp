#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream> 
#include <thread> 
using namespace std; 
  
// A dummy function 
void foo(int Z) 
{ 
    for (int i = 0; i < Z; i++) { 
        cout << "Thread using function"
               " pointer as callable\n"; 
    } 
} 
  
// A callable object 
class thread_obj { 
public: 
    void operat(int x) 
    { 
        for (int i = 0; i < x; i++) 
            cout << "Thread using function"
                  " object as  callable\n"; 
    } 
}; 
  
int main() 
{ 
    cout << "Threads 1 and 2 and 3 "
         "operating independently" << endl; 
 
    thread th1(foo, 3); 
    thread th2(&thread_obj::operat, thread_obj(), 3); 
    auto f = [](int x) { 
        for (int i = 0; i < x; i++) 
            cout << "Thread using lambda"
             " expression as callable\n"; 
    }; 
  
    thread th3(f, 3); 
    th1.join(); 
    th2.join(); 
    th3.join(); 
  
    return 0; 
} 
// g++ -std=c++11 -pthread thread.cpp
