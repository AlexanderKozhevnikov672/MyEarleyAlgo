#pragma once

#include "Grammar.h"

class Reader {
 public:
  static Grammar ReadGrammar(std::istream& input_stream);

 private:
  static void ReadNonterminals(std::istream& input_stream,
                               int nonterminal_count,
                               Grammar& grammar);

  static void ReadTerminals(std::istream& input_stream,
                            int terminal_count, Grammar& grammar);

  static void ReadRules(std::istream& input_stream, int rule_count,
                        Grammar& grammar);

  static Rule ReadRule(std::istream& input_stream, Grammar& grammar);
};
