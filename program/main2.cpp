#include <ctime>
#include <cstdio>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#define getTime(ini, fim) ((fim - ini) / (float) CLOCKS_PER_SEC)

class FlowshopBB {
private:
	int exploredNodes;
	clock_t initialTime;
	int limitExploredNodes;
	float maximumAllowedTime;

public:
	FlowshopBB(int _limitExploredNodes, int _maximumAllowedTime) {
		initialTime = clock(); /* initial execution time */

		exploredNodes = 0;
		limitExploredNodes = _limitExploredNodes;
		maximumAllowedTime = (float) _maximumAllowedTime;


		printf("limit maxtime: %d %f\n", limitExploredNodes, maximumAllowedTime);
	}

	~FlowshopBB() {}
};

int main(int argc, const char **argv) {
	if (argc == 3) {
		std::string line;
		std::stringstream ss;
		std::ifstream instranceFile(std::string(argv[1]));

		int limitExploredNodes;
		int maximumAllowedTime;

		// std::getline(instranceFile, line);
		// std::getline(instranceFile, line);

		// ss >> limitExploredNodes >> maximumAllowedTime;

		// FlowshopBB bb(limitExploredNodes, maximumAllowedTime);

		return 0;
	} else {
		printf("ERROR: you should pass 3 parameters. Instead, %d was/were passed!\n", argc);
		return 1;
	}
}
