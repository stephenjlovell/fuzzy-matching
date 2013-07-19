// Copyright (c) 2012 Dylon Edwards
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <stack>
#include <set>
#include <unordered_map>
#include "dawg.h"

namespace levenshtein {
  // DawgNode Class Implementation
  
  unsigned long long DawgNode::next_id() {
    static unsigned long long next_id = 0;
    return ++ next_id;
  }
  // DawgNode Constructor
  DawgNode::DawgNode() : id(next_id()) {
	DawgNode::final=false;
  }

  // DawgNode Destructor
  DawgNode::~DawgNode() {
    for (auto pair : edges) {
      delete pair.second;
    }
  }

  long long DawgNode::get_id(){
	  return this->id;
  }

  // This method returns a unique string made up of the char values and IDs of each child node that is connected to this->DawgNode
  std::string DawgNode::signature() const {
    std::vector<std::string> edges;  // local vector<> object.  Overloads public map<> object edges.
    for (auto pair : this->edges) {
      std::stringstream stream;  // re-initialize variable stream
      stream << pair.first << pair.second->id;  // adds the char value and the ID of edge to stream. Ex: "a1"
      edges.push_back(stream.str());
    }
    std::sort(edges.begin(), edges.end());
    std::stringstream stream;
    for (auto edge : edges) {  // refers to local vector<> object.
      stream << edge;  
    }
    return stream.str();  // appends all edges in local vector<> object to single return string.  Ex: "a1b2"
  }

  struct NodeStackRecord{
	std::string prefix;
	int cost;
	int depth;
	int i;
	DawgNode* node;
	NodeStackRecord(std::string Prefix, int Cost, int Depth, int I, DawgNode* Node) : prefix(Prefix), cost(Cost), depth(Depth), i(I), node(Node){};
  };

  // Dawg Class Implementation

  // Dawg Constructor
  Dawg::Dawg(std::vector<std::string> &dictionary) : root(new DawgNode()) {
	Dawg::previous_word = "";
	for (std::string word : dictionary) {
      insert(word);
    }
	finish();
  }

  //Dawg Destructor
  Dawg::~Dawg() {
    delete this->root;
  }

  void Dawg::insert(std::string word) {
    size_t i = 0; // find the longest common prefix
    size_t upper = std::min(word.length(), previous_word.length());
    for (; i < upper && word[i] == previous_word[i]; ++i);

    // Check the unchecked_nodes for redundant nodes, proceeding from the leaf
    // up to the longest common prefix.  Then, truncate the list at that point.
    minimize(i);

    // Add the suffix, starting from the correct node mid-way through the graph
    DawgNode *node = unchecked_nodes.size()
      ? std::get<2>(unchecked_nodes.back())
      : root;

    for (; i < word.length(); ++i) {
      const char &edge = word[i];
      DawgNode *next_node = new DawgNode();
      node->edges[edge] = next_node;
      unchecked_nodes.push_back(std::make_tuple(node, edge, next_node));
      node = next_node;
    }

    node->final = true;
    previous_word = word;
  }

  void Dawg::finish() {
    minimize(0);
	DawgNode* temp_node = new DawgNode;
	node_count = temp_node->get_id();
	delete temp_node;
  }

  void Dawg::minimize(size_t lower) {
    while (unchecked_nodes.size() > lower) {
      DawgNode *parent, *child; char edge;
      std::tie(parent, edge, child) = unchecked_nodes.back();
      unchecked_nodes.pop_back();

      std::string signature = child->signature();

      auto previous = minimized_nodes.find(signature);
      if (previous != minimized_nodes.end()) {
        parent->edges[edge] = previous->second;
        child->edges.erase(child->edges.begin(), child->edges.end());
        delete child;
      }
      else {
        minimized_nodes[signature] = child;
      }
    }
  }

  bool Dawg::accepts(std::string &word) const {
    DawgNode *node = root;
    for (const char &edge : word) {
      auto next = node->edges.find(edge);  //  std::map::find(k) - Searches the container for an element with a key equivalent to k 
										   //					   and returns an iterator to it if found, otherwise it returns an iterator to map::end.
      if (next == node->edges.end()) {	   //  std::map::end()   - Returns an iterator referring to the past-the-end element in the map container.
        return false;  // all edges have been checked.  char edge not found
      }
      node = next->second;  //move on to the next node  *node = std::pair<>::second
    }
	//  Every char edge has been found.  If this is a final node, return true.  Otherwise the word exists only as a substring of another dictionary entry.
    return node->final;  // returns bool DawgNode::final for node *node.
  }

  long long Dawg::Count_Nodes(){
	  return node_count;
  }
  
  //-------------------------------------------------------------------------------------------------//
  //  Copyright (c) 2013 Steve Lovell                                                                //
  //  Extend class to provide methods for approximate matching:                                      //
  //-------------------------------------------------------------------------------------------------//

  //This method reads in a query term and returns a vector containing all strings matching the query within edit distance k.
  std::vector<std::string> Dawg::Fuzzy_Match(std::string Query_Term, int K){
	std::vector<std::string> Results;
	std::deque<NodeStackRecord> Frontier;
	// initialize Dawg parameters
	N1 = Query_Term.length();
	k = K;
	query_term = Query_Term;
	for(int i=0; i<=k;i++){
		query_term += " ";  // lengthen query term to prevent subscript out of range errors.
	}

	for(auto edge : root->edges){
		std::string first = std::string(1,edge.first);
		Frontier.push_front(*new NodeStackRecord(first,0,1,0,edge.second));  // push edges of root node into Frontier.
	}

	Fuzzy_Match_Compare(Results, Frontier);
	return Results;  // return a vector containing all matching strings found.

  }
  
