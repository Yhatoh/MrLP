#include <iostream>
#include <map>
#include <memory>
#include <string>

typedef struct Node Node;

struct Edge {
  char transition;
  Node* dest;
};


class Node {
  private:
    std::string label;
    uint64_t num_edges;
    uint64_t max_edges;
    Edge* edges;
  
  public:
    Node();
    Node(std::string label, uint64_t num_edges);

    void add_edge(char transition, Node* dest);
    
    // getters
    Edge* get_edges();
    uint64_t get_num_edges();
    std::string get_label();
    
    // setters
    void set_edges(Edge* edges_, uint64_t num_edges_);
    void set_num_edges(uint64_t num_edges_);
    void set_label(std::string label_);

};

struct SubExpr {
  std::string label;
  Node* start;
  Node* end;
};

class FSM {
  private:
    //std::map< std::string, 
    //          std::pair< std::shared_ptr< Node >, std::shared_ptr< Node > > > subexpr;
    uint64_t amount_expr;
    SubExpr* subexpr;
    bool check_aux(std::string str, int i, Node* act_node);
  public:
    Node* start;
    Node* end;
    uint64_t num_states;
    uint64_t num_edges;
   
    FSM();
    FSM(std::string expr);

    ~FSM();
    
    void clear();
    //void set_expr(string expr);
    void create_fsm(std::string tag_scope, std::string expr, Node* act_start, Node* act_end);
    bool check(std::string str);
    friend std::ostream& operator<<(std::ostream& os, const FSM &fsm);
};
