// Cpp include(s)
#include<string>

// Local include(s)
#include <Types.hpp>

ParsedExpr::ParsedExpr(uint64_t type_, void* expr_) : 
  type(type_), expr(expr_) {}

uint64_t ParsedExpr::get_type() {
  return type;
}

void* ParsedExpr::get_expr() {
  return expr;
}

ParsedBinop::ParsedBinop(std::string op_, 
                         uint64_t left_type_, void* left_expr_, 
                         uint64_t right_type_, void* right_expr_) :
  op(op_), 
  left_type(left_type_), left_expr(left_expr_),  
  right_type(right_type_), right_expr(right_expr_)  {}

std::string ParsedBinop::get_op() {
  return op;
}

uint64_t ParsedBinop::get_type(uint64_t arg) {
  return (arg == 0 ? left_type : right_type);
}

void* ParsedBinop::get_expr(uint64_t arg) {
  return (arg == 0 ? left_expr : right_expr);
}
