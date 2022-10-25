// Cpp include(s)
#include<string>

class ParsedExpr {
  private:
    uint64_t type;
    void* expr;
  public:
    ParsedExpr(uint64_t type_, void* expr_);
    uint64_t get_type();
    void* get_expr();   
};

class ParsedBinop {
  private:
    std::string op;
    uint64_t left_type;
    uint64_t right_type;
    void* left_expr;
    void* right_expr;
  public:
    ParsedBinop(std::string op_, 
                uint64_t left_type_, void* left_expr_, 
                uint64_t right_type, void* right_expr_);
    std::string get_op();
    uint64_t get_type(uint64_t arg);
    void* get_expr(uint64_t arg);
};
