#include "Earley.h"

void Earley::fit(const Grammar& grammar) {
  grammar_ = grammar;
  old_starting_ = grammar.starting_nonterminal;
  grammar_.starting_nonterminal = kNewStarting;
  grammar_.nonterminals.insert(kNewStarting);
  new_rule_ = {kNewStarting, std::string(1, old_starting_)};
  grammar_.rules[kNewStarting].push_back(new_rule_);
  for (char nonterminal : grammar_.nonterminals) {
    for (auto& [left, right] : grammar_.rules[nonterminal]) {
      right.push_back(kRuleEnd);
    }
  }
  new_rule_.right.push_back(kRuleEnd);
}

bool Earley::predict(const std::string& word) {
  PrepareData(word);
  InsertSituation({new_rule_, 0, 0, 0});
  for (int level_num = 0; level_num <= word_length_; ++level_num) {
    UpdateLevel(level_num);
  }
  return levels_[word_length_][kRuleEnd].find({new_rule_, 0,
                                               word_length_, 1}) !=
         levels_[word_length_][kRuleEnd].end();
}

char Earley::Situation::GetNowChar() const {
  return rule.right[point_pos];
}

bool operator==(const Earley::Situation& first,
                const Earley::Situation& second) {
  return first.rule.left == second.rule.left &&
         first.rule.right == second.rule.right &&
         first.parent_scanned == second.parent_scanned &&
         first.now_scanned == second.now_scanned &&
         first.point_pos == second.point_pos;
}

size_t Earley::SituationHash::operator()(
                             const Situation& situation) const {
  const size_t kCoefficient = 43;
  const size_t kMod = 120'072'007;
  size_t result = situation.rule.left;
  for (char symbol : situation.rule.right) {
    result = (result * kCoefficient + symbol) % kMod;
  }
  result = (result + situation.parent_scanned +
            situation.point_pos) % kMod;
  return result;
}

void Earley::PrepareData(const std::string& word) {
  word_length_ = word.size();
  word_ = word;
  levels_.clear();
  levels_.resize(word_length_ + 2);
  update_queues_.clear();
  update_queues_.resize(word_length_ + 2);
}

void Earley::InsertSituation(const Situation& situation) {
  if (levels_[situation.now_scanned][situation.GetNowChar()].find(
                                                        situation) !=
      levels_[situation.now_scanned][situation.GetNowChar()].end()) {
    return;
  }
  update_queues_[situation.now_scanned].push(situation);
  levels_[situation.now_scanned][situation.GetNowChar()].insert(
                                                          situation);
}

void Earley::UpdateScan(const Situation& situation) {
  if (situation.GetNowChar() != word_[situation.now_scanned]) {
    return;
  }
  Situation new_situation = situation;
  ++new_situation.now_scanned;
  ++new_situation.point_pos;
  InsertSituation(new_situation);
}

void Earley::UpdatePredict(const Situation& situation) {
  if (grammar_.nonterminals.find(situation.GetNowChar()) ==
      grammar_.nonterminals.end()) {
    return;
  }
  for (const Rule& rule : grammar_.rules[situation.GetNowChar()]) {
    Situation new_situation = {rule, situation.now_scanned,
                               situation.now_scanned, 0};
    InsertSituation(new_situation);
  }
}

void Earley::UpdateComplete(const Situation& situation) {
  if (situation.GetNowChar() != kRuleEnd) {
    return;
  }
  for (const Situation& parent_situation :
       levels_[situation.parent_scanned][situation.rule.left]) {
    Situation new_situation = parent_situation;
    new_situation.now_scanned = situation.now_scanned;
    ++new_situation.point_pos;
    InsertSituation(new_situation);
  }
}

void Earley::UpdateSituation(const Situation& situation) {
  UpdateScan(situation);
  UpdatePredict(situation);
  UpdateComplete(situation);
}

void Earley::UpdateLevel(int level_num) {
  while (!update_queues_[level_num].empty()) {
    UpdateSituation(update_queues_[level_num].front());
    update_queues_[level_num].pop();
  }
}
