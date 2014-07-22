#include <iostream>

#include "../vm.h"

#define SASM_RUN_VER 0.1

using namespace std;

void print_help();

int main(int argc, char** argv)
{
  if(argc < 2 || string(argv[1]) == "--help" || string(argv[1]) == "-h")
  {
    print_help();
    return 0;
  }

  Sam::VM vm;

  vm.load(string(argv[1]));

  vm.execute();

  return 0;
}

void print_help()
{
  cout << "Sasm-run " << SASM_RUN_VER << "\n"
       "Load and execute sasm-assembled binaries.\n"
       "Usage: sasm-run <filename>\n";
}