  void Dawg::Fuzzy_Match_Compare(std::vector<std::string> &Results, std::deque <NodeStackRecord> &Frontier){
	int max_depth = N1 + k;
	int edits_remaining = k;

	while (!Frontier.empty()){
		NodeStackRecord front = Frontier.front();  	// inspect the front item in the stack
		edits_remaining = k - front.cost;
		
		if((N1 - front.i <= edits_remaining) && (max_depth - front.depth <= edits_remaining) && (front.node->final)){  // check if current record is a goal.
			Results.push_back(front.prefix);  // add item to Results vector.
		}
		
		if((front.depth > max_depth) || (edits_remaining < 0)){   // || (front.i >= N1)
			Frontier.pop_front();  // maximum search depth exceeded, or current record is a non-match, or all characters in query term have been checked.
			continue;
		}

		if(query_term[front.i] == front.prefix.back()){
			for(auto edge : front.node->edges){
				Frontier.push_back(*new NodeStackRecord(front.prefix+std::string(1,edge.first),front.cost,front.depth+1,front.i+1,edge.second)); // equality
			}
		} else {
			Frontier.push_back(*new NodeStackRecord(front.prefix, front.cost+1,front.depth, front.i+1,front.node));  // deletion
			for(auto edge : front.node->edges){
				Frontier.push_back(*new NodeStackRecord(front.prefix+std::string(1,edge.first),front.cost+1,front.depth+1,front.i,edge.second));  // insertion
				Frontier.push_back(*new NodeStackRecord(front.prefix+std::string(1,edge.first),front.cost+1,front.depth+1,front.i+1,edge.second));  // substitution
			}
		}
		Frontier.pop_front();  // remove current record from Frontier.
	} //wend

  }  //end function

  std::set<std::string> Dawg::Search(std::string Query_Term, int K){
	
	std::set<std::string> Results;
	DawgNode* node = root;

	query_term = Query_Term;
	N1 = query_term.length();
	k = K;
	fcalls = 0;
	fsavings = 0;
	for(int i=0; i<=k;i++){
		query_term += " ";  // lengthen query term to prevent subscript out of range errors.
	}
	
	
	for (auto edge : root->edges){
		Search_Recursive(Results, std::string(1,edge.first), edge.second, 0, k);
	}
	std::cout << std::endl << "Savings = " << fsavings << std::endl;
	std::cout << "total calls = " << fcalls << std::endl;
	return Results;
  }

  void Dawg::Search_Recursive(std::set<std::string> &Results, std::string prefix, DawgNode* node, int i_prime, int k_prime){
	
	fcalls++;
	Update_Memo(prefix,node,i_prime,k_prime);
	
	std::string ch, qc;
	if (!prefix.length() == 0){ch = prefix[prefix.length()-1]; } else { ch = "";}  //this should never be zero
	qc = query_term[i_prime];

	if((k_prime==-1) || ((k_prime==0) && (ch != qc))) return; // cannot match
	
	// check if node is a goal:
	if ((node->final) && (((N1-i_prime)<=k_prime) || (((N1-i_prime)<=(k_prime+1)) && (ch == qc)))){
		Results.emplace(prefix);  // match found.  add to Results set.
	}

	int edge_count=0;
	for(auto edge : node->edges){edge_count++;}
	if(edge_count==0) return;  // no more edges to explore.

	if (ch != qc){  
		if(!Read_Memo(prefix, node,i_prime+1, k_prime-1)){
			Search_Recursive(Results, prefix, node,i_prime+1, k_prime-1);  // deletion
		}
	}
	for (auto edge : node->edges){
		if (ch == qc){
			if(!Read_Memo(prefix+std::string(1,edge.first) , edge.second,i_prime+1, k_prime)){
				Search_Recursive(Results, prefix+std::string(1,edge.first) , edge.second,i_prime+1, k_prime);  // equality
			}
		} else {
			if(!Read_Memo(prefix+std::string(1,edge.first), edge.second,i_prime+1, k_prime-1)){
				Search_Recursive(Results, prefix+std::string(1,edge.first), edge.second,i_prime+1, k_prime-1);  // insertion
			}
			if(!Read_Memo(prefix+std::string(1,edge.first), edge.second,i_prime, k_prime-1)){
				Search_Recursive(Results, prefix+std::string(1,edge.first), edge.second,i_prime, k_prime-1);  // substitution
			}
		}
	}
    return;
  }

  void Dawg::Update_Memo(std::string prefix, DawgNode* node, int i_prime, int k_prime){
	std::unordered_map<long long,std::unordered_map<int,int>>* p1 = new std::unordered_map<long long,std::unordered_map<int,int>>;
	std::unordered_map<int,int>* p2 = new std::unordered_map<int,int>;
	bool boolVisited = true;
	long long node_ID = node->get_id();

	if(!Visited.emplace(prefix,*p1).second) delete p1; // allocated memory was not needed, object already existed.
	if(!Visited.at(prefix).emplace(node_ID,*p2).second) delete p2;  // allocated memory was not needed, object already existed.
	Visited.at(prefix).at(node_ID).emplace(i_prime,k_prime);
    return;
  }

  bool Dawg::Read_Memo(std::string prefix, DawgNode* node, int i_prime, int k_prime){
	int m;
	try{ m = Visited.at(prefix).at(node->get_id()).at(i_prime); } // 
	catch(const std::out_of_range){return false;}
	if(k_prime > m){ return false;} else {fsavings++; return true;}
  }
}  //end namespace 




