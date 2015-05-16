#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <cmath>

typedef std::vector<std::vector<float> > FloatMatrix;

/*
 * Download dataset from https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data
 * and place it inside datasets directory.
 */
FloatMatrix getBoston() {
  std::ifstream infile("datasets/housing.data");

  int featureNumber=14;
  FloatMatrix houses;

  float feature;
  std::vector<float> house;
  int i = 0;
  while(infile >> feature){
    house.push_back(feature);
    i++;
    if (i == featureNumber) {
      houses.push_back(house);
      house = std::vector<float>();
      i = 0;
    }
  }

  return houses;
}

/*
 * Compute euclidean distance between vectors vA and vB
 */
float euclideanDistance(std::vector<float> vA, std::vector<float> vB) {
  assert(vA.size() == vB.size());
  // std::cout << "vA:" << vA.at(0) << std::endl;
  // std::cout << "vB:" << vB.at(0) << std::endl;
  auto squaredDiff = [](float a, float b) {
    auto tmp = std::pow(a - b, 2);
    // std::cout << "squaredDiff:" << a << " " << b << " " << tmp << std::endl;
    return tmp;
  };
  auto sumOfSquaredDiffs = std::inner_product(vA.begin(), vA.end() - 1, vB.begin(), 0, std::plus<float>(), squaredDiff);
  auto tmp = std::sqrt(sumOfSquaredDiffs); 
  // std::cout << "euclideanDistance:" << tmp << std::endl;
  return tmp;
}

float knnRegression(FloatMatrix trainSet, std::vector<float> testExample, int k) {
  std::vector<std::tuple<float, float> > distance_and_output(trainSet.size());
  auto transformFunction = [testExample](std::vector<float> trainExample) {
    return std::make_tuple(euclideanDistance(trainExample, testExample), trainExample.back());
  };
  std::cout << "Before transform" << std::endl;
  std::transform(trainSet.begin(), trainSet.end(), distance_and_output.begin(), transformFunction);
  std::cout << "Before sort. First element: " << std::get<0>(distance_and_output.front()) << " " << std::get<1>(distance_and_output.front()) << std::endl;
  std::sort(distance_and_output.begin(), distance_and_output.end(), [](std::tuple<float, float> do1, std::tuple<float, float> do2) {
    return std::get<0>(do1) < std::get<0>(do2);
  });
  std::cout << "After sort. First element: " << std::get<0>(distance_and_output.front()) << " " << std::get<1>(distance_and_output.front()) << std::endl;
  std::cout << "After sort. Second element: " << std::get<0>(distance_and_output[1]) << " " << std::get<1>(distance_and_output[1]) << std::endl;
  std::cout << "After sort. Third element: " << std::get<0>(distance_and_output[2]) << " " << std::get<1>(distance_and_output[2]) << std::endl;

  std::vector<float> outputs(k);
  auto transformBackFunction = [](std::tuple<float, float> dist_and_out) {
    return std::get<1>(dist_and_out);
  };
  std::transform(distance_and_output.begin(), distance_and_output.begin() + k, outputs.begin(), transformBackFunction);
  for (auto output : outputs)
    std::cout << "Element: " << output << std::endl;
  float sum = std::accumulate(outputs.begin(), outputs.end(), 0.0);
  std::cout << "Sum: " << sum << std::endl;
	return sum/k;
}

int main() {
  FloatMatrix houses = getBoston();

  std::cout << houses.size() << std::endl;

  std::cout << houses[0][0] << std::endl;
  std::cout << houses[0][1] << std::endl;
  std::cout << houses[1][0] << std::endl;

  // FloatMatrix trainset = FloatMatrix(houses.begin() + 1, houses.begin() + 4);
  // FloatMatrix testset =  FloatMatrix(houses.begin(), houses.begin() + 1);

  FloatMatrix trainset = FloatMatrix(houses.begin(),    houses.end() - 50);
  FloatMatrix testset =  FloatMatrix(houses.end() - 50, houses.end());

  std::cout << trainset.size() << std::endl;
  std::cout << testset.size()  << std::endl;

  auto k = 5;
  auto error = 0.0;
  for (auto testExample : testset) {
    auto predicted = knnRegression(trainset, testExample, k);
    auto actual = testExample.back();
    error += std::abs(predicted - actual) / actual;
  }
  std::cout << "Average regression error: "<< error/testset.size() * 100 << " %" <<std::endl;
}