#include <iostream>

#include <RegEx.hpp>

int main(){
  std::string re = "ho*la+|(a|s)+d*f";
  FSM fsm(re);
  std::cout << fsm;
  std::cout << (fsm.check("sdddf") ? "Bien" : "Mal") << "\n";
  RegEx er(re);
  std::string str = "holaholaholahola";
  std::cout << er.first_match(str) << "\n";
  std::cout << er.match(str, 3) << "\n";
  std::cout << er.match(str, 7) << "\n";
  for(uint64_t e : er.full_match(str)) {
    std::cout << e << " "; 
  }
  std::cout << "\n";
  std::cout << fsm.num_edges << "\n";
  std::cout << fsm.num_states << "\n";
  return 0;
}
