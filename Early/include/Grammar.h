#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Rule {
  char left;
  std::string right;
};

struct Grammar {
  char starting_nonterminal;
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> terminals;
  std::unordered_map<char, std::vector<Rule>> rules;
};
