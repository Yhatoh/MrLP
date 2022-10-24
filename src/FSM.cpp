// Cpp include(s)
#include <iostream>
#include <set>
#include <stack>
#include <string>

// Local include(s)
#include <FSM.hpp>

const uint64_t initial_max_edges = 1000;

Node::Node() : max_edges(initial_max_edges) {
  edges = new Edge[initial_max_edges];
}

Node::Node(std::string label_, uint64_t num_edges_) : 
  label(label_), num_edges(num_edges_), max_edges(initial_max_edges) {
  edges = new Edge[initial_max_edges];
}

Node::~Node() {
  delete[] edges;
}

void Node::add_edge(char transition, Node* dest) {
  num_edges++;
  if(num_edges > max_edges) {
    Edge* aux = new Edge[max_edges * 2];
    for(uint64_t i = 0; i < max_edges; i++) {
      aux[i] = edges[i];
    }
    max_edges *= 2;
    delete[] edges;
    edges = aux;
  }

  Edge edge;
  edge.transition = transition;
  edge.dest = dest;
  edges[num_edges - 1] = edge;
}

Edge* Node::get_edges() {
  return edges;
}

uint64_t Node::get_num_edges() {
  return num_edges;
}

std::string Node::get_label() {
  return label;
}

void Node::set_edges(Edge* edges_, uint64_t num_edges_) {
  delete[] edges;
  edges = new Edge[num_edges];
  for(uint64_t i = 0; i < num_edges_; i++) {
    edges[i] = edges_[i];
  }
}

void Node::set_num_edges(uint64_t num_edges_) {
  num_edges = num_edges_;
}

void Node::set_label(std::string label_) {
  label = label_;
}

FSM::FSM(std::string expr) {
  num_states = 0;
  num_edges = 0;
  amount_expr = 0;
  subexpr = new SubExpr[0];
  start = new Node();
  end = new Node();
  create_fsm("0_", expr, start, end);
}

FSM::~FSM() {
  clear();
  delete[] subexpr;
  delete start;
  delete end;
}

void FSM::clear() {
  Node* next[num_states];
  uint64_t next_insert = 0;
  uint64_t next_pop = 0;

  next[next_insert++] = start;
  Node* to_erase[num_states];
  
  uint64_t count = 0;
  to_erase[count++] = start;

  std::set< std::string > visited;
  while(count < num_states){
    Node* act = next[next_pop++];
    Edge* edges = act->get_edges();
    for(uint64_t i = 0; i < act->get_num_edges(); i++) {
      Edge edge = edges[i];
      if(visited.find(edge.dest->get_label()) == visited.end()) {
        visited.insert(edge.dest->get_label());
        std::cout << edge.dest->get_label() << "\n";
        next[next_insert++] = edges[i].dest;
        to_erase[count++] = edges[i].dest;
      }
    }
  }
  std::cout << "-----\n";
  for(uint64_t i = 0; i < num_states; i++) {
    Node* aux = to_erase[i];
    if(aux->get_label() != "0_S" && aux->get_label() != "0_F")
      delete aux;  
  }
}

