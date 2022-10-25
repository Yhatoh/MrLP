// Cpp inlude(s)
#include <string>

// Local include(s) 
#include <Types.hpp>

class Parser {
  private:
    RegEx id("[a-z]([A-Z]|[1-2])*");
    RegEx num("[1-9][0-9]*");
    RegEx binop("+|-|*|/");
    RegEx print("print");

    ParsedExpr expr;
  public:
    Parser();

    ParsedExpr get_expr();

}
