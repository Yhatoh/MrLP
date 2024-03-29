// Cpp include(s)
#include <string>
#include <vector>

// Local include(s)
#include <FSM.hpp>

class RegEx {
  private:
    FSM* fsm;
  public:

    RegEx();
    RegEx(std::string expr_);

    ~RegEx();

    void clear();

    void set_expr(std::string expr_);
    FSM* get_expr();

    // get the position of the first match of expr in str
    // and max length of that match
    std::pair< uint64_t, uint64_t > first_match(std::string str);

    // get the position of the i-th match of expr in str
    // and max length of that match
    std::pair< uint64_t, uint64_t > match(std::string str, uint64_t i);

    // get a vector of positions of all the matches of expr in str
    std::vector< std::pair< uint64_t, uint64_t > > full_match(std::string str);
    
    // split a string by the expr
    std::vector< std::string > split(std::string str);
};
