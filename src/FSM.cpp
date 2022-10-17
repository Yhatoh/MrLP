// Cpp include(s)
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <queue>
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
  start = new Node();
  end = new Node();
  create_fsm("0_", expr, start, end);
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
      std::pair< std::shared_ptr< Node >, std::shared_ptr< Node > > pnodes(substart, subend);
      subexpr.insert(std::pair< std::string, std::pair< std::shared_ptr< Node >, std::shared_ptr< Node > > >(tag, pnodes));
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

      i++;
    } else if(i + 1 < expr.length() && expr[i + 1] == '+') {
      Node* dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge(expr[i], dest);
      
      act = dest;
      act->add_edge(expr[i], dest);

      dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge('$', dest);
      act = dest;
      i++;
    } else if(i + 1 < expr.length() && expr[i + 1] == '?') {
      Node* dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge('$', dest);
      act->add_edge(expr[i], dest);
      i++;
      act = dest;
    } else if(i + 2 < expr.length() && i + 1 < expr.length() && expr[i] == '$' && expr[i + 2] == '$') {
      std::pair< std::shared_ptr< Node >, std::shared_ptr< Node > > subfsm = subexpr.find(expr.substr(i, 3))->second;
      
      Node* aux = new Node();
      aux->set_label(subfsm.first.get()->get_label());
      aux->set_num_edges(subfsm.first.get()->get_num_edges());
      aux->set_edges(subfsm.first.get()->get_edges(), aux->get_num_edges());

      act->add_edge('$', aux);
   
      act = subfsm.second.get();
      if(i + 3 < expr.length() && expr[i + 3] == '+') {
        act->add_edge('$', aux);
        i++;
      } else if(i + 3 < expr.length() && expr[i + 3] == '*') {
        aux->add_edge('$', act); 
        act->add_edge('$', aux); 
        i++;
      } else if(i + 3 < expr.length() && expr[i + 3] == '?') {
        aux->add_edge('$', act); 
        i++;
      }
      i += 2;
    } else if(expr[i] == '|') {
      act->add_edge('$', act_end);
      act = act_start;
    } else {
      Node* dest = new Node(tag_scope + std::to_string(count_node++), 0);
      act->add_edge(expr[i], dest);
      act = dest;
    }
  } 

  act->add_edge('$', act_end);
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
  std::queue< Node > next;
  next.push(*(fsm.start));
  std::set< std::string > visited;
  while(!next.empty()){
    Node act = next.front();
    std::cout << "Node: " << act.get_label() << " Transitions: ";
    Edge* edges = act.get_edges();
    for(uint64_t i = 0; i < act.get_num_edges(); i++) {
      Edge edge = edges[i];
      std::cout << "(" << edge.transition << "," << edge.dest->get_label() << ") ";
      if(visited.find(edge.dest->get_label()) == visited.end()) {
        visited.insert(edge.dest->get_label());
        next.push(*(edges[i].dest));
      }
    }
    next.pop();
    std::cout << "\n";
  }
  return os; 
}
