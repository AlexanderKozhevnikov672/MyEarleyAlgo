#include <Earley.h>
#include <Reader.h>
#include <fstream>

int main() {
  std::ifstream input_file("../manual/test.txt");
  std::ofstream output_file("../manual/answer.txt");
  Grammar grammar = Reader::ReadGrammar(input_file);
  Earley earley;
  earley.fit(grammar);
  int word_count;
  input_file >> word_count;
  std::string word;
  std::getline(input_file, word);
  for (int word_num = 0; word_num < word_count; ++word_num) {
    std::getline(input_file, word);
    output_file << (earley.predict(word) ? "Yes" : "No") << "\n";
  }
}
