// Cpp include(s)
#include <vector>
#include <string>

// Local include(s)
#include <RegEx.hpp>

RegEx::RegEx() {}
RegEx::RegEx(std::string expr_) {
  fsm = new FSM(expr_); 
}

RegEx::~RegEx() {
  clear();
}

void RegEx::clear() {
  fsm->clear();
  delete fsm;
}

void RegEx::set_expr(std::string expr_) { 
  delete fsm;
  fsm = new FSM(expr_);
}

FSM* RegEx::get_expr() { return fsm; }

uint64_t RegEx::first_match(std::string str) {
  for(uint64_t j = 0; j < str.length(); j++) {
    for(uint64_t k = str.length() - 1; k > 0; k--) {
      if(fsm->check(str.substr(j, k))) {
        return j;
      } 
    }
  }  
  return str.length();
}

uint64_t RegEx::match(std::string str, uint64_t i) {
  uint64_t ith = 0;
  for(uint64_t j = 0; j < str.length(); j++) {
    for(uint64_t k = str.length() - j; k > 0; k--) {
      if(fsm->check(str.substr(j, k))) {
        ith++;
        if(ith == i) {
          return j;
        }
        j += k - 1;
        break;
      } 
    }
  }  
  return str.length();
}

std::vector< uint64_t > RegEx::full_match(std::string str){
  std::vector< uint64_t > ret;
  for(uint64_t j = 0; j < str.length(); j++) {
    for(uint64_t k = str.length() - j; k > 0; k--) {
      if(fsm->check(str.substr(j, k))) {
        ret.push_back(j);
        j += k - 1;
        break;
      } 
    }
  }
  return ret;
}
