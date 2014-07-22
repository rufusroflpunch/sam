#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <istream>
#include <ctype.h>
#include <cstdlib>
#include <sstream>
#include "../vm.h"

struct Token
{
  enum token_type
  {
    TOK_IDENT = 1,
    TOK_INT,
    TOK_END_LINE,
    TOK_CHAR,
    TOK_UNKNOWN,
    TOK_EOF
  } type;
  
  uint value;
  std::string str_value;
};

void skip_ws(std::istream &tok_stream)
{
  while (tok_stream.peek() == ' ') tok_stream.ignore(1);
}

/*
 * Algorithm:
 * 1.           Define empty token.
 * 2.           Skip leading whitespace.
 * 3.           Peek ahead to determine next token.
 * 4.           Set the token type.
 * 5.           Get characters until they no longer match.
 * 6.           Set the string value to match the string of the token.
 * 7.           Set the value to match the numeric value (if applicable).
 * 8.           Return the Token object.
 */
Token get_tok(std::istream &tok_stream)
{
  Token tok;
  char tok_c = 0;
  std::string str;
  
  skip_ws(tok_stream); // Skip leading whitespace
  
  if (tok_stream.peek() == '\n')                                // If it's a newline
  {
    tok_stream.get(tok_c);
    tok.type = Token::TOK_END_LINE;
    tok.str_value = "\n";
    tok.value = '\n';
  }
  else if (isalpha(tok_stream.peek()))                          // If it's alphabetic characters
  {
    tok.type = Token::TOK_IDENT;
    while (isalnum(tok_stream.peek()) && tok_stream.good())
    {
      tok_stream.get(tok_c);
      str += tok_c;
    }
    tok.str_value = str;
  }
  else if (isdigit(tok_stream.peek()))                          // If it's a series of numbers
  {
    tok.type = Token::TOK_INT;
    while (isdigit(tok_stream.peek()) && tok_stream.good())
    {
      tok_stream.get(tok_c);
      str += tok_c;
    }
    tok.str_value = str;
    tok.value = (uint)atoi(str.c_str());
  }
  else if (tok_stream.peek() == '\'' && tok_stream.good())       // Extract the character
  {
    tok.type = Token::TOK_CHAR;
    tok_stream.get(tok_c);                                       // Discard the first apostrophe
    tok_stream.get(tok_c);
    tok.str_value += tok_c;
    tok.value = (uint)tok_c;
    tok_stream.get(tok_c);
    if (tok_c != '\'') tok.type = Token::TOK_UNKNOWN;            // If char doesn't match 'x' then return unknown token
  }
  else if (tok_stream.eof())
  {
    tok.type = Token::TOK_EOF;
    tok.str_value = "EOF";
  }
  else
  {
    tok_stream.get(tok_c);
    tok.type = Token::TOK_UNKNOWN;                           // If it doesn't match.
    tok.str_value += tok_c;
  }
  
  return tok;
}

#endif