#include <iostream>
using namespace std;
#include "../vm.h"
#include "dryrun.h"

BEGIN_TEST();

Sam::VM vm;

BEFORE_EACH([&] { vm.clear(); });

TEST("push(5)", [&]
{
  vm.push(5);
  vm.execute();
  return vm.peek() == 5;
});

TEST("pop()", [&]
{
  vm.push(1);
  vm.pop();
  vm.execute();
  return true; // The program will segfault if this doesn't work
});

TEST("add()", [&]
{
  vm.push(1);
  vm.push(2);
  vm.add();
  vm.execute();
  return vm.peek() == 3;
});

TEST("sub()", [&]
{
  vm.push(1);
  vm.push(3);
  vm.sub();
  vm.execute();
  return vm.peek() == 2;
});

TEST("mul()", [&]
{
  vm.push(5);
  vm.push(5);
  vm.mul();
  vm.execute();
  return vm.peek() == 25;
});

TEST("div()", [&]
{
  vm.push(2);
  vm.push(10);
  vm.div();
  vm.execute();
  return vm.peek() == 5;
});

TEST("mod()", [&]
{
  vm.push(2);
  vm.push(3);
  vm.mod();
  vm.execute();
  return vm.peek() == 1;
});

TEST("inc()", [&]
{
  vm.push(2);
  vm.inc();
  vm.execute();
  return vm.peek() == 3;
});

TEST("dec()", [&]
{
  vm.push(5);
  vm.dec();
  vm.execute();
  return vm.peek() == 4;
});



END_TEST();
