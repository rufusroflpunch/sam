#Sam
###About

Sam is a stack-based virtual machine written in standards-compliant C++. It's written in a way as to be entirely platform independent.

The basic package is three simple files: `bytecode.h`, the file that defines the various base instructions of the virtual machine. `vm.h` and `vm.cpp` define the virtual machine logic. While you certainly can go through the trouble of compiling a separate binary if you like, it would probably just be easer to include these as part of your project.

###Usage
Let's examine a piece of code:

```
#include "vm.h"

int main()
{
    Sam::VM vm;
    vm.trace = false;
    
    vm.push(65);
    vm.out();
    vm.dbg();
    vm.halt();
    vm.out();
    
    vm.execute();
    return 0;
}
```

First things, first, we include the necessary header file, `vm.h`. The imported class and methods are inside the `Sam` namespace. Next, inside the main execution method, we declare an instance of the virtual machine: `Sam::VM vm`.The following line `vm.trace=false` sets ths debugging trace to false. Strictly, this isn't necessary, because it is off by default. We will see more of that in a moment.

The next section shows how to attach instructions to the virtual machine. This is a good time to talk about how instructions are stored inside the virtual machine. The instructions are stored as an array of `unsigned int`. This is coded in a platform independent way, so it doesn't matter whether your compilers default integer is 16-bit, 32-bit, etc. Each of these methods is equivalent to an opcode of the virtual machine. Something important to note is that calling these methods doesn't actually execute the instruction, it only adds it to the virtual machine for execution in the order it is called.

For example, calling `vm.push(65)` adds two values to the instruction set. First, it adds the opcode for the **PUSH** instruction and the integer value **65**. Note, while this is only one *instruction*, it takes up two spots in instruction memory. This will be important when jumping around in the code for things like looping.

What does the **PUSH** instruction actually do? Why, it pushes the value **65** onto the stack, of course! All operations in the virtual machine are completed by pushing, popping and modifying values on the stack.

The next instruction, `vm.out()` adds the **OUT** instruction to the machine. The OUT instruction takes no parameters, thus only occupies one place in instruction memory. The **OUT** instruction tells the VM to output the ASCII character of the associated integer on the stack. The ASCII character of **65** just happens to be **A**.

The next instruction `vm.dbg()` outputs the actual integer value of the stack (in hexadecimal), so this would output **41** instead of **A**.

`vm.halt()` adds the **HALT** instruction, which ceases execution of the application. Which means that, even though we add an instruction after **HALT** it will never be executed if we reach the **HALT** first. Later, you will see ways to get around this by jumping.

The final piece of relevant code is the method `vm.execute()` which actually executes the instructions as we added them.

Not to terrily complicated, is it?

For more examples, look under the **samples** folder.

###Program Memory

Now that you understand the stack and the instruction memory, the third thing to understand is program memory. This behave like RAM in your computer. It's temporary storage for use by your application. It begins at memory location 0, and (unlike computer RAM) expands as necessary for your application. There are four instructions that use program memory: `LOAD`, `STORE`, `SLOAD` and `SSTORE`. See more about them below.

###Tracing

Sometimes, it can be difficult to debug your application. That's what tracing is for! By setting trace to true, the application will output some useful debugging information to stdout. The above example application will provide the following output:

```
0	: 1     Stack: N/A  Out: 
2	: 16    Stack: 65   Out: A
3	: 18    Stack: 65   Out: 41
4	: 17    Stack: 65   Out: 

```

The first column is the instruction number. For instance, `PUSH 65` is the first instruction, and it has one argument, so it uses spots 0 and 1. That's why the next instruction begins at instruction spot 2. The next column is the opcode number. At this point, it's not terribly useful unless you know the actual integer number of the opcode. Column 3 shows the current top value on the stack. At instruction 0, there has been no value actually pushed yet, so it says **N/A**. However, you can see at the next step, it was pushed show the stack shows **65**. The final column shows whatever is output by that instruction.

###API

Virtual machines are instantiated as objects of class Sam::VM. They are completely independent. There is no global state, so you may instantiate more than one VM at once if needed. See the example application `name.cpp` under the `samples` folder for an example of how this might be useful.

Each virtual machine contains the state variable `error_state` which is useful in the save a load functions. Error state can contain the following states:

* ERR_NONE: Default
* ERR_INVALID_INS: This is set when attempting to load an invalid instruction from file.
* ERR_OPEN_FILE: Error opening file with load or save.
* ERR_BYTECODE_VER: The file to open was written in a different bytecode set.
* ERR_INT_SIZE: The file uses a different base int size than your machine.

The virtual machines have several member functions:

`void execute()`  
This function is used to execute the virtual machine at the currect instruction position until HALT is reached, or the end of the instructions are reached. This **can** be executed multiple times per VM.

`bool load(std::string filename)`  
Supply the string **filename** to load the instruction set from a binary file. Returns true if successful. False if unsuccessful, and sets the `error_state`. If the file doesn't match the current bytecode version or the correct integer size (stored in a header at the beginning of the file), then it will return an error.

`bool save(std::string filename)`  
Suply the string **filename** to save the current instruction set to a binary file.

`void clear()`  
Clear's the virtual machine completely. This includes the stack, program memory, etc.

