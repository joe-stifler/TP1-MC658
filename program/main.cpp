/**
 * University: UNICAMP
 * Discipline: MC658
 * Professor: Cid C. de Souza
 * PED: Natanael Ramos
 * Data of creation: March 30, 2019
 * Author1 (RA 176665): Jose Ribeiro Neto <j176665@dac.unicamp.br>
 * Author2 (RA XXXXXX): <@>
 *
 * File: main.cpp
 **/

#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iostream>

#include "FlowshopBB.h"

int main(int argc, const char **argv) {
	if (argc == 3) {
		std::string line;
		std::ifstream paramsFile;
		paramsFile.open(argv[2], std::ios::in | std::ios::binary);

		std::ifstream instanceFile;
		instanceFile.open(argv[1], std::ios::in | std::ios::binary);

		if (!instanceFile.is_open()) {
    		printf("ERROR: Not possible to open instance file.");
			return 1;
		}

		if (!paramsFile.is_open()) {
    		printf("ERROR: Not possible to open params file.");
			return 1;
		}

		std::getline(paramsFile, line); /* Reads the params data (maximum number of nodes to explore). */

		int limitExploredNodes;
		std::stringstream ssNodes(line);
		ssNodes >> limitExploredNodes;

		std::getline(paramsFile, line); /* Reads the params data (maximum allowed execution time). */

		int maximumAllowedTime;
		std::stringstream ssTime(line);
		ssTime >> maximumAllowedTime;

		FlowshopBB fbb(limitExploredNodes, maximumAllowedTime);

		std::getline(instanceFile, line);
		std::stringstream instances(line);

		int tasks;
		instances >> tasks;

		if (tasks > MAXIMUM_TASKS_ALLOWED) {
			printf("ERROR: limit of tasks reached. You should pass a value less than %d tasks\n", MAXIMUM_TASKS_ALLOWED + 1);
			return 1;
		}

		/* Reads all the tasks execution time (for both machine 1 and machine 2) */
		for (int task = 0; task < tasks; ++task) {
			std::getline(instanceFile, line);
			std::stringstream taskStream(line);

			int d1, d2;
			taskStream >> d1 >> d2;

			fbb.addTask(d1, d2);
		}

		fbb.solve();

		Node bestNode = fbb.getBestNode();

		printf("BestNode: %d %d %d\n", bestNode.f1, bestNode.f2, bestNode.sumF2);
		printf("Explored nodes: %d\n", fbb.getExploredNodes());

		return 0;
	}

	printf("ERROR: you should pass 3 parameters: executable, instance file, params file. Instead, %d was/were passed!\n", argc);

	return 1;
}
