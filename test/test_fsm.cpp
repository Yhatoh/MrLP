#include <iostream>

#include <FSM.hpp>

int main(){
  std::string re = "ho*la+|(a|s)+d*f";
  FSM fsm(re);
  std::cout << fsm;
  std::cout << (fsm.check("sdddf") ? "Bien" : "Mal") << "\n";
  return 0;
}
