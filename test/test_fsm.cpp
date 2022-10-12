#include <iostream>

#include <FSM.hpp>

int main(){
  std::string re = "a?b+(zs*l+)c*dfg";
  FSM fsm(re);
  std::cout << fsm;
  return 0;
}
