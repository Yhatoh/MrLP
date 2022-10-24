#include <iostream>

#include <RegEx.hpp>

int main(){
  std::string re = "ho*la+|(a|s)+d*f";
  FSM fsm(re);
  std::cout << fsm;
  std::cout << (fsm.check("sdddf") ? "Bien" : "Mal") << "\n";
  std::cout << (fsm.check("shdddf") ? "Bien" : "Mal") << "\n";
  RegEx er(re);
  std::string str = "holaholaholahola";
  std::cout << er.first_match(str) << "\n";
  std::cout << er.match(str, 3) << "\n";
  std::cout << er.match(str, 7) << "\n";
  for(uint64_t e : er.full_match(str)) {
    std::cout << e << " "; 
  }
  std::cout << "\n";
  std::string re2 = "h[a-z]*l";
  FSM fsm2(re2);
  std::cout << fsm2;
  std::cout << (fsm2.check("hal") ? "Bien" : "Mal") << "\n";
  std::cout << (fsm2.check("hbal") ? "Bien" : "Mal") << "\n";
  std::cout << fsm.num_edges << "\n";
  std::cout << fsm.num_states << "\n";
  return 0;
}
