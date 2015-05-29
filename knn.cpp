#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <cmath>

/*
 * This implementation gives the same results as the one in R
 * You can check it by:
 * 
 * library(FNN)
 * library(MASS)
 * knn.reg(train=Boston[1:505,1:13], y=Boston[1:505,14], test=Boston[506,1:13], k=5)
 */

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

  std::random_shuffle(houses.begin(), houses.end());
  return houses;
}

/*
 * Compute euclidean distance between two vectors created from vectors vA and vB by taking all but the last of their elements.
 */
float euclideanDistance(std::vector<float> vA, std::vector<float> vB) {
  assert(vA.size() == vB.size());
  auto squaredDiff = [](float a, float b) {
    return std::pow(a - b, 2);
  };
  auto sumOfSquaredDiffs = std::inner_product(vA.begin(), vA.end() - 1, vB.begin(), 0, std::plus<float>(), squaredDiff);
  return std::sqrt(sumOfSquaredDiffs);
}

float knnRegression(FloatMatrix trainSet, std::vector<float> testExample, int k) {
  std::vector<std::tuple<float, float> > distance_and_output(trainSet.size());
  auto transformFunction = [testExample](std::vector<float> trainExample) {
    return std::make_tuple(euclideanDistance(trainExample, testExample), trainExample.back());
  };
  std::transform(trainSet.begin(), trainSet.end(), distance_and_output.begin(), transformFunction);
  std::sort(distance_and_output.begin(), distance_and_output.end(), [](std::tuple<float, float> do1, std::tuple<float, float> do2) {
    return std::get<0>(do1) < std::get<0>(do2);
  });

  std::vector<float> outputs(k);
  auto transformBackFunction = [](std::tuple<float, float> dist_and_out) {
    return std::get<1>(dist_and_out);
  };
  std::transform(distance_and_output.begin(), distance_and_output.begin() + k, outputs.begin(), transformBackFunction);
  float sum = std::accumulate(outputs.begin(), outputs.end(), 0.0);
  return sum/k;
}

int main() {
  FloatMatrix houses = getBoston();

  auto testsetSize = 50;
  auto k = 5;

  FloatMatrix trainset = FloatMatrix(houses.begin(), houses.end() - testsetSize);
  FloatMatrix testset =  FloatMatrix(houses.end() - testsetSize, houses.end());
  
  auto sum_error = 0.0;
  for (auto testExample : testset) {
    auto predicted = knnRegression(trainset, testExample, k);
    auto actual = testExample.back();
    auto error = std::abs(predicted - actual) / actual * 100;
    std::printf("Actual: %5.2f, predicted: %5.2f, error: %.0f%%\n", actual, predicted, error);
    sum_error += error;
  }
  auto avg_error = sum_error / testset.size();
  std::cout << "Average regression error: "<<  avg_error << "%" <<std::endl;

  assert(avg_error < 25.0);

  std::cout << "All tests have passed" << std::endl;
}