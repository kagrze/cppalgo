#include <iostream>
#include <cassert>
#include <algorithm>

#define DEBUG false

/*
 * Dynamic programming algorithm for solving to The knapsack problem
 * Returns a value of the obtimal subset of items
 */
float knapsack(std::vector<int> values, std::vector<int> weights, int totalCapacity) {
    assert(values.size() == weights.size());

    std::vector<std::vector<int> > totalValues(values.size() + 1, std::vector<int>(totalCapacity + 1, 0));

    for(int i=1; i <= values.size(); i++)
        for(int c=0; c <= totalCapacity; c++) {
            if (weights.at(i - 1) > c) {
                totalValues.at(i).at(c) = totalValues.at(i - 1).at(c);
            } else {
                totalValues.at(i).at(c) = std::max(
                    totalValues.at(i - 1).at(c),
                    totalValues.at(i - 1).at(c - weights.at(i - 1)) + values.at(i - 1));
            }
            if (DEBUG) std::cout << "Total value for the first " << i <<" items and capacity " << c << " is "<< totalValues.at(i).at(c) << std::endl;
        }

    return totalValues.back().back();
}

int main() {
    assert(knapsack({1, 2}, {2, 1}, 2) == 2);

    assert(knapsack({1, 2, 4}, {1, 1, 3}, 2) == 3);
    assert(knapsack({1, 2, 4}, {1, 1, 3}, 3) == 4);
    assert(knapsack({1, 2, 4}, {1, 1, 3}, 4) == 6);
    assert(knapsack({2, 1, 4}, {1, 1, 3}, 4) == 6);
    assert(knapsack({1, 2, 4}, {1, 1, 3}, 5) == 7);

    assert(knapsack({1, 4, 5, 4}, {4, 5, 7, 5}, 11) == 8);

    std::cout << "All tests have passed" << std::endl;
}