void FSM::create_fsm(std::string tag_scope, std::string expr, Node* act_start, Node* act_end) {
  // mantain depth of groups in re
  std::stack< uint64_t > st;
  // remember all subexpressions
  uint64_t count_subexpr = 0;
  for(uint64_t i = 0; i < expr.length(); i++) {
    // begin of a group
    if(expr[i] == '(') {
      st.push(i);
    } else if(expr[i] == ')') {
      uint64_t l = st.top();
      std::string substr = expr.substr(l + 1, i - 1 - (l + 1) + 1);
 
      Node* substart = new Node();
      Node* subend = new Node();     
      create_fsm(tag_scope + std::to_string(count_subexpr) + "_", substr, substart, subend);

      std::string tag = "$" + std::to_string(count_subexpr) + "$";
      
      expr.replace(l, i - l + 1, tag); 
      
      SubExpr* aux = new SubExpr[amount_expr + 1];
      for(uint64_t i = 0; i < amount_expr; i++) {
        aux[i] = subexpr[i];
      } 
      delete[] subexpr;
      SubExpr to_add;
      to_add.label = tag;
      to_add.start = substart;
      to_add.end = subend;
      aux[amount_expr++] = to_add;
      subexpr = aux;

      count_subexpr++;
      st.pop();
    }
  }
  // that is for later (cases with groups and |)
  uint64_t count_node = 1;
  act_start->set_label(tag_scope + "S");
  act_start->set_num_edges(0);
  act_end->set_label(tag_scope + "F");
  act_end->set_num_edges(0);
  num_states += 2;

  Node* act = act_start;
  bool act_end_created = false;
  for(uint64_t i = 0; i < expr.length(); i++) {
    if(i + 1 < expr.length() && expr[i + 1] == '*') {
      Node* dest = act;
      act->add_edge(expr[i], dest);
      act = dest;
      dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge('$', dest);
      act = dest;
      
      num_states += 1;
      num_edges += 2;

      i++;
    } else if(i + 1 < expr.length() && expr[i + 1] == '+') {
      Node* dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge(expr[i], dest);
      
      act = dest;
      act->add_edge(expr[i], dest);

      dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge('$', dest);
      act = dest;
      
      num_states += 2;
      num_edges += 3;

      i++;
    } else if(i + 1 < expr.length() && expr[i + 1] == '?') {
      Node* dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge('$', dest);
      act->add_edge(expr[i], dest);
      act = dest;

      num_states += 1;
      num_edges += 2;

      i++;
    } else if(i + 2 < expr.length() && i + 1 < expr.length() && expr[i] == '$' && expr[i + 2] == '$') {
      SubExpr subfsm;
      std::string to_compare = expr.substr(i, 3);
      for(uint64_t i = 0; i < amount_expr; i++) {
        if(to_compare == subexpr[i].label) {
          subfsm = subexpr[i];
          break;
        }
      }

      Node* aux = subfsm.start;
      act->add_edge('$', aux);
      
      num_edges++;

      act = subfsm.end;
      if(i + 3 < expr.length() && expr[i + 3] == '+') {
        act->add_edge('$', aux);
        i++;
      } else if(i + 3 < expr.length() && expr[i + 3] == '*') {
        aux->add_edge('$', act); 
        act->add_edge('$', aux);
        num_edges++;
        i++;
      } else if(i + 3 < expr.length() && expr[i + 3] == '?') {
        aux->add_edge('$', act);
        i++;
      }
      num_edges++;

      i += 2;
    } else if(expr[i] == '|') {
      act->add_edge('$', act_end);
      act = act_start;

      num_edges++;
    } else {
      Node* dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge(expr[i], dest);
      act = dest;

      num_states++;
      num_edges++;
    }
  } 

  act->add_edge('$', act_end);
  num_edges++;
}

bool FSM::check(std::string str) {
  return check_aux(str, 0, start);  
}

bool FSM::check_aux(std::string str, int i, Node* act_node) {
  if(act_node == end && i == str.length()) {
    return true;
  }

  uint64_t node_num_edges = act_node->get_num_edges();
  Edge* edges = act_node->get_edges();
  bool res = false;
  for(uint64_t j = 0; j < node_num_edges; j++) {
    if(i < str.length() && edges[j].transition == str[i]){
      res = check_aux(str, i + 1, edges[j].dest) || res;
    } else if(edges[j].transition == '$') {
      res = check_aux(str, i, edges[j].dest) || res;
    }
  }

  return res;
}

std::ostream& operator<<(std::ostream& os, const FSM &fsm) {
  //std::queue< Node > next;
  Node* next[fsm.num_states];
  uint64_t next_insert = 0;
  uint64_t next_pop = 0;

  next[next_insert++] = fsm.start;
  std::set< std::string > visited;
  while(next_pop < fsm.num_states){
    Node* act = next[next_pop++];
    std::cout << "node: " << act->get_label() << " transitions: ";
    Edge* edges = act->get_edges();
    for(uint64_t i = 0; i < act->get_num_edges(); i++) {
      Edge edge = edges[i];
      std::cout << "(" << edge.transition << "," << edge.dest->get_label() << ") ";
      if(visited.find(edge.dest->get_label()) == visited.end()) {
        visited.insert(edge.dest->get_label());
        next[next_insert++] = edges[i].dest;
      }
    }
    std::cout << "\n";
  }
  return os; 
}
