// Cpp include(s)
#include <vector>

// Local inlude(s)
#include <Parser.hpp>
#include <RegEx.hpp>

Parser::Parser(std::string to_parse) { 
  RegEx id("[a-z]([A-Z]|[1-2])*");
  RegEx num("[1-9][0-9]*");
  RegEx binop("+|-|*|/");
  RegEx eq("=");
  RegEx print("print");
  RegEx espace(" *");
  
  std::vector< std::pair< uint64_t, uint64_t > > espaces = espace.full_match(to_parse);
  for(uint64_t i = espaces.size(); i > 0; i--) {
    auto rep = espaces[i - 1];
    to_parse.replace(rep.first, rep.second, " ");
  }

  std::cout << to_parse << "\n";
  std::cout << binop.first_match(to_parse) << "\n";
  std::cout << to_parse[binop.first_match(to_parse)] << "\n";
}
