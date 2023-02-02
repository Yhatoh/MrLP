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
  delete fsm;
}

void RegEx::set_expr(std::string expr_) { 
  delete fsm;
  fsm = new FSM(expr_);
}

FSM* RegEx::get_expr() { return fsm; }

std::pair< uint64_t, uint64_t > RegEx::first_match(std::string str) {
  for(uint64_t j = 0; j < str.length(); j++) {
    for(uint64_t k = str.length(); k > 0; k--) {
      if(fsm->check(str.substr(j, k))) {
        return {j, k};
      } 
    }
  }  
  return {-1, str.length()};
}

std::pair< uint64_t, uint64_t > RegEx::match(std::string str, uint64_t i) {
  uint64_t ith = 0;
  for(uint64_t j = 0; j < str.length(); j++) {
    for(uint64_t k = str.length() - j; k > 0; k--) {
      if(fsm->check(str.substr(j, k))) {
        ith++;
        if(ith == i) {
          return {j, k};
        }
        j += k - 1;
        break;
      } 
    }
  }  
  return {-1, str.length()};
}

std::vector< std::pair< uint64_t, uint64_t > > RegEx::full_match(std::string str){
  std::vector< std::pair< uint64_t, uint64_t > > ret;
  for(uint64_t j = 0; j < str.length(); j++) {
    for(uint64_t k = str.length() - j; k > 0; k--) {
      if(fsm->check(str.substr(j, k))) {
        ret.push_back(std::pair< uint64_t, uint64_t >(j, k));
        j += k - 1;
        break;
      } 
    }
  }
  return ret;
}

std::vector< std::string > RegEx::split(std::string str) {
  std::vector< std::string > splitted;
  std::pair< uint64_t, uint64_t > pos;
  do {
    pos = first_match(str);
    uint64_t len = (pos.first == (uint64_t)-1 ? pos.second : pos.first);
    splitted.push_back(str.substr(0, len));
    str.replace(0, len + pos.second, "");
  } while(pos.first != (uint64_t)-1);
  return splitted;
}
