#include <iostream>

#include "vm.h"

using namespace std;

int main(int argc, char** argv)
{
  Sam::VM vm;
  vm.trace = false;
  
  vm.push(0);
  vm.inc();
  vm.dbg();
  vm.jle(0xff,2);
  vm.halt();
  
//   vm.execute();
  vm.save(string("/home/ryan/hello.bin"));
  
  Sam::VM vm2;
  vm2.trace = true;
  vm2.load(string("/home/ryan/hello.bin"));
  
//   cout << vm2.error_state;
    vm2.execute();
  
  string test;
  cin >> test;

  return 0;
}