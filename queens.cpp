#include <iostream>
#include <cassert>
#include <vector>

/*
 *  For simplicity rotated along the main diagonal.
 */
void printConfiguration(std::vector<int> positions) {
    for(auto p : positions) {
        for (auto i = 0; i < positions.size(); i++)
            if (i == p)
                std::cout << "X ";
            else
                std::cout << "_ ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/*
 * Check whether queens in previous columns do not threaten queen in a given position.
 */
bool isSafe(int column_to_check, int row_to_check, std::vector<int> positions) {
    for (auto c = 0; c < column_to_check; c++) {
        auto other_row_pos = positions[c];
        if (other_row_pos == row_to_check || // Same row
            other_row_pos == row_to_check - (column_to_check - c) || // Same diagonal
            other_row_pos == row_to_check + (column_to_check - c))   // Same diagonal
            return false;    
    }
    return true;
}

/*
 * Place a queen in the given column. 'positions' is a vector of row ids for given columns where queens are already placed
 */
void queensRecursive(int column, std::vector<int> positions, std::vector<std::vector<int>> * solutions) {
     if (column < positions.size()) {         
        for (int row = 0; row < positions.size(); row++) {
            if (isSafe(column, row, positions)) {
                positions[column] = row;
                // Place another queen
                queensRecursive(column + 1, positions, solutions);
            }
        }
     } else
         solutions->push_back(positions);
}

/*
 * Returns all possible solutions for placing n queen on the n x n chess board using backtracking algorithm.
 */
std::vector<std::vector<int>> queens(int n) {
    std::vector<std::vector<int>> solutions;
    queensRecursive(0, std::vector<int>(n, -1), &solutions);
    return solutions;
}

int main() {
    assert(queens(4).size() == 2);   // There are 2   solutions for 4-queens problem
    assert(queens(5).size() == 10);  // There are 10  solutions for 5-queens problem
    assert(queens(6).size() == 4);   // There are 4   solutions for 6-queens problem
    assert(queens(7).size() == 40);  // There are 40  solutions for 7-queens problem
    assert(queens(8).size() == 92);  // There are 92  solutions for 8-queens problem
    assert(queens(9).size() == 352); // There are 352 solutions for 9-queens problem

    std::cout << "All tests have passed" << std::endl;
    std::cout << "One of the solutions for 8-queen problem:" << std::endl << std::endl;
    printConfiguration(queens(8).at(0));
}