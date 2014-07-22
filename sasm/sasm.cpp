#include <iostream>
#include <fstream>
#include <sstream>
#include "../vm.h"
#include "tokenizer.h"
#include "parser.h"

#define SASM_VER 0.1

using namespace std;

void print_help();

int main(int argc, char** argv)
{
  ifstream istr;
  string out_file = "";
  Sam::VM vm;
  Error_State err;

  if(argc < 2 || string(argv[1]) == "--help" || string(argv[1]) == "-h")
  {
    print_help();
    return 1;
  }

  if(string(argv[1]) == "-r")
  {
    istr.open(argv[2]);
  }
  else if(string(argv[1]) == "-o")
  {
    istr.open(argv[3]);
    out_file = argv[2];
  }
  else
  {
    istr.open(argv[1]);
    out_file = "a.out";
  }

  if(!istr)
  {
    cout << "Unable to open file." << endl;
    return 1;
  }


  if(!parse(istr, vm, err))
  {
    cout << "Error (" << err.line_no << "): " << err.err_msg << endl;
    return 1;
  }

  if(!out_file.empty())            // If they elected to save it as a file.
  {
    vm.save(out_file);
  }
  else
    vm.execute();               // Execute if they didn't

  return 0;
}



void print_help()
{
  cout << "Sasm " << SASM_VER << "\n"
       "Usage: sasm [options] <filename>\n"
       "Parse and execute a Sam assembler file to output readable binary.\n\n"
       "Options: \n"
       "-h, --help\t\tPrint this help screen.\n"
       "-r <file>\t\tAssemble and run the sasm file.\n"
       "-o <file>\t\tAssemble the Sasm file and output a binary file.\n"
       "-b\t\t\tLoad and run a binary file.\n";
}