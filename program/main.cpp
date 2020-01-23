/**
 * University: UNICAMP
 * Discipline: MC658
 * Professor: Cid C. de Souza
 * PED: Natanael Ramos
 * Author1 (RA 176665): Jose Ribeiro Neto <j176665@dac.unicamp.br>
 * Author2 (RA XXXXXX): <@>
 *
 * File: main.cpp
 * Data of creation: March 29, 2019
 **/

#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iostream>
#include <random>

#include "FlowshopBB.h"

int main(int argc, const char **argv) {
	if (argc == 3) {
		clock_t initialTime = clock();

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

		unsigned long limitExploredNodes;
		std::stringstream ssNodes(line);
		ssNodes >> limitExploredNodes;

		std::getline(paramsFile, line); /* Reads the params data (maximum allowed execution time). */

		int maximumAllowedTime;
		std::stringstream ssTime(line);
		ssTime >> maximumAllowedTime;

		FlowshopBB fbb(limitExploredNodes, maximumAllowedTime, initialTime);

		std::getline(instanceFile, line);
		std::stringstream instances(line);

		int tasks;
		instances >> tasks;

		if (tasks >= MAXIMUM_TASKS_ALLOWED) {
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

		Node bestDual = fbb.getBestDual();
		Node bestPrimal = fbb.getBestPrimal();

		printf("%s,", argv[1]);

		if (bestPrimal.sumF2 == INT_MAX) {
			printf(",");
		} else {
			printf("%d,", bestPrimal.sumF2);
		}

		if (bestDual.sumF2 == INT_MAX) {
			printf(",");
		} else {
			printf("%d,", bestDual.sumF2);
		}

		printf("%lu,", fbb.getExploredNodes());

		if (bestPrimal.sumF2 == INT_MAX) {
			printf(",");
		} else {
			printf("%.2f,", fbb.getTimeFoundBestPrimal());
		}

		if (bestDual.sumF2 == INT_MAX) {
			printf(",");
		} else {
			printf("%.2f,", fbb.getTimeFoundBestDual());
		}

		printf("%.2f\n", fbb.getElapsedTime());

		// printf("\n");
		// printf("Nodos explorados: %ld\n", fbb.getExploredNodes());
		//
		// printf("Melhor primal: %d\n", bestPrimal.sumF2);
		// printf("Melhor tempo primal: %.2f\n", fbb.getTimeFoundBestPrimal());
		//
		// printf("Melhor dual: %d\n", bestDual.sumF2);
		// printf("Melhor tempo dual: %.2f\n", fbb.getTimeFoundBestDual());
		//
		// printf("Tempo total: %.2f\n", fbb.getElapsedTime());
		//
		// printf("Melhor solução: ");
		// for (int i = 0; i < tasks; ++i) {
		// 	if (i != 0) printf("-");
		// 	printf("%d", int(bestPrimal.orderTasks[i]) + 1);
		// }
		//
		// printf("\n");

		return 0;
	}

	printf("ERROR: you should pass 3 parameters: executable, instance file, params file. Instead, %d was/were passed!\n", argc);

	return 1;
}
