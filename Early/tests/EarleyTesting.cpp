#include <gtest/gtest.h>

#include <Earley.h>
#include <Reader.h>
#include <fstream>

std::string RunTest(std::istream& input_stream) {
  Grammar grammar = Reader::ReadGrammar(input_stream);
  Earley earley;
  earley.fit(grammar);
  int word_count;
  input_stream >> word_count;
  std::string word;
  std::getline(input_stream, word);
  std::string result;
  for (int word_num = 0; word_num < word_count; ++word_num) {
    std::getline(input_stream, word);
    result.push_back(earley.predict(word) ? '1' : '0');
  }
  return result;
}

TEST(EarleyTesting, FirstTest) {
  std::ifstream input_file("../tests/test1.txt");
  EXPECT_EQ(RunTest(input_file), "11010");
}

TEST(EarleyTesting, SecondTest) {
  std::ifstream input_file("../tests/test2.txt");
  EXPECT_EQ(RunTest(input_file), "01011");
}

TEST(EarleyTesting, FirdTest) {
  std::ifstream input_file("../tests/test3.txt");
  EXPECT_EQ(RunTest(input_file), "10101");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
