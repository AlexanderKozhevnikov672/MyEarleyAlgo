#pragma once 

#include "Grammar.h"
#include <iostream>
#include <map>
#include <queue>

class Earley {
 public:
  void fit(const Grammar& grammar);

  bool predict(const std::string& word);
 
 private:
  constexpr static char kNewStarting = '$';
  constexpr static char kRuleEnd = '#';
  
  Grammar grammar_;
  char old_starting_;
  Rule new_rule_;
  int word_length_;
  std::string word_;

  struct Situation {
    Rule rule;
    int parent_scanned;
    int now_scanned;
    int point_pos;

    char GetNowChar() const;
  };

  friend bool operator==(const Earley::Situation& first,
                         const Earley::Situation& second);

  struct SituationHash {
    size_t operator()(const Situation& situation) const;
  };

  using Level = std::map<char,
                    std::unordered_set<Situation, SituationHash>>;

  std::vector<Level> levels_;
  std::vector<std::queue<Situation>> update_queues_;

  void PrepareData(const std::string& word);

  void InsertSituation(const Situation& situation);

  void UpdateScan(const Situation& situation);

  void UpdatePredict(const Situation& situation);

  void UpdateComplete(const Situation& situation);

  void UpdateSituation(const Situation& situation);

  void UpdateLevel(int level_num);
};
