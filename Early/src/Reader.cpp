#include "Reader.h"
#include <exception>
#include <iostream>
#include <optional>

Grammar Reader::ReadGrammar(std::istream& input_stream) {
  Grammar grammar;
  int nonterminal_count;
  int terminal_count;
  int rule_count;
  input_stream >> nonterminal_count >> terminal_count >> rule_count;
  ReadNonterminals(input_stream, nonterminal_count, grammar);
  ReadTerminals(input_stream, terminal_count, grammar);
  ReadRules(input_stream, rule_count, grammar);
  
  input_stream >> grammar.starting_nonterminal;
  if (grammar.nonterminals.find(grammar.starting_nonterminal) ==
      grammar.nonterminals.end()) {
    throw std::invalid_argument(
              "Starting nonterminal is not a nonterminal!");
  }
  return grammar;
}

void Reader::ReadNonterminals(std::istream& input_stream,
                              int nonterminal_count,
                              Grammar& grammar) {
  for (int nonterminal_num = 0; nonterminal_num < nonterminal_count;
       ++nonterminal_num) {
    char nonterminal;
    input_stream >> nonterminal;
    grammar.nonterminals.insert(nonterminal);
  }
}

void Reader::ReadTerminals(std::istream& input_stream,
                           int terminal_count, Grammar& grammar) {
  for (int terminal_num = 0; terminal_num < terminal_count;
       ++terminal_num) {
    char terminal;
    input_stream >> terminal;
    if (grammar.nonterminals.find(terminal) !=
        grammar.nonterminals.end()) {
      throw std::invalid_argument(
              "Intersection of sets of terminals and nonterminals!");
    }
    grammar.terminals.insert(terminal);
  }
}

void Reader::ReadRules(std::istream& input_stream, int rule_count,
                       Grammar& grammar) {
  for (int rule_num = 0; rule_num < rule_count; ++rule_num) {
    Rule rule = ReadRule(input_stream, grammar);
    grammar.rules[rule.left].push_back(rule);
  }
}

Rule Reader::ReadRule(std::istream& input_stream, Grammar& grammar) {
  std::string str;
  while (str.empty()) {
    std::getline(input_stream, str);
  }
  int size = str.size();
  std::optional<int> arrow_pos;
  for (int pos = 0; pos < size - 1; ++pos) {
    if (str[pos] == '-' && str[pos + 1] == '>') {
      arrow_pos = pos;
      break;
    }
  }
  if (!arrow_pos.has_value()) {
    throw std::invalid_argument("No arrow in rule!");
  }
  std::string left = str.substr(0, arrow_pos.value());
  while (!left.empty() && left.back() == ' ') {
    left.pop_back();
  }
  if (left.size() != 1 || grammar.nonterminals.find(left[0]) ==
                          grammar.nonterminals.end()) {
    throw std::invalid_argument("Not a context-free grammar!");
  }
  arrow_pos.value() += 2;
  while (arrow_pos.value() < size && str[arrow_pos.value()] == ' ') {
    ++arrow_pos.value();
  }
  std::string right = str.substr(arrow_pos.value());
  for (int pos = 0; pos < static_cast<int>(right.size()); ++pos) {
    if (grammar.nonterminals.find(right[pos]) ==
        grammar.nonterminals.end() &&
        grammar.terminals.find(right[pos]) ==
        grammar.terminals.end()) {
      throw std::invalid_argument("Unidentified symbol in rule!");
    }
  }
  return {left[0], right};
}
