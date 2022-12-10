// Cpp inlude(s)
#include <string>

// Local include(s) 
#include <Types.hpp>

class Parser {
  private:
    ParsedExpr* expr;
  public:
    Parser(std::string to_parse);
    ParsedExpr get_expr();
};
