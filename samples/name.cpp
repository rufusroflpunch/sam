/*
 * This sample application shows how to to use IN, which stores a
 * string (packed into integers) into program memory. This also
 * demonstrates that the machines are individual and self-contained
 * by creating two independent machines (output and input).
 */

#include "../vm.h"

int main()
{
  Sam::VM output;
  Sam::VM input;
  
  // You can see here, there is a separate VM which I have dedicated to output.
  // When looping over a string to output (rather than writing each instruction
  // by hand) I am adding more instructions to the code. Rather than keeping
  // track of the count in my head, I just dedicated a separate machine for
  // output.
  std::string question = "What is your name? ";
  for (auto c : question)
  {
    output.push(c);
    output.out();
    output.pop();
  }
  output.execute();                           // Output the code.

  input.in(10, 0);                            // 0 - 2: Input the name from the user.
                                              // Resrerve up to 10 integers of space,
                                              // store in memory block 0.
  input.execute();

  std::string answer = "Your name is ";
  for (auto c : answer)
  {
    output.push(c);
    output.out();
    output.pop();
  }
  output.execute();


  input.push(0);                  // 3 - 4: This is the memory counter.
  input.store(11);                // 5 - 6: Store the memory counter at loc. 11.
  input.load(11);                 // 7 - 8: Storing it pops it, so reload it.
  input.sload();                  // 9:     Load the memory location of stack value.
  input.jeq(0, 20);               // 10-12: If the current integer == 0, just to 20 (HALT)
                                  //        NOTE: When inputing using IN, the machine
                                  //        delimits the end of the string with a
                                  //        NULL integer. Hence why we HALT on 0.
  input.out();                    // 13:    Output the character to screen.
  input.pop();                    // 14:    Pop the character off the stack.
  input.load(11);                 // 15-16: Load the counter from memory.
  input.inc();                    // 17:    Increment the counter so it points to next char.
  input.jmp(5);                   // 18-19: Jump the instruction code 5 to loop again.
  input.halt();                   // 20:    End.
  input.execute();

  output.push('\n');
  output.out();
  output.execute();

  return 0;
}
