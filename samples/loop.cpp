/*
 * This demonstrates a simple loop. It increments the number on the stack until it
 * is >= 100, then outputs it as an ASCII character and quits.
 */


#include "../vm.h"

int main()
{
  Sam::VM vm;                 // Instantiate the virtual machine.
  
  vm.push(0);                 // 0, 1:    Push 0 onto the stack.
  vm.inc();                   // 2:       Increment the stack value by 1.
  vm.jle(100,2);              // 3, 4, 5: Jump to instruction 2 (INC) until the stack
                              //          value is >= 100.
  vm.out();                   // 6:       Output the ascii character for the stack value.
  vm.halt();                  // 7:       End the program.

  vm.execute();               // Execute the machine.

  return 0;
}
