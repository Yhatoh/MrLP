/*
 * My RegEx implementation
 *
 *  - special char `+` `*` `|` `?`
 *  - recursive? maybe 
 *  - group
 */

class RegEx {
  public:
    string expr;

    RegEx();
    RegEx(string expr_);

    void set_expr(string expr_);
    string get_expr();

    // get the position of the first match of expr in str
    uint64_t first_match(string str);

    // get the position of the i-th match of expr in str
    uint64_t match(string str, uint64_t i);

    // get a vector of positions of all the matches of expr in str
    vector< uint64_t > full_match(string str);
};
