/*
 * This simple program shows multiple ways to output a string.
 */

#include "../vm.h"
#include <string>


int main()
{
  Sam::VM vm;

  // Method 1:
  // This method works by pushing each character individually on the stack,
  // then calling out to output the ASCII character code associated with it.
  // Note: This does waste some space, as every memory location is still a full
  // sized integer (16-bit, 32-bit, etc) depending on the platform, even
  // though the characters are only 8-bit

  std::string hello = "hello world\n";
  for (auto it = hello.begin(); it != hello.end(); it++)
  {
    vm.push(*it);
    vm.out();
    vm.pop();
  }


  // Method 2:
  // This method is more difficult to grasp, but is more memory efficient.
  // OUT is capable of outputing multiple characters per integer. This example
  // assumes 32-bit or greater unsigned integers. Each integer holds multiple
  // chars in successive memory locations.

  uint one = 0;
  one = ((int)'h' << 24) | ((int)'e' << 16) | ((int)'l' << 8) | ((int)'l');
  uint two = 0;
  two = ((int)'o' << 24) | ((int)'!' << 16) | ((int)'\n' << 8);

  vm.push(one);
  vm.out();
  vm.push(two);
  vm.out();


  vm.execute();

  return 0;
}