`void reset()`  
Reboots the virtual machine back to default position with the current instruction set.

###Instruction Set
Time for the juicy stuff! Here are all of the available instructions:

####PUSH
`vm.push(int val)`  
Push **val**  onto the stack.

####POP
`vm.pop()`  
Pop the top value from the stack.

####ADD
`vm.add()`  
Pops the top two values off the stack, adds them and pushes the new value onto the stack.

####SUB
`vm.sub()`  
Pops the top two values off the stack, subtracts them and pushes the new value onto the stack. Note, this is done in the order they are popped. So, if you want to do (4 - 3), you actually want to push the three onto the stack first, then the four.

####MUL
`vm.mul()`  
Pops the top two values off the stack, multiplies them and pushes the new value onto the stack.

####DIV
`vm.div()`  
Pops the top two values off the stack, divides them and pushes the new value onto the stack. Note, this is done in the order they are popped. So, if you want to do (4 / 3), you actually want to push the three onto the stack first, then the four.

####MOD
`vm.mod()`  
Pops the top two values off the stack, divides them and pushes the remainder onto the stack. Note, this is done in the order they are popped. So, if you want to do (4 % 3), you actually want to push the three onto the stack first, then the four.

####INC
`vm.inc()`  
Increments the value on top of the stack.

####DEC
`vm.dec()`  
Decrements the value on top of the stack.

####JGE
`vm.jge(int val, int addr)`  
If the value on the stack is greater than or equal to **val**, then jump to the address **addr**. The address is a pointer to the instruction in code, so the very first instruction is at 0. Remember, instruction arguments also use up values in the instruction space, so be sure you don't jump to the an argument value instead of an instruction.

####JGT
`vm.jgt(int val, int addr)`  
If the value on the stack is greater than  **val**, then jump to the address **addr**. The address is a pointer to the instruction in code, so the very first instruction is at 0. Remember, instruction arguments also use up values in the instruction space, so be sure you don't jump to the an argument value instead of an instruction.

####JLE
`vm.jle(int val, int addr)`  
If the value on the stack is less than or equal to **val**, then jump to the address **addr**. The address is a pointer to the instruction in code, so the very first instruction is at 0. Remember, instruction arguments also use up values in the instruction space, so be sure you don't jump to the an argument value instead of an instruction.

####JLT
`vm.jlt(int val, int addr)`  
If the value on the stack is less than **val**, then jump to the address **addr**. The address is a pointer to the instruction in code, so the very first instruction is at 0. Remember, instruction arguments also use up values in the instruction space, so be sure you don't jump to the an argument value instead of an instruction.

####JEQ
`vm.jeq(int val, int addr)`  
If the value on the stack is greater than or equal to **val**, then jump to the address **addr**. The address is a pointer to the instruction in code, so the very first instruction is at 0. Remember, instruction arguments also use up values in the instruction space, so be sure you don't jump to the an argument value instead of an instruction.

####JMP
`vm.jge(int addr)`  
Jump to the address **addr**. The address is a pointer to the instruction in code, so the very first instruction is at 0. Remember, instruction arguments also use up values in the instruction space, so be sure you don't jump to the an argument value instead of an instruction.

####OUT
`vm.out()`  
Output the ASCII character represented by the integer on the stack. Important note: Assume for a moment that your compiler represents an unsigned integer as 32-bits. However, an ASCII character is only 8 bits, which means that representing an 8 bit value with 32-bits of memory can be a waste of space. If that is a concern, note that it is possible to store multiple characters in an integer value, and one **OUT** instruction can output all of them. For instance, if your `int` is 0x41 it will output **A**. However, if you make it 0x41424344 it will output **ABCD**. It's not all or nothing, either; 0x41424300 will output **ABC**. If you don't feel like doing all of the extra work to pack to pack multiple characters into one integer, feel free to ignore this.

####IN
`vm.in(int size, int addr)`  
This takes input from stdin. **size** represents the number of memory to use. It reads them and stores them in program memory at **addr**. Note: The size is in integer, not characters. The **IN** instruction returns them as packed integers. So with a compiler that has 32-bit integers, it will actually store 4 8-bit characters inside one integer. Additionally, just as string in C are delimited by a null character, string in the Sam VM are delimited by a null integer (0x00, 0x0000, 0x000000, etc. depending on platform). So the actual size of the string in program memory will be `size + 1`.

####DBG
`vm.dbg()`  
This outputs the integer value of the top of the stack in hexadecimal.

####STORE
`vm.store(int addr)`  
This pops the integer value on the stack and stores it at the given memory location at address **addr**.

####LOAD
`vm.load(int addr)`  
This loads the integer value from **addr** in program memory and pushes it onto the stack.

####SSTORE
`vm.sstore()`  
This pops the first value off the stack and uses the integer as an address in program. It pops the second value off the stack to use as the value to store at the address.

####SLOAD
`vm.sload()`  
This pops the top integer off the stack and uses it as an address to load from and pushes the new value onto the stack.

####HALT
`vm.halt()`  
Ceases execution of the application.

### The Future

I would like to create a separate assembler and assembly language. Potentially, it would be a nice learning excercise to implement a higher level programming language on top of the virtual machine.