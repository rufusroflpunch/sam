#ifndef PARSER_H
#define PARSER_H

#include "../vm.h"
#include "tokenizer.h"
#include <iostream>
#include <sstream>


struct Error_State
{
  int line_no;
  std::string err_msg;
};

/*
 * Algorithm:
 * 1. Check the token.
 * 2. Parse the token, then check for expected arguments/follow-up tokens.
 * 3. If the follow up token are present, execute the necessary VM api command.
 * 4. Otherwise, set the error state and return early (false for unsuccessful).
 * 5. Make sure the line or file ends where expected.
 * 6. Get the token for the next round.
 * 7. Increment the line number.
 */
bool parse(std::istream &istr, Sam::VM &vm, Error_State &err)
{
  err.line_no = 1;
  Token tok = get_tok(istr);                    // Get initial token
  
  while (istr.good())
  {
    if (tok.type == Token::TOK_IDENT)
    {
      if (tok.str_value == "push")
      {
        Token next_tok = get_tok(istr);
        if (next_tok.type != Token::TOK_INT && next_tok.type != Token::TOK_CHAR)
        {
          err.err_msg = "Expected integer or char. Found: ";
          err.err_msg += next_tok.str_value;
          return false;
        }
        else vm.push(next_tok.value);
      }
      else if (tok.str_value == "pop")
        vm.pop();
      else if (tok.str_value == "add")
        vm.add();
      else if (tok.str_value == "sub")
        vm.sub();
      else if (tok.str_value == "mul")
        vm.mul();
      else if (tok.str_value == "div")
        vm.div();
      else if (tok.str_value == "mod")
        vm.mod();
      else if (tok.str_value == "inc")
        vm.inc();
      else if (tok.str_value == "dec")
        vm.dec();
      else if (tok.str_value == "out")
        vm.out();
      else if (tok.str_value == "dbg")
        vm.dbg();
      else if (tok.str_value == "halt")
        vm.halt();
    }
    else if (tok.type == Token::TOK_UNKNOWN)
    {
      err.err_msg = "Unknown token found: ";
      err.err_msg += tok.str_value;
      return false;
    }
    
    tok = get_tok(istr);                        // Expecting an EOL or EOF here
    if (tok.type != Token::TOK_END_LINE && tok.type != Token::TOK_EOF)
    {
      err.err_msg = "Expected EOL or EOF.";
      return false;
    }
    
    tok = get_tok(istr);                        // Prepare tok for next round
    err.line_no++;                              // Increment for next line.
    
  }
  
  return true;                                  // Return successful parse
}

#endif