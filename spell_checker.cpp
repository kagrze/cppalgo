#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <string>
#include <algorithm>

/*
 * Download corpus form http://www.sls.hawaii.edu/bley-vroman/brown.txt
 * and place it inside datasets directory.
 * This method returns a map from words to the probability of their existence in the corpus.
 */
std::map <std::string, float> readCorpus() {
  auto corpus_file = "datasets/brown.txt";
  std::ifstream infile(corpus_file);

  std::map <std::string, float> vocabulary;

  std::string word;
  int corpus_size = 0;
  while(infile >> word){
    corpus_size ++;
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (vocabulary.count(word) == 1)
      vocabulary[word] = vocabulary[word] + 1;
    else
      vocabulary[word] = 1;
  }

  std::for_each(vocabulary.begin(), vocabulary.end(), [corpus_size](std::pair<const std::string, float> & p) {
    p.second = p.second / corpus_size;
  });

  return vocabulary;
}

/*
 * Measuring the minimal edit distance using Levenshtein distance
 * The Levenshtein distance is a generalization of the Hamming distance which allows only substitution.
 * This is a bottom-up dynamic programming algorithm for computing the Levenshtein distance
 */
int levenshtein(std::string a, std::string b) {
  std::vector<std::vector<int> > distances(a.size() + 1, std::vector<int>(b.size() + 1, 0));

  for (auto i=1; i<= a.size(); i++)
    distances.at(i).at(0) = i;
  for (auto j=1; j<= b.size(); j++)
    distances.at(0).at(j) = j;

  for(auto i=1; i <= a.size(); i++)
    for(auto j=1; j <= b.size(); j++)
      distances.at(i).at(j) = std::min(std::min(
        distances.at(i - 1).at(j    ) + 1,
        distances.at(i    ).at(j - 1) + 1),
        distances.at(i - 1).at(j - 1) + ((a.at(i - 1) != b.at(j - 1)) ? 1 : 0));

  return distances.back().back();
}

/*
 * Spell checker based on the Noisy Channel Model
 *               1
 * P(x|w) = -------------
 *          1 + lev(x, w)
 */
std::string correct(std::string misspelling, std::map <std::string, float> vocabulary) {
  std::map <std::string, float> candidatesProbabilites;

  for (auto p : vocabulary)
    if (levenshtein(p.first, misspelling) <= 2)
      candidatesProbabilites[p.first] = p.second / (1 + levenshtein(p.first, misspelling));

  auto max = max_element(
    candidatesProbabilites.begin(), candidatesProbabilites.end(),
    [](const std::pair<std::string, float> & p1, const std::pair<std::string, float> & p2) {
      return p1.second < p2.second;
    }
  );

  return max->first;
}

int main() {
  auto vocabulary = readCorpus();

  assert(correct("acress",  vocabulary) == "across"); // and not actress
  assert(correct("imature", vocabulary) == "nature"); // and not immature
  assert(correct("raning",  vocabulary) == "having"); // and not raining or running

  std::cout << "All tests have passed" << std::endl;
}