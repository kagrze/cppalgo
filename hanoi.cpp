#include <iostream>
#include <cassert>

/*
 * Function returns minimal number of steps required to move a disk of size @diskSize from 
 * page @src to @dst given @spare.
 * In addition the function logs steps to the standard output.
 */
int hanoi(unsigned int diskSize, char src, char dst, char spare) {
	assert(diskSize > 0);
	if (diskSize == 1) {
		std::cout << "Moving disk of size 1 from peg " << src << " to " << dst << std::endl;
		return 1;
	} else {
		auto a = hanoi(diskSize - 1, src, spare, dst);		
		std::cout << "Moving disk of size " << diskSize << " from peg " << src << " to " << dst << std::endl;
		auto b = hanoi(diskSize - 1, spare, dst, src);
		return a + 1 + b;
	}
}

void test_hanoi() {
	assert(hanoi(1, 'A', 'C', 'B') == 1);
	std::cout << std::endl;
	assert(hanoi(2, 'A', 'C', 'B') == 3);
    std::cout << std::endl;
	assert(hanoi(3, 'A', 'C', 'B') == 7);
    std::cout << std::endl;
    assert(hanoi(4, 'A', 'C', 'B') == 15);
}

int main() {
	test_hanoi();
	std::cout << "All tests have passed" << std::endl;
}