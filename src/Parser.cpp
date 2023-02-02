// Cpp include(s)
#include <vector>

// Local inlude(s)
#include <Parser.hpp>

struct NodeBinop {
  std::string id;
  NodeBinop* left = NULL;
  NodeBinop* right = NULL;
};

struct ParsedBinop {
  NodeBinop* node;
  ParsedBinop(std::string str) {
    node = new NodeBinop;
    parse(str, node);
  }

  void parse(std::string str, NodeBinop* act) {
    RegEx times_div("\\*|\\/");
    RegEx plus_minus("\\+|\\-");

    auto pos = plus_minus.first_match(str);
    if(pos.first != (uint64_t) -1) {
      act->id = str.substr(pos.first, pos.second);
      act->left = new NodeBinop;
      act->right = new NodeBinop;
      
      parse(str.substr(0, pos.first), act->left);
      parse(str.substr(pos.first + 1, str.length() - pos.first), act->right);
      
      return;
    }


    pos = times_div.first_match(str);
    if(pos.first != (uint64_t) -1) {
      act->id = str.substr(pos.first, pos.second);
      act->left = new NodeBinop;
      act->right = new NodeBinop;
      
      parse(str.substr(0, pos.first), act->left);
      parse(str.substr(pos.first + 1, str.length() - pos.first), act->right);
      
      return;
    }

    RegEx id("[a-z](_|[a-z]|[A-Z]|[0-9])*");
    pos = id.first_match(str);
    if(pos.first != (uint64_t) -1 && pos.second == str.length()) {
      act->id = str;
      return;
    }

    RegEx num("[1-9][0-9]*");
    pos = num.first_match(str);
    if(pos.first != (uint64_t) -1 && pos.second == str.length()) {
      act->id = str;
      return;
    }

    std::cerr << "EXPR_ERROR 1: this " << str << " is not a correct binop.\n";
    exit(0); 
  }

  void traverse() {
    traverse(node);
  }

  void traverse(NodeBinop* act) {
    if(act == NULL) return;
    std::cout << act->id << "\n";
    traverse(act->left);
    traverse(act->right);
  }
};

Parser::Parser(std::string to_parse) { 
  RegEx id("[a-z](_|[a-z]|[A-Z]|[0-9])*");
  RegEx num("[1-9][0-9]*");
  RegEx eq("=");
  RegEx print("print");
  RegEx espace(" *");
  
  std::vector< std::pair< uint64_t, uint64_t > > espaces
    = espace.full_match(to_parse);
  // removing unnecesarry space;
  for(uint64_t i = espaces.size(); i > 0; i--) {
    auto rep = espaces[i - 1];
    to_parse.replace(rep.first, rep.second, "");
  }
  
  std::cout << to_parse << "\n";
  auto splitted = eq.split(to_parse);
  ParsedBinop(splitted[1]).traverse(); 
}
