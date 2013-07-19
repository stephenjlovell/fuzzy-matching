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

#ifndef __LEVENSHTEIN__DAWG__H__
#define __LEVENSHTEIN__DAWG__H__

#include <stack>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <tuple>

namespace levenshtein {

  // ===========================================================================
  // Taken and modified for my purposes from the following source:
  //  o http://stevehanov.ca/blog/index.php?id=115
  // ===========================================================================
  //
  // This class represents a node in the directed acyclic word graph (DAWG,
  // a.k.a.  Minimal Acyclic Finite State Automaton, or MA-FSA).  It has a list
  // of edges to other nodes.  It has functions for testing whether it is
  // equivalent to another node.  Nodes are equivalent if they have identical
  // edges, and each identical edge leads to identical states.
  class DawgNode {
    public:
      DawgNode();
      ~DawgNode();

      std::map<const char, DawgNode *> edges;
      bool final;  // originally bool final = false; Moved initialization to constructor DawgNode()
	  long long get_id();
      std::string signature() const;
    private:
      static unsigned long long next_id();
      const unsigned long long id;
  };
  
  struct NodeStackRecord;
  
  class Dawg {
    public:
      Dawg(std::vector<std::string> &dictionary);
	  ~Dawg();

      DawgNode *root;

      void insert(std::string word);
      void finish();
      void minimize(size_t lower);
      bool accepts(std::string &word) const;
	  long long Count_Nodes();
	  std::vector<std::string> Dawg::Fuzzy_Match(std::string str1, int K);
	  std::set<std::string> Dawg::Search(std::string query_term, int k);
    private:
	  std::string previous_word;
	  std::string query_term;
	  int N1, k;
	  long fcalls, fsavings;
	  long long node_count;
	  std::unordered_map<std::string,std::unordered_map<long long,std::unordered_map<int,int>>> Visited;
      std::vector<std::tuple<DawgNode*, char, DawgNode*>> unchecked_nodes;
      std::unordered_map<std::string, DawgNode*> minimized_nodes;

	  void Dawg::Fuzzy_Match_Compare(std::vector<std::string> &vResults, std::deque<NodeStackRecord> &Frontier);
	  void Dawg::Search_Recursive(std::set<std::string> &vResults, std::string prefix, DawgNode* node, int i, int k);
	  void Update_Memo(std::string prefix, DawgNode* node, int i, int k);
	  bool Read_Memo(std::string prefix, DawgNode* node, int i_prime, int k_prime);
	};
}

#endif
