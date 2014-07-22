#include <iostream>
#include <sstream>
#include "../vm.h"
#include "tokenizer.h"
#include "parser.h"

using namespace std;

void print_help();

int main(int argc, char **argv)
{
    
  string test_program =
    "push 'a'\n"
    "out\n"
    "inc\n"
    "jlt 'z' dd\n"
    "out\n";
  istringstream istr(test_program);
  Sam::VM vm;
  Error_State err;
  
  if (!parse(istr, vm, err))
    cout << "Error (" << err.line_no << "): " << err.err_msg << endl;
  else vm.execute();
  
  return 0;
}



void print_help()
{
  cout << "Usage: "
    "sasm <filename>\n"
    "Parse and execute a Sam assembler file to output readable binary.\n";